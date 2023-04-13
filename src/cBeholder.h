#pragma once
#include "MazeGenerator/MazeManager.h"

#define PI 3.14159
#define HALFPI 1.57079

enum moveDir
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NONE
};

class cBeholder
{
public:
	cBeholder();
	~cBeholder();

	void update();
	
	int PosRow;
	int PosCol;
	cMeshObj* meshObj;
	MazeManager* mazeManager;

private:
	int m_prevPosRow;
	int m_prevPosCol;
	moveDir move;
	void ProcessMove();
	void checkSurround(bool& u, bool& d, bool& l, bool& r);
	void rotate();
	void calWorldPos();

};
