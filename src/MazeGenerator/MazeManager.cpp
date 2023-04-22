#include "MazeManager.h"
#include <chrono>
#include <iostream>
#include "../Shader/cShaderManager.h"

CRITICAL_SECTION CS_MazeManager_ToBeLoadedVectorLock;
void drawObj(cMeshObj* pCurrentMeshObject, glm::mat4x4 mat_PARENT_Model, cShaderManager* pShaderManager, cVAOManager* pVAOManager);

struct sMazeThreadData
{
	sMazeThreadData()
	{
		//pTheMonster = NULL;
		bExitThread = false;
		bSuspendThread = false;
		// Pause for one frame at 60Hz (16 ms give or take)
		suspendTime_ms = 16;
	}
	cMazeMaker_W2023* pTheMaze;
	MazeManager* mazeManager;
	cShaderManager* pShaderManager;
	cVAOManager* pVAOManager;
	unsigned int* widthIndexCentre;
	unsigned int* heightIndexCentre;
	unsigned int* halfSizeInCells;
	std::vector< std::vector<bool> >* mazeRegion;
	// Setting to true exits thread
	bool bExitThread;
	// Setting to true will stop the update
	bool bSuspendThread;
	unsigned int suspendTime_ms;
	std::vector<cMeshObj*>*  vecMeshObj;
};

MazeManager::MazeManager()
{
	InitializeCriticalSection(&CS_MazeManager_ToBeLoadedVectorLock);

	for (unsigned int rowIndex = 0; rowIndex < 30; rowIndex++)
	{
		// Make an empty row
		mazeRegion.push_back(std::vector<bool>());

		for (unsigned int columnIndex = 0; columnIndex < 30; columnIndex++)
		{
			// Add an empty region
			mazeRegion[rowIndex].push_back(false);

		}//for ( unsigned int columnIndex
	}//for ( unsigned int rowIndex

	suspend = false;
}

MazeManager::~MazeManager()
{
	DeleteCriticalSection(&CS_MazeManager_ToBeLoadedVectorLock);
}

void MazeManager::CreateMaze(cVAOManager* pVAOManager)
{
	maker = new cMazeMaker_W2023();
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	cMazeMaker_W2023::sProcessMemoryCounters memInfoStart;
	//maker.GenerateMaze(MAZESIZE,MAZESIZE);
	//maker.getMemoryUse(memInfoStart);	
	maker->GenerateMaze(MAZESIZE,MAZESIZE);
	maker->reduceMaze2();
	maker->getMemoryUse(memInfoStart);
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	std::cout << "Generate Maze took " << time_span.count() << " seconds." << std::endl;
	cMazeMaker_W2023::sProcessMemoryCounters memInfoEnd;
	//maker.getMemoryUse(memInfoEnd);
	maker->getMemoryUse(memInfoEnd);

	unsigned long deltaMemoryUsed = memInfoEnd.WorkingSetSize - memInfoStart.WorkingSetSize;
	unsigned long deltaMemoryUsed_G = deltaMemoryUsed >> 30;
	unsigned long deltaMemoryUsed_M = (deltaMemoryUsed % (1024 * 1024 * 1024)) / (1024 * 1024);
	unsigned long deltaMemoryUsed_K = ((deltaMemoryUsed % (1024 * 1024 * 1024)) % (1024 * 1024)) / 1024;
	unsigned long deltaMemoryUsed_B = ((deltaMemoryUsed % (1024 * 1024 * 1024)) % (1024 * 1024)) % 1024;
	std::cout << "Delta memory:";
	std::cout << "\t" << deltaMemoryUsed_G << " G";// << std::endl;
	std::cout << "\t" << deltaMemoryUsed_M << " M";// << std::endl;
	std::cout << "\t" << deltaMemoryUsed_K << " K";// << std::endl;
	std::cout << "\t" << deltaMemoryUsed_B << " bytes" << std::endl;
	this->meshObj = pVAOManager->findMeshObjAddr("floorA1");


}

