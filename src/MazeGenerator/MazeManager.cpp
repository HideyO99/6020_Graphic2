#include "MazeManager.h"
#include <chrono>
#include <iostream>

CRITICAL_SECTION CS_MazeManager_ToBeLoadedVectorLock;

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

DWORD WINAPI UpdateAreaThread(LPVOID pVOIDMaze)
{
	//std::cout << "Starting the monster thread" << std::endl;
	sMazeThreadData* pMaze = (sMazeThreadData*)(pVOIDMaze);

	std::vector< std::vector<bool> > mazeRegion;

	pMaze->pTheMaze->GetMazeAreaSquare(pMaze->widthIndexCentre, pMaze->heightIndexCentre, pMaze->halfSizeInCells, mazeRegion);
	unsigned int mazeSize = pMaze->halfSizeInCells * 2;

	EnterCriticalSection(&CS_MazeManager_ToBeLoadedVectorLock);
	pMaze->mazeRegion->clear();

	for (int i = 0; i < mazeSize; i++)
	{
		std::vector<bool> b;
		for (int j = 0; j < mazeSize; j++)
		{
			b.push_back(mazeRegion[i][j]);
		}
		pMaze->mazeRegion->push_back(b);
	}

	LeaveCriticalSection(&CS_MazeManager_ToBeLoadedVectorLock);

	Sleep(0);

	return 0;
}

void MazeManager::UpdateArea(int mazeViewRowIndex, int mazeViewColumnIndex, int mazeViewSize)
{
	this->maker->GetMazeAreaSquare(mazeViewRowIndex, mazeViewColumnIndex, mazeViewSize, this->mazeRegion);
}

void MazeManager::UpdateAreaAsync(int mazeViewRowIndex, int mazeViewColumnIndex, int mazeViewSize)
{
	//InitializeCriticalSection(&g_CS);
	LPDWORD lpThreadId = NULL;
	HANDLE hThread = 0;

	sMazeThreadData* threadData = new sMazeThreadData();
	threadData->pTheMaze = maker;
	threadData->widthIndexCentre = mazeViewRowIndex;
	threadData->heightIndexCentre = mazeViewColumnIndex;
	threadData->halfSizeInCells = mazeViewSize;
	threadData->mazeRegion = &(this->mazeRegion);

	hThread =
		CreateThread(NULL,				// Security attributes
			0,					// Use default stack size
			UpdateAreaThread,	// Address of the function we are going to call
			(void*)threadData,			// Please wait a moment
			0,  // 0 or CREATE_SUSPENDED
			lpThreadId);

	Sleep(0);

	threadData->bExitThread = true;
	//DeleteCriticalSection(&g_CS);
}

