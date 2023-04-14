#pragma once
#include "cMazeMaker_W2023.h"
#include "..\Shader\cShaderManager.h"
#include "../VAOManager/cVAOManager.h"
#include <Windows.h>
#include <assert.h>

#define MAZESIZE 1000



class MazeManager
{
public:
	MazeManager();
	~MazeManager();

	void CreateMaze(cVAOManager* pVAOManager);
	void UpdateArea(int mazeViewRowIndex, int mazeViewColumnIndex, int mazeViewSize, cShaderManager* pShaderManager, cVAOManager* pVAOManager);
	//void UpdateArea1(int mazeViewRowIndex, int mazeViewColumnIndex, int mazeViewSize);
	void UpdateAreaAsync(int mazeViewRowIndex, int mazeViewColumnIndex, int mazeViewSize);
	void update(int mazeViewRowIndex, int mazeViewColumnIndex, int mazeViewSize, cShaderManager* pShaderManager, cVAOManager* pVAOManager);
	//DWORD WINAPI UpdateAreaThread(LPVOID pVOIDMonster);
	bool getMazeAtPos(int row, int col);
	void setBeholderAtPos(int row, int col, bool available, int id);
	bool getPos(int row, int col);
	void startThread();
	//int minViewRow;
	//int maxViewRow;
	//int minViewCol;
	//int maxViewCol;
	unsigned int ViewRowIndex;
	unsigned int ViewColumnIndex;
	unsigned int ViewSize;

	bool suspend;
	cMazeMaker_W2023* maker;
	cMeshObj* meshObj;
	std::vector< std::vector<bool> > mazeRegion;
	std::vector <cMeshObj*> vecMesh;
	int beholderMap[MAZESIZE-1][MAZESIZE-1];
};

