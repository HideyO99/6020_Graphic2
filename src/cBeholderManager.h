#pragma once
#include "MeshObj/cMeshObj.h"
#include "cBeholder.h"
#define NUMBEHOLDER 100

class cBeholderManager
{
public:
	cBeholderManager();
	~cBeholderManager();

	void init(MazeManager* maze, cVAOManager* vao, cMeshObj* mesh, cShaderManager* shader);
	void createBeholder();
	void render();
	bool isAvailable(int row, int col);
	void update();

	cShaderManager* pShaderManager;
	cVAOManager* pVAOManager;
	cMeshObj* meshObj;
	MazeManager* m_mazeManager;
	std::vector<cBeholder*> vecBeholder;
};

