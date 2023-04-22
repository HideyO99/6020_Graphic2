#include "MonsterManager.h"
#include <chrono>
#include "../Animation/AnimationManager.h"

extern AnimationManager* g_pAnimationManager;
void drawObj(cMeshObj* pCurrentMeshObject, glm::mat4x4 mat_PARENT_Model, cShaderManager* pShaderManager, cVAOManager* pVAOManager);

MonsterManager::MonsterManager()
{
}

MonsterManager::~MonsterManager()
{
}

void MonsterManager::init(MazeManager* maze, cVAOManager* vao, cMeshObj* mesh, cShaderManager* shader)
{
	const char* ANIMATION1 = "asset/model/MremirehODesbiens@asciiIdel.fbx";
	const char* ANIMATION2 = "asset/model/MremirehODesbiens@Walk.fbx";
	const char* ANIMATION3 = "asset/model/MremirehODesbiens@ZombieAttack.fbx";
	const char* ANIMATION4 = "asset/model/MremirehODesbiens@Dying.fbx";
	//std::string charFBX = "asset/model/MremirehODesbiens@ZombieIdle.fbx";
	std::string charFBX = "asset/model/MremirehODesbiens@asciiIdel.fbx";


	vecMonster.clear();
	this->m_mazeManager = maze;
	this->pVAOManager = vao;
	this->meshObj = mesh;
	this->pShaderManager = shader;
	
	this->prototypeCharacter = new Character();
	std::vector<std::string> animation;
	animation.push_back(ANIMATION1);
	animation.push_back(ANIMATION2);
	animation.push_back(ANIMATION3);
	animation.push_back(ANIMATION4);
	//this->prototypeCharacter.
	this->prototypeCharacter->LoadCharacterFromAssimp(charFBX.c_str());
	
	for (int i = 0; i < animation.size(); i++)
	{
		// Load animation
		this->prototypeCharacter->LoadAnimationFromAssimp(animation[i].c_str());
	}
	for (int i = 0; i < NUMMONSTER; i++)
	{
		createMonster(i);
	}
	//this->meshObj->Animation.IsCharacterAnimation = true;
	//this->meshObj->Animation.AnimationTime = 0.f;
	//this->meshObj->Animation.IsLooping = true;
	//this->meshObj->Animation.IsPlaying = true;
	////this->meshObj->Animation.AnimationType = m_AnimationName;
	//this->meshObj->Animation.Speed = 1.f;
	//this->meshObj->position = glm::vec3(0.f, 0.f, 0.f);
	//this->meshObj->hasBone = true;
}

void MonsterManager::createMonster(int id)
{
	int row, col;
	do
	{
		row = rand() % (MAZESIZE - 1);
		col = rand() % (MAZESIZE - 1);

	} while (!isAvailable(row, col));

	Monster* pMonster = new Monster();
	pMonster->id = id;
	pMonster->PosRow = row;
	pMonster->PosCol = col;
	pMonster->meshObj = new cMeshObj();
	pMonster->meshObj->bDoNotLight = true;
	pMonster->meshObj->bUse_RGBA_colour = false;
	pMonster->meshObj->meshName = this->prototypeCharacter->getName();
	pMonster->meshObj->instanceName = std::to_string(id);

	pMonster->meshObj->textures[0] = this->meshObj->textures[0];
	pMonster->meshObj->textureRatios[0] = this->meshObj->textureRatios[0];
	//pMonster->meshObj->scale = this->meshObj->scale;
	pMonster->meshObj->scale = glm::vec3(0.05f);
	//pMonster->charAnimate = this->prototypeCharacter;
	pMonster->charAnimate = new Character();
	*pMonster->charAnimate = *(this->prototypeCharacter);

	pMonster->meshObj->Animation.IsCharacterAnimation = true;
	pMonster->meshObj->Animation.AnimationTime = 0.f;
	pMonster->meshObj->Animation.IsLooping = true;
	pMonster->meshObj->Animation.IsPlaying = true;
	pMonster->meshObj->Animation.Speed = 1.f;
	//pMonster->meshObj->position = glm::vec3(id*50, 4.f, -5.f);
	pMonster->calWorldPos();
	pMonster->meshObj->hasBone = true;
	pMonster->meshObj->Animation.AnimationType = "mixamo.com";
	pMonster->task->pcharacter = pMonster->charAnimate;
	pMonster->task->SetState(new IdleState(pMonster->meshObj->position, 0));
	//pMonster->charAnimate->SetAnimation(0);
	//g_pAnimationManager->animationOBJList.push_back(pMonster->meshObj);
	this->vecMonster.push_back(pMonster);
}

