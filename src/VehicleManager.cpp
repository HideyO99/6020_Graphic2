#include "VehicleManager.h"

void drawObj(cMeshObj* pCurrentMeshObject, glm::mat4x4 mat_PARENT_Model, cShaderManager* pShaderManager, cVAOManager* pVAOManager);

VehicleManager::VehicleManager()
{
}

VehicleManager::~VehicleManager()
{
}

void VehicleManager::init(MazeManager* maze, cVAOManager* vao, cMeshObj* mesh, cShaderManager* shader)
{
	vecVehicle.clear();
	this->m_mazeManager = maze;
	this->pVAOManager = vao;
	this->meshObj = mesh;
	this->pShaderManager = shader;

	for (int i = 0; i < NUMVEHICLE; i++)
	{
		createVehicle(i);
	}
}

void VehicleManager::createVehicle(int id)
{
	int row, col;
	do
	{
		row = rand() % (MAZESIZE - 1);
		col = rand() % (MAZESIZE - 1);

	} while (!isAvailable(row, col));

	Vehicle* pVehicle = new Vehicle();
	pVehicle->id = id;
	pVehicle->PosRow = row;
	pVehicle->PosCol = col;
	pVehicle->m_VehicleGameObject = new cMeshObj();
	pVehicle->m_VehicleGameObject->bDoNotLight = true;
	pVehicle->m_VehicleGameObject->bUse_RGBA_colour = false;
	pVehicle->m_VehicleGameObject->meshName = "Vehicle";
	pVehicle->m_VehicleGameObject->instanceName = std::to_string(id);

	pVehicle->m_VehicleGameObject->textures[0] = this->meshObj->textures[0];
	pVehicle->m_VehicleGameObject->textureRatios[0] = this->meshObj->textureRatios[0];

	vecVehicle.push_back(pVehicle);
}

void VehicleManager::update(float elapsed)
{
	render();
}

void VehicleManager::render()
{
	glm::mat4 matIdentity = glm::mat4(1.0f);
	for (int i = 0; i < vecVehicle.size(); i++)
	{
		drawObj(this->vecVehicle[i]->m_VehicleGameObject, matIdentity, pShaderManager, pVAOManager);
	}
}

bool VehicleManager::isAvailable(int row, int col)
{
	if (m_mazeManager->getMazeAtPos(row, col))
	{
		return false;
	}

	for (int i = 0; i < vecVehicle.size(); i++)
	{
		if (vecVehicle[i]->PosCol == col && vecVehicle[i]->PosRow == row)
		{
			return false;
		}
	}

	return true;
}
