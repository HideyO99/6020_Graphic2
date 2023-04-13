#include "cBeholderManager.h"

void drawObj(cMeshObj* pCurrentMeshObject, glm::mat4x4 mat_PARENT_Model, cShaderManager* pShaderManager, cVAOManager* pVAOManager);

cBeholderManager::cBeholderManager()
{
}

cBeholderManager::~cBeholderManager()
{
}

void cBeholderManager::init()
{
	vecBeholder.clear();

}

void cBeholderManager::createBeholder()
{
	int row, col;
	do
	{
		row = rand() % 1000;
		col = rand() % 1000;

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
	
	vecBeholder.push_back(pBeholder);
}

void cBeholderManager::render()
{
	glm::mat4 matIdentity = glm::mat4(1.0f);
	for (int i = 0; i < vecBeholder.size(); i++)
	{
		drawObj(this->vecBeholder[i]->meshObj, matIdentity, pShaderManager, pVAOManager);
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
