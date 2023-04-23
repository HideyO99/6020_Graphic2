#pragma once
#include <vector>
#include "MeshObj/cMeshObj.h"
#include "../PhysicInterface/src/iVehicle.h"
#include "../PhysicInterface/src/iCharacterCTRL.h"

class Vehicle
{
public:
	Vehicle();
	~Vehicle();
	int id;
	int PosRow;
	int PosCol;
	bool alive;
	unsigned int m_PlaneModelId;
	unsigned int m_ChassisModelId;
	unsigned int m_WheelModelId;

	unsigned int m_WoodTextureId;

	unsigned int m_GroundMaterialId;
	unsigned int m_ChassisMaterialId;
	unsigned int m_WheelMaterialId;

	float m_EngineForce;
	float m_Steering;
	float m_BrakeValue;

	glm::vec3 m_WalkDirection;
	iVehicle* m_Vehicle;
	iCharacterCTRL* m_CharacterController;
	cMeshObj* m_VehicleGameObject;
	std::vector<cMeshObj*> m_Wheels;
private:

};

Vehicle::Vehicle()
{
}

Vehicle::~Vehicle()
{
}