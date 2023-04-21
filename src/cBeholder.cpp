#include "cBeholder.h"

//struct sMonsterThreadData
//{
//	sMonsterThreadData()
//	{
//		pTheMonster = NULL;
//		bExitThread = false;
//		bSuspendThread = false;
//		// Pause for one frame at 60Hz (16 ms give or take)
//		suspendTime_ms = 16;
//	}
//	cBeholder* pTheMonster;
//	
//	// Setting to true exits thread
//	bool bExitThread;
//	// Setting to true will stop the update
//	bool bSuspendThread;
//	unsigned int suspendTime_ms;
//};
//
//DWORD WINAPI MonsterThread(LPVOID pVOIDMonster)
//{
//	std::cout << "Starting the monster thread" << std::endl;
//	sMonsterThreadData* pMonster = (sMonsterThreadData*)(pVOIDMonster);
//
//	while (!pMonster->bExitThread)
//	{
//		if (!pMonster->bSuspendThread)
//		{
//			if (pMonster->pTheMonster->isMove)
//			{
//				pMonster->pTheMonster->ProcessMove();
//				pMonster->pTheMonster->rotate();
//			}
//			//pMonster->bSuspendThread = true;
//			//			Sleep(0);		// Release this thread if needed
//		}
//		else
//		{
//			// Put thread to sleep for X ms
//			Sleep(pMonster->suspendTime_ms);
//			//pMonster->bSuspendThread = false;
//		}
//	}
//
//	std::cout << "Exiting the monster thread" << std::endl;
//	return 0;
//}

cBeholder::cBeholder()
{
	move = NONE;
	alive = true;
}

cBeholder::~cBeholder()
{
}

void cBeholder::ProcessMove()
{
	bool u, d, l, r;
	checkSurround(u, d, l, r);
	moveDir newD = move;
	m_prevPosRow = PosRow;
	m_prevPosCol = PosCol;
	
	switch (move)
	{
	case UP:
		if (u)
		{
			if (r&&l)
			{
				newD = DOWN;
			}
			else if(!r&&!l)
			{
				if (rand() % 2)
				{
					newD = RIGHT;
				}
				else
				{
					newD = LEFT;
				}
			}
			else
			{
				if (r)
				{
					newD = LEFT;
				}
				else
				{
					newD = RIGHT;
				}
			}
		}
		else
		{
			PosRow--;
		}
		break;
	case DOWN:
		if (d)
		{
			if (r && l)
			{
				newD = UP;
			}
			else if (!r && !l)
			{
				if (rand() % 2)
				{
					newD = RIGHT;
				}
				else
				{
					newD = LEFT;
				}
			}
			else
			{
				if (r)
				{
					newD = LEFT;
				}
				else
				{
					newD = RIGHT;
				}
			}
		}
		else
		{
			PosRow++;
		}
		break;
	case LEFT:
		if (l)
		{
			if (u && d)
			{
				newD = RIGHT;
			}
			else if (!u && !d)
			{
				if (rand() % 2)
				{
					newD = UP;
				}
				else
				{
					newD = DOWN;
				}
			}
			else
			{
				if (u)
				{
					newD = DOWN;
				}
				else
				{
					newD = UP;
				}
			}
		}
		else
		{
			PosCol--;
		}
		break;
	case RIGHT:
		if (r)
		{
			if (u && d)
			{
				newD = LEFT;
			}
			else if (!u && !d)
			{
				if (rand() % 2)
				{
					newD = UP;
				}
				else
				{
					newD = DOWN;
				}
			}
			else
			{
				if (u)
				{
					newD = DOWN;
				}
				else
				{
					newD = UP;
				}
			}
		}
		else
		{
			PosCol++;
		}
		break;
	case NONE:
		switch (rand()%4)
		{
		case 0:
			newD = UP;
			break;
		case 1:
			newD = DOWN;
			break;
		case 2:
			newD = LEFT;
			break;
		case 3:
			newD = RIGHT;
			break;
		default:
			break;
		};
		break;
	default:
		break;
	}
	move = newD;
	mazeManager->setBeholderAtPos(m_prevPosRow, m_prevPosCol, true, id);
	mazeManager->setBeholderAtPos(PosRow, PosCol, false,id);
}

void cBeholder::checkSurround(bool& u, bool& d, bool& l, bool& r)
{
	u = mazeManager->getMazeAtPos(PosRow - 1, PosCol);
	d = mazeManager->getMazeAtPos(PosRow + 1, PosCol);

	l = mazeManager->getMazeAtPos(PosRow,	PosCol - 1);
	r = mazeManager->getMazeAtPos(PosRow,	PosCol + 1);
}

void cBeholder::rotate()
{
	switch (move)
	{
	case UP:
		meshObj->rotation = glm::vec3(0);
		break;
	case DOWN:
		meshObj->rotation = glm::vec3(0,PI,0);
		break;
	case LEFT:
		meshObj->rotation = glm::vec3(0, HALFPI, 0);
		break;
	case RIGHT:
		meshObj->rotation = glm::vec3(0, -HALFPI, 0);
		break;
	case NONE:
		meshObj->rotation = glm::vec3(0);
		break;
	default:
		break;
	}
}

//void cBeholder::startThread()
//{
//	LPDWORD lpThreadId = NULL;
//	HANDLE hThread = 0;
//	sMonsterThreadData* threadData = new sMonsterThreadData();
//	threadData->pTheMonster = this;
//	hThread =
//		CreateThread(NULL,				// Security attributes
//			0,					// Use default stack size
//			MonsterThread,	// Address of the function we are going to call
//			(void*)threadData,			// Please wait a moment
//			0,  // 0 or CREATE_SUSPENDED
//			lpThreadId);
//	
//	Sleep(0);
//}

bool cBeholder::scanEnemy(moveDir d)
{
	int R = PosRow;
	int C = PosCol;
	bool foundEnemy = false;

	switch (d)
	{
	case UP:
		R--;
		break;
	case DOWN:
		R++;
		break;
	case LEFT:
		C--;
		break;
	case RIGHT:
		C++;
		break;
	case NONE:
		break;
	default:
		break;
	}
	if (R != PosRow || C != PosCol)
	{
		foundEnemy = !(mazeManager->getPos(R, C));
	}
	if (foundEnemy)
	{
		int enemyID = mazeManager->beholderMap[R][C];
		attack(enemyID);
	}
	
	return false;
}

void cBeholder::attack(int enemyID)
{
	pVecBeholder->at(enemyID)->dead();
}

void cBeholder::dead()
{
	this->alive = false;
	//meshObj->scale = glm::vec3(1);
	meshObj->rotation = glm::vec3(HALFPI, PI, 0);
}

void cBeholder::calWorldPos()
{
	const float TILESIZE = 10.0f;  // How big each tile is in the world (each grid is 10 units)
	const float offset = -5.0f;  // 
	// Each mesh is 500x500 but that's too big
	const float MESHTOWORLDSCALE = TILESIZE / 5.f;

	float cellXLocation = ((PosCol * TILESIZE) + offset);
	float cellYLocation = ((PosRow * TILESIZE) + offset);
	//todo
	this->meshObj->position.x = cellXLocation;
	this->meshObj->position.y = 3.f;
	this->meshObj->position.z = cellYLocation;
	//this->meshObj->scale = glm::vec3(MESHTOWORLDSCALE);
}

void cBeholder::update()
{
	if (this->alive)
	{
		//ProcessMove();
		rotate();
		//scanEnemy(move);
	}
}