void MazeManager::UpdateArea(int mazeViewRowIndex, int mazeViewColumnIndex, int mazeViewSize, cShaderManager* pShaderManager, cVAOManager* pVAOManager)
{
	this->maker->GetMazeAreaSquare(mazeViewRowIndex, mazeViewColumnIndex, mazeViewSize, this->mazeRegion);

	this->meshObj->bDoNotLight = true;

	unsigned int mazeFullSize = mazeViewSize * 2;
	for (unsigned int rowIndex = 0; rowIndex < mazeFullSize; rowIndex++)
	{
		for (unsigned int columnIndex = 0; columnIndex < mazeFullSize; columnIndex++)
		{

			const float TILESIZE = 10.0f;  // How big each tile is in the world (each grid is 10 units)
			// Each mesh is 500x500 but that's too big
			const float MESHTOWORLDSCALE = TILESIZE / 500.0f;

			float cellXLocation = -(mazeViewSize * TILESIZE);
			float cellYLocation = -(mazeViewSize * TILESIZE);

			cellXLocation += (columnIndex * TILESIZE);
			cellYLocation += (rowIndex * TILESIZE);
			//todo
			this->meshObj->position.x = cellXLocation;
			this->meshObj->position.z = cellYLocation;
			this->meshObj->scale = glm::vec3(MESHTOWORLDSCALE);


			// If it's a WALL, draw a floor mesh there
			if (this->mazeRegion.size() != 0)
			{
				if (this->mazeRegion[rowIndex][columnIndex])
				{
					//todo
					this->meshObj->scale.y *= 20.0f;
					this->meshObj->position.y = 9.0f;
					this->meshObj->bDoNotLight = false;
				}
				else
				{
					this->meshObj->scale.y = MESHTOWORLDSCALE;
					this->meshObj->position.y = 0.0f;
					this->meshObj->bDoNotLight = true;

				}
				glm::mat4 matIdentity = glm::mat4(1.0f);
				drawObj(this->meshObj, matIdentity, pShaderManager, pVAOManager);
			}
		}
	}

}

