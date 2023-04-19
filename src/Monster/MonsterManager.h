#pragma once
#include <Windows.h>
#include <assert.h>
#include <vector>
#include "Monster.h"

class MonsterManager
{
public:
	MonsterManager();
	~MonsterManager();

	void init();
	void createMonster(int id);
	void update();
	void render();

	void oneThread();
	void startThread();
	Character* prototypeCharacter;

	std::vector<Monster*> vecMonster;
private:
	bool isInSight(int inputRow, int inputCol);
	//HANDLE threadID;
};

