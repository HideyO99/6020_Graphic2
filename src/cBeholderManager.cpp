#include "cBeholderManager.h"

extern bool g_startGame;
void drawObj(cMeshObj* pCurrentMeshObject, glm::mat4x4 mat_PARENT_Model, cShaderManager* pShaderManager, cVAOManager* pVAOManager);

cBeholderManager::cBeholderManager()
{
}

cBeholderManager::~cBeholderManager()
{
}

void cBeholderManager::init(MazeManager* maze, cVAOManager* vao, cMeshObj* mesh, cShaderManager* shader)
{
	vecBeholder.clear();
	this->m_mazeManager = maze;
	this->pVAOManager = vao;
	this->meshObj = mesh;
	this->pShaderManager = shader;

	for (int i = 0; i < NUMBEHOLDER; i++)
	{
		createBeholder();
	}
}

void cBeholderManager::createBeholder()
{
	int row, col;
	do
	{
		row = rand() % (MAZESIZE - 1);
		col = rand() % (MAZESIZE - 1);

	} while (!isAvailable(row, col));

	cBeholder* pBeholder = new cBeholder();
	pBeholder->PosRow = row;
	pBeholder->PosCol = col;
	pBeholder->mazeManager = m_mazeManager;
	pBeholder->meshObj = new cMeshObj();
	pBeholder->meshObj->bDoNotLight = true;
	pBeholder->meshObj->bUse_RGBA_colour = false;
	pBeholder->meshObj->meshName = this->meshObj->meshName;
	pBeholder->meshObj->textures[0] = this->meshObj->textures[0];
	pBeholder->meshObj->textureRatios[0] = this->meshObj->textureRatios[0];
	pBeholder->meshObj->scale = this->meshObj->scale;
	pBeholder->calWorldPos();
	pBeholder->startThread();
	
	vecBeholder.push_back(pBeholder);
}

void cBeholderManager::render()
{
	glm::mat4 matIdentity = glm::mat4(1.0f);
	for (int i = 0; i < vecBeholder.size(); i++)
	{
		if (isInSight(this->vecBeholder[i]->PosRow, this->vecBeholder[i]->PosCol))
		{
			const float TILESIZE = 10.0f;
			const float offset = -5.0f;

			float cellXLocation = ((this->vecBeholder[i]->PosCol - m_mazeManager->ViewColumnIndex) * TILESIZE);
			float cellYLocation = ((this->vecBeholder[i]->PosRow - m_mazeManager->ViewRowIndex) * TILESIZE);

			this->vecBeholder[i]->meshObj->position.x = cellXLocation + offset;
			this->vecBeholder[i]->meshObj->position.z = cellYLocation + offset;

			drawObj(this->vecBeholder[i]->meshObj, matIdentity, pShaderManager, pVAOManager);
		}
	}
}

bool cBeholderManager::isAvailable(int row, int col)
{
	if (m_mazeManager->getMazeAtPos(row, col))
	{
		return false;
	}
	
	for (int i = 0; i < vecBeholder.size(); i++)
	{
		if (vecBeholder[i]->PosCol == col && vecBeholder[i]->PosRow == row)
		{
			return false;
		}
	}

	return true;
}

void cBeholderManager::update()
{
	//if(g_startGame)
	{
		for (int i = 0; i < vecBeholder.size(); i++)
		{
			vecBeholder[i]->isMove = g_startGame;
		}
	}
	render();
}

bool cBeholderManager::isInSight(int inputRow, int inputCol)
{
	int R = inputRow - m_mazeManager->ViewRowIndex;
	int C = inputCol - m_mazeManager->ViewColumnIndex;
	bool result = (abs(R) < m_mazeManager->ViewSize && abs(C) < m_mazeManager->ViewSize);

	return result;
}