void MonsterManager::update(float elapsed)
{
	//startThread();
	for (int i = 0; i < vecMonster.size(); i++)
	{
		vecMonster[i]->update(elapsed);
	}
	render();
}

void MonsterManager::render()
{
	glm::mat4 matIdentity = glm::mat4(1.0f);
	for (int i = 0; i < vecMonster.size(); i++)
	{
		if (isInSight(this->vecMonster[i]->PosRow, this->vecMonster[i]->PosCol))
		{
			const float TILESIZE = 10.0f;
			const float offset = -5.0f;

			float cellXLocation = ((this->vecMonster[i]->PosCol - int(m_mazeManager->ViewColumnIndex)) * TILESIZE);
			float cellYLocation = ((this->vecMonster[i]->PosRow - int(m_mazeManager->ViewRowIndex)) * TILESIZE);

			this->vecMonster[i]->meshObj->position.x = cellXLocation + offset;
			this->vecMonster[i]->meshObj->position.y = 4;
			this->vecMonster[i]->meshObj->position.z = cellYLocation + offset;
			drawObj(this->vecMonster[i]->meshObj, matIdentity, pShaderManager, pVAOManager);
		}
	}
}

bool MonsterManager::isAvailable(int row, int col)
{
	if (m_mazeManager->getMazeAtPos(row, col))
	{
		return false;
	}

	for (int i = 0; i < vecMonster.size(); i++)
	{
		if (vecMonster[i]->PosCol == col && vecMonster[i]->PosRow == row)
		{
			return false;
		}
	}

	return true;
}

void MonsterManager::startThread()
{
	ResumeThread(threadID);
}

bool MonsterManager::isInSight(int inputRow, int inputCol)
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
		manager = NULL;
		bExitThread = false;
		bSuspendThread = false;
		// Pause for one frame at 60Hz (16 ms give or take)
		suspendTime_ms = 16;
	}
	MonsterManager* manager;

	// Setting to true exits thread
	bool bExitThread;
	// Setting to true will stop the update
	bool bSuspendThread;
	unsigned int suspendTime_ms;
};

DWORD WINAPI monsterThread(LPVOID pVOIDMonster)
{
	std::cout << "Starting the beholder manager thread" << std::endl;
	sMonsterThreadData* pMonster = (sMonsterThreadData*)(pVOIDMonster);
	std::chrono::steady_clock::time_point CurrentUpdate = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point lastUpdate = CurrentUpdate;
	std::chrono::duration<double> elapsed;

	while (!pMonster->bExitThread)
	{
		if (!pMonster->bSuspendThread)
		{
			CurrentUpdate = std::chrono::steady_clock::now();
			elapsed = CurrentUpdate - lastUpdate;
			if (elapsed.count() >  0.333333)
			{
				lastUpdate = CurrentUpdate;
				for (int i = 0; i < pMonster->manager->vecMonster.size(); i++)
				{
					pMonster->manager->vecMonster[i]->update(elapsed.count());
				}
				//pMonster->bSuspendThread = true;
				Sleep(200);		// Release this thread if needed
			}
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

void MonsterManager::createThread()
{
	LPDWORD lpThreadId = NULL;
	HANDLE hThread = 0;
	sMonsterThreadData* threadData = new sMonsterThreadData();
	threadData->manager = this;
	hThread =
		CreateThread(NULL,				// Security attributes
			0,					// Use default stack size
			monsterThread,	// Address of the function we are going to call
			(void*)threadData,			// Please wait a moment
			CREATE_SUSPENDED,  // 0 or CREATE_SUSPENDED
			lpThreadId);
	threadID = hThread;
	Sleep(0);
}