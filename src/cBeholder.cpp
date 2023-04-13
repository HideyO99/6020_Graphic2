#include "cBeholder.h"

cBeholder::cBeholder()
{
	move = NONE;
}

cBeholder::~cBeholder()
{
}

void cBeholder::ProcessMove()
{
	bool u, d, l, r;
	checkSurround(u, d, l, r);
	moveDir newD = move;
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
	this->meshObj->scale = glm::vec3(MESHTOWORLDSCALE);
}

void cBeholder::update()
{
	
	ProcessMove();
	rotate();
	//calWorldPos();
}
