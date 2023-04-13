#include "MazeManager.h"
#include <chrono>
#include <iostream>

CRITICAL_SECTION CS_MazeManager_ToBeLoadedVectorLock;

MazeManager::MazeManager()
{
	InitializeCriticalSection(&CS_MazeManager_ToBeLoadedVectorLock);
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

	while (!pMaze->bExitThread)
	{
		if (!pMaze->bSuspendThread)
		{
			//EnterCriticalSection(&g_CS);
			pMaze->pTheMaze->GetMazeAreaSquare(pMaze->widthIndexCentre, pMaze->heightIndexCentre, pMaze->halfSizeInCells, *pMaze->mazeRegion);
			//LeaveCriticalSection(&g_CS);
			std::cout << "mazeRegion size = " << pMaze->mazeRegion->size() << std::endl;

			Sleep(0);		// Release this thread if needed
		}
		else
		{
			// Put thread to sleep for X ms
			Sleep(pMaze->suspendTime_ms);
		}
	}

	//std::cout << "mazeRegion size = " << pMaze->mazeRegion->size()<< std::endl;
	return 0;
}

void MazeManager::UpdateArea(int mazeViewRowIndex, int mazeViewColumnIndex, int mazeViewSize)
{
	//InitializeCriticalSection(&g_CS);
	LPDWORD lpThreadId = NULL;
	HANDLE hThread = 0;

	sMazeThreadData* threadData = new sMazeThreadData();
	threadData->pTheMaze = maker;
	threadData->widthIndexCentre = mazeViewRowIndex;
	threadData->heightIndexCentre = mazeViewColumnIndex;
	threadData->halfSizeInCells = mazeViewSize;
	threadData->mazeRegion = &mazeRegion;

	hThread =
		CreateThread(NULL,				// Security attributes
			0,					// Use default stack size
			UpdateAreaThread,	// Address of the function we are going to call
			(void*)threadData,			// Please wait a moment
			0,  // 0 or CREATE_SUSPENDED
			lpThreadId);

	Sleep(0);
	//EnterCriticalSection(&g_CS);
	//memcpy(&this->mazeRegion, &threadData->mazeRegion, mazeViewSize * mazeViewSize * sizeof(bool));
	//LeaveCriticalSection(&g_CS);
	

	//for (size_t i = 0; i < mazeViewSize*2; i++)
	//{
	//	mazeRegion.push_back(std::vector<bool>());
	//	for (size_t j = 0; j < mazeViewSize*2; j++)
	//	{
	//		mazeRegion[i].push_back(threadData->mazeRegion[i][j]);
	//	}
	//}

	threadData->bExitThread = true;
	//DeleteCriticalSection(&g_CS);
}

