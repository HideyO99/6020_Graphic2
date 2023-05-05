#pragma once
#include <iPhysicsFactory.h>
#include <iPhysicsWorld.h>

#include "CollisionListener.h"

#pragma comment(lib, "PhysicModule.lib")
#include <PhysicsFactory.h>
#include <vector>

class Physic
{
public:
	Physic();
	~Physic();

	void init();
	void destroy();
	void update(float dt);
	void UserForce(glm::vec3 dir);
	void setActive(int num);

	//private:
	void createPlane();
	void createBall(cMeshObj* mOBJ, glm::vec3 pos, float size, bool isStatic = false);
	void createBox(cMeshObj* mOBJ, glm::vec3 pos, glm::vec3 size, bool isStatic = false);
	void createFloor(cMeshObj* mGround);
	void createCylinder(cMeshObj* mOBJ, glm::vec3 pos, glm::vec3 size, bool isStatic = false);

	CollisionListener* collisionListener;
	iPhysicsFactory* Factory;
	iPhysicsWorld* World;
	std::vector<cObject*> m_ballList;
	std::vector<cObject*> m_ObjList;
};