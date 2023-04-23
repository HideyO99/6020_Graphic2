#pragma once
#include "MazeGenerator/MazeManager.h"
#include "Vehicle.h"

#define NUMVEHICLE 1

class VehicleManager
{
public:
	VehicleManager();
	~VehicleManager();

	void init(MazeManager* maze, cVAOManager* vao, cMeshObj* mesh, cShaderManager* shader);
	void createVehicle(int id);
	void update(float elapsed);
	void render();
	bool isAvailable(int row, int col);

	void createThread();
	void startThread();
	cShaderManager* pShaderManager;
	cVAOManager* pVAOManager;
	cMeshObj* meshObj;
	MazeManager* m_mazeManager;

	std::vector<Vehicle*> vecVehicle;

private:

};
