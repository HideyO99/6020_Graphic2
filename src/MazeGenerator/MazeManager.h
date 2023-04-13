#pragma once
#include "cMazeMaker_W2023.h"
#include "../VAOManager/cVAOManager.h"
#include <Windows.h>
#include <assert.h>

#define MAZESIZE 1000

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

	unsigned int widthIndexCentre;
	unsigned int heightIndexCentre;
	unsigned int halfSizeInCells;
	std::vector< std::vector<bool> >* mazeRegion;
	// Setting to true exits thread
	bool bExitThread;
	// Setting to true will stop the update
	bool bSuspendThread;
	unsigned int suspendTime_ms;
};

class MazeManager
{
public:
	MazeManager();
	~MazeManager();

	void CreateMaze(cVAOManager* pVAOManager);
	void UpdateArea(int mazeViewRowIndex, int mazeViewColumnIndex, int mazeViewSize);
	void UpdateAreaAsync(int mazeViewRowIndex, int mazeViewColumnIndex, int mazeViewSize);

	//DWORD WINAPI UpdateAreaThread(LPVOID pVOIDMonster);
	cMazeMaker_W2023* maker;
	cMeshObj* meshObj;
	std::vector< std::vector<bool> > mazeRegion;
};

