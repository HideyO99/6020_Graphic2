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
		createBeholder(i);
	}
}

void cBeholderManager::createBeholder(int id)
{
	int row, col;
	do
	{
		row = rand() % (MAZESIZE - 1);
		col = rand() % (MAZESIZE - 1);

	} while (!isAvailable(row, col));

	cBeholder* pBeholder = new cBeholder();
	pBeholder->id = id;
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
	pBeholder->pVecBeholder = &(this->vecBeholder);
	
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

			float cellXLocation = ((this->vecBeholder[i]->PosCol - int(m_mazeManager->ViewColumnIndex)) * TILESIZE);
			float cellYLocation = ((this->vecBeholder[i]->PosRow - int(m_mazeManager->ViewRowIndex)) * TILESIZE);

			this->vecBeholder[i]->meshObj->position.x = cellXLocation + offset;
			this->vecBeholder[i]->meshObj->position.y = 4;
			this->vecBeholder[i]->meshObj->position.z = cellYLocation + offset;
			this->vecBeholder[i]->meshObj->scale = glm::vec3(20.f);
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
	startThread();
	//if(g_startGame)
	//{
	//	for (int i = 0; i < vecBeholder.size(); i++)
	//	{
	//		vecBeholder[i]->isMove = g_startGame;
	//	}
	//}
	render();
}

bool cBeholderManager::isInSight(int inputRow, int inputCol)
{
	int minRow = m_mazeManager->ViewRowIndex - m_mazeManager->ViewSize;
	int maxRow = m_mazeManager->ViewRowIndex + m_mazeManager->ViewSize;
	int minCol = m_mazeManager->ViewColumnIndex - m_mazeManager->ViewSize;
	int maxCol = m_mazeManager->ViewColumnIndex + m_mazeManager->ViewSize;
	bool result = (minRow <= inputRow) && (inputRow < maxRow) && (minCol <= inputCol) && (inputCol < maxCol);

	return result;
}

struct sMonsterThreadData
{
	sMonsterThreadData()
	{
		pTheMonster = NULL;
		bExitThread = false;
		bSuspendThread = false;
		// Pause for one frame at 60Hz (16 ms give or take)
		suspendTime_ms = 16;
	}
	cBeholderManager* pTheMonster;

	// Setting to true exits thread
	bool bExitThread;
	// Setting to true will stop the update
	bool bSuspendThread;
	unsigned int suspendTime_ms;
};

DWORD WINAPI beholderThread(LPVOID pVOIDMonster)
{
	std::cout << "Starting the beholder manager thread" << std::endl;
	sMonsterThreadData* pMonster = (sMonsterThreadData*)(pVOIDMonster);

	while (!pMonster->bExitThread)
	{
		if (!pMonster->bSuspendThread)
		{
			for (int i = 0; i < pMonster->pTheMonster->vecBeholder.size(); i++)
			{
				pMonster->pTheMonster->vecBeholder[i]->update();
			}
			//pMonster->bSuspendThread = true;
						Sleep(200);		// Release this thread if needed
		}
		else
		{
			// Put thread to sleep for X ms
			Sleep(pMonster->suspendTime_ms);
			//pMonster->bSuspendThread = false;
		}
	}

	std::cout << "Exiting the monster thread" << std::endl;
	return 0;
}

void cBeholderManager::oneThread()
{
	LPDWORD lpThreadId = NULL;
	HANDLE hThread = 0;
	sMonsterThreadData* threadData = new sMonsterThreadData();
	threadData->pTheMonster = this;
	hThread =
		CreateThread(NULL,				// Security attributes
			0,					// Use default stack size
			beholderThread,	// Address of the function we are going to call
			(void*)threadData,			// Please wait a moment
			CREATE_SUSPENDED,  // 0 or CREATE_SUSPENDED
			lpThreadId);
	threadID = hThread;
	Sleep(0);
}

void cBeholderManager::startThread()
{
	ResumeThread(threadID);
}

