#pragma once
#include <Windows.h>
#include <assert.h>
#include <vector>
#include "Monster.h"
#include "../VAOManager/cVAOManager.h"
#include "../Shader/cShaderManager.h"
#include "../MazeGenerator/MazeManager.h"
#define NUMMONSTER 2
class MonsterManager
{
public:
	MonsterManager();
	~MonsterManager();

	void init(MazeManager* maze, cVAOManager* vao, cMeshObj* mesh, cShaderManager* shader);
	void createMonster(int id);
	void update(float elapsed);
	void render();
	bool isAvailable(int row, int col);

	void createThread();
	void startThread();
	Character* prototypeCharacter;
	cShaderManager* pShaderManager;
	cVAOManager* pVAOManager;
	cMeshObj* meshObj;
	MazeManager* m_mazeManager;

	std::vector<Monster*> vecMonster;
private:
	bool isInSight(int inputRow, int inputCol);
	HANDLE threadID;
};