DWORD WINAPI UpdateAreaThread(LPVOID pVOIDMaze)
{
	std::cout << "Starting the maze thread" << std::endl;
	sMazeThreadData* pMaze = (sMazeThreadData*)(pVOIDMaze);
	
	//std::vector< std::vector<bool> > mazeRegion;
	while (!pMaze->bExitThread)
	{
		if (!pMaze->bSuspendThread)
		{
			unsigned int mazeFullSize = *(pMaze->halfSizeInCells) * 2;
			if (pMaze->vecMeshObj->size() != mazeFullSize * mazeFullSize)
			{
				EnterCriticalSection(&CS_MazeManager_ToBeLoadedVectorLock);
				pMaze->mazeManager->suspend = true;
				pMaze->vecMeshObj->clear();
				for (int i = 0; i < mazeFullSize; i++)
				{
					for (int j = 0; j < mazeFullSize; j++)
					{
						cMeshObj* mesh = new cMeshObj();
						pMaze->vecMeshObj->push_back(mesh);
					}
				}
				pMaze->mazeManager->suspend = false;
				LeaveCriticalSection(&CS_MazeManager_ToBeLoadedVectorLock);
			}
			//if (pMaze->mazeManager->suspend)
			std::vector< std::vector<bool> > mazeRegion;
			
			pMaze->pTheMaze->GetMazeAreaSquare(*(pMaze->widthIndexCentre), *(pMaze->heightIndexCentre), *(pMaze->halfSizeInCells), pMaze->mazeManager->mazeRegion);
			

			//pMaze->mazeManager->meshObj->bDoNotLight = true;
			//pMaze->vecMeshObj->clear();
			
			for (unsigned int rowIndex = 0; rowIndex < mazeFullSize; rowIndex++)
			{
				for (unsigned int columnIndex = 0; columnIndex < mazeFullSize; columnIndex++)
				{
					cMeshObj* meshObj = pMaze->vecMeshObj->at(rowIndex * mazeFullSize + columnIndex);// new cMeshObj();
					meshObj->meshName = pMaze->mazeManager->meshObj->meshName;
					meshObj->bUse_RGBA_colour = pMaze->mazeManager->meshObj->bUse_RGBA_colour;
					const float TILESIZE = 10.0f;  // How big each tile is in the world (each grid is 10 units)
					// Each mesh is 500x500 but that's too big
					const float MESHTOWORLDSCALE = TILESIZE / 500.0f;

					float cellXLocation = -(*(pMaze->halfSizeInCells) * TILESIZE);
					float cellYLocation = -(*(pMaze->halfSizeInCells) * TILESIZE);

					cellXLocation += (columnIndex * TILESIZE);
					cellYLocation += (rowIndex * TILESIZE);

					meshObj->position.x = cellXLocation;
					meshObj->position.z = cellYLocation;
					meshObj->scale = glm::vec3(MESHTOWORLDSCALE);
					meshObj->textures[0] = pMaze->mazeManager->meshObj->textures[0];
					meshObj->textureRatios[0] = pMaze->mazeManager->meshObj->textureRatios[0];

					// If it's a WALL, draw a floor mesh there
					if (pMaze->mazeManager->mazeRegion.size() != 0)
					{
						if (pMaze->mazeManager->mazeRegion[rowIndex][columnIndex])
						{

							meshObj->scale.y *= 20.0f;
							meshObj->position.y = 9.0f;
							meshObj->bDoNotLight = false;
						}
						else
						{
							meshObj->scale.y = MESHTOWORLDSCALE;
							meshObj->position.y = 0.0f;
							meshObj->bDoNotLight = true;

						}
						//pMaze->vecMeshObj->at(0)->bUse_RGBA_colour = true;
						//pMaze->vecMeshObj->at(0)->color_RGBA = glm::vec4(1, 0, 0, 1);
						//pMaze->vecMeshObj->at(mazeFullSize*mazeFullSize -1 )->bUse_RGBA_colour = true;
						//pMaze->vecMeshObj->at(mazeFullSize*mazeFullSize -1 )->color_RGBA = glm::vec4(0, 1, 0, 1);
						//glm::mat4 matIdentity = glm::mat4(1.0f);
						//EnterCriticalSection(&CS_MazeManager_ToBeLoadedVectorLock);
						//pMaze->vecMeshObj->push_back(meshObj);
						//drawObj(pMaze->mazeManager->meshObj, matIdentity, pMaze->pShaderManager, pMaze->pVAOManager);
						//LeaveCriticalSection(&CS_MazeManager_ToBeLoadedVectorLock);
					}
				}
			}
				
			
		}
		else
		{
			// Put thread to sleep for X ms
			Sleep(pMaze->suspendTime_ms);
		}
		//EnterCriticalSection(&CS_MazeManager_ToBeLoadedVectorLock);

		//LeaveCriticalSection(&CS_MazeManager_ToBeLoadedVectorLock);
	}
	std::cout << "Exiting the maze thread" << std::endl;
	Sleep(50);

	return 0;
}

//void MazeManager::UpdateArea1(int mazeViewRowIndex, int mazeViewColumnIndex, int mazeViewSize)
//{
//	this->maker->GetMazeAreaSquare(mazeViewRowIndex, mazeViewColumnIndex, mazeViewSize, this->mazeRegion);
//}

void MazeManager::UpdateAreaAsync(int mazeViewRowIndex, int mazeViewColumnIndex, int mazeViewSize)
{
	//LPDWORD lpThreadId = NULL;
	//HANDLE hThread = 0;

	//sMazeThreadData* threadData = new sMazeThreadData();
	//threadData->mazeManager = this;
	//threadData->pTheMaze = maker;
	//threadData->widthIndexCentre = mazeViewRowIndex;
	//threadData->heightIndexCentre = mazeViewColumnIndex;
	//threadData->halfSizeInCells = mazeViewSize;
	//threadData->mazeRegion = &(this->mazeRegion);
	//threadData->vecMeshObj = &(this->vecMesh);

	//hThread =
	//	CreateThread(NULL,				// Security attributes
	//		0,					// Use default stack size
	//		UpdateAreaThread,	// Address of the function we are going to call
	//		(void*)threadData,			// Please wait a moment
	//		0,  // 0 or CREATE_SUSPENDED
	//		lpThreadId);
	//
	//Sleep(0);


}

