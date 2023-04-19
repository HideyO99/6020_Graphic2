#pragma once
#include <Windows.h>
#include <assert.h>
#include <vector>
#include "Monster.h"
#include "../VAOManager/cVAOManager.h"
#include "../Shader/cShaderManager.h"
#define NUMMONSTER 2
class MonsterManager
{
public:
	MonsterManager();
	~MonsterManager();

	void init(cVAOManager* vao, cMeshObj* mesh, cShaderManager* shader);
	void createMonster(int id);
	void update();
	void render();

	void createThread();
	void startThread();
	Character* prototypeCharacter;
	cShaderManager* pShaderManager;
	cVAOManager* pVAOManager;
	cMeshObj* meshObj;

	std::vector<Monster*> vecMonster;
private:
	bool isInSight(int inputRow, int inputCol);
	HANDLE threadID;
};

