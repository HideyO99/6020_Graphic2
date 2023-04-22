#pragma once
#include "../Animation/Character.h"
#include "../MeshObj/cMeshObj.h"
#include "../AI/stateMachine.h"
#include "../AI/cSteering.h"

class Monster
{
public:
	Monster();
	~Monster();

	void update(double dt);
	int id;
	int PosRow;
	int PosCol;
	bool alive;
	Character* charAnimate;
	cMeshObj* meshObj;
	stateMachine* task;
	cSteering* steering;
	//MazeManager* mazeManager;
	//std::vector<cBeholder*>* pVecBeholder;

	void calWorldPos();
	void ProcessMove();
	void rotate();
	//void startThread();
	bool isMove;
	//bool scanEnemy(moveDir d);
	void attack();
	void dead();

private:
	int m_prevPosRow;
	int m_prevPosCol;
	//moveDir move;
	//void checkSurround(bool& u, bool& d, bool& l, bool& r);
};