void MazeManager::update(int mazeViewRowIndex, int mazeViewColumnIndex, int mazeViewSize, cShaderManager* pShaderManager, cVAOManager* pVAOManager)
{
	//int size = mazeViewSize * 2;
	////size has changed
	//if (vecMesh.size() != size * size)
	//{
	//	//suspend = true;
	//	ViewSize = mazeViewSize;
	//	vecMesh.clear();
	//	for (int i = 0; i < size; i++)
	//	{
	//		for (int j = 0; j < size; j++)
	//		{
	//			cMeshObj* mesh = new cMeshObj();
	//			vecMesh.push_back(mesh);
	//		}
	//	}
	//	//suspend = false;
	//}
	//UpdateAreaAsync( mazeViewRowIndex,  mazeViewColumnIndex,  mazeViewSize);
	//minViewRow = mazeViewRowIndex - mazeViewSize;
	//maxViewRow = mazeViewRowIndex + mazeViewSize;	
	//minViewCol = mazeViewColumnIndex - mazeViewSize;
	//maxViewCol = mazeViewColumnIndex + mazeViewSize;
	ViewRowIndex = mazeViewRowIndex;
	ViewColumnIndex = mazeViewColumnIndex;
	ViewSize = mazeViewSize;
	if(!this->suspend)
	{
		glm::mat4 matIdentity = glm::mat4(1.0f);
		for (int i = 0; i < vecMesh.size(); i++)
		{
			drawObj(this->vecMesh[i], matIdentity, pShaderManager, pVAOManager);
		}
	}
}

bool MazeManager::getMazeAtPos(int row, int col)
{
	if (row < 0 || col < 0 || row >= MAZESIZE-1 || col >= MAZESIZE-1)
	{
		return true;
	}

	return maker->maze2[row][col][0];
}

void MazeManager::setBeholderAtPos(int row, int col, bool available, int id)
{
	maker->maze2[row][col][1] = available;
	if (!available)
	{
		beholderMap[row][col] = id;
	}
	else
	{
		beholderMap[row][col] = NULL;
	}

}

bool MazeManager::getPos(int row, int col)
{
	if (row < 0 || col < 0 || row >= MAZESIZE - 1 || col >= MAZESIZE - 1)
	{
		return true;
	}

	return maker->maze2[row][col][1];
}

void MazeManager::startThread()
{
	int size = this->ViewSize * 2;
	if (this->vecMesh.size() != size * size)
	{
		this->vecMesh.clear();
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				cMeshObj* mesh = new cMeshObj();
				this->vecMesh.push_back(mesh);
			}
		}
	}

	LPDWORD lpThreadId = NULL;
	HANDLE hThread = 0;

	sMazeThreadData* threadData = new sMazeThreadData();
	threadData->mazeManager = this;
	threadData->pTheMaze = maker;
	threadData->widthIndexCentre = &(this->ViewRowIndex);
	threadData->heightIndexCentre = &(this->ViewColumnIndex);
	threadData->halfSizeInCells = &(this->ViewSize);
	threadData->mazeRegion = &(this->mazeRegion);
	threadData->vecMeshObj = &(this->vecMesh);

	hThread =
		CreateThread(NULL,				// Security attributes
			0,					// Use default stack size
			UpdateAreaThread,	// Address of the function we are going to call
			(void*)threadData,			// Please wait a moment
			0,  // 0 or CREATE_SUSPENDED
			lpThreadId);

	Sleep(0);
}

