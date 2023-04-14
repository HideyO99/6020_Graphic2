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
	int id;
	int PosRow;
	int PosCol;
	bool alive;
	cMeshObj* meshObj;
	MazeManager* mazeManager;
	std::vector<cBeholder*>* pVecBeholder;

	void calWorldPos();
	void ProcessMove();
	void rotate();
	void startThread();
	bool isMove;
	bool scanEnemy(moveDir d);
	void attack(int enemyID);
	void dead();

private:
	int m_prevPosRow;
	int m_prevPosCol;
	moveDir move;
	void checkSurround(bool& u, bool& d, bool& l, bool& r);

};
