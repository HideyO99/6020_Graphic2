#include "MazeManager.h"
#include <chrono>
#include <iostream>

MazeManager::MazeManager()
{
}

MazeManager::~MazeManager()
{
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



}
