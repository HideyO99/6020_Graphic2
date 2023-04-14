#pragma once
#include "MeshObj/cMeshObj.h"
#include "cBeholder.h"
#define NUMBEHOLDER 1000

class cBeholderManager
{
public:
	cBeholderManager();
	~cBeholderManager();

	void init(MazeManager* maze, cVAOManager* vao, cMeshObj* mesh, cShaderManager* shader);
	void createBeholder(int id);
	void render();
	bool isAvailable(int row, int col);
	void update();
	void oneThread();
	void startThread();

	cShaderManager* pShaderManager;
	cVAOManager* pVAOManager;
	cMeshObj* meshObj;
	MazeManager* m_mazeManager;
	std::vector<cBeholder*> vecBeholder;
private:
	bool isInSight(int inputRow,int inputCol);
	HANDLE threadID;
};

