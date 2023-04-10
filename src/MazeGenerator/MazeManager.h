#pragma once
#include "cMazeMaker_W2023.h"
#include "../VAOManager/cVAOManager.h"

#define MAZESIZE 1000

class MazeManager
{
public:
	MazeManager();
	~MazeManager();

	void CreateMaze(cVAOManager* pVAOManager);
	cMazeMaker_W2023* maker;
};

