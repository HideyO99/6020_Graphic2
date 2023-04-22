#pragma once
#include <iPhysicWorld.h>
#include <Physx/PxPhysics.h>
#include <Physx/PxPhysicsAPI.h>

#include <vector>
#include "RigidBody.h"
#include "CollisionHandler.h"
#include <map>
#include "CharacterCTRL.h"

class PhysicWorld : public iPhysicWorld
{
public:
	PhysicWorld();
	virtual ~PhysicWorld();

	virtual void setGravity(const Vec3& gravity) override;
	virtual void addBody(iCollision* body) override;
	virtual void removeBody(iCollision* body) override;

	virtual void AddCharacterController(iCharacterCTRL* characterController) override;
	virtual void RemoveCharacterController(iCharacterCTRL* characterController) override;

	virtual void timeStep(float dt) override;
	virtual void addToCollisionListener(iCollisionListener* listener) override;
	virtual void resetWorld() override;

	//static physx::PxMaterial getMaterail();
	//static physx::PxControllerManager getControllerManager();
	static void addControllerToMap(iCharacterCTRL* charCon, physx::PxController* phyCon);

	static physx::PxMaterial* m_Material;
	static physx::PxControllerManager* m_ControllerManager;

private:
	//PhysX
	physx::PxDefaultAllocator m_DefaultAllocatorCallback;
	physx::PxDefaultErrorCallback m_DefaultErrorCallback;
	physx::PxDefaultCpuDispatcher* m_Dispatcher = nullptr;
	physx::PxTolerancesScale m_ToleranceScale;
	physx::PxFoundation* m_Foundation = nullptr;
	physx::PxPhysics* m_Physics = nullptr;
	physx::PxScene* m_Scene = nullptr;
	//physx::PxMaterial* m_Material = nullptr;
	physx::PxPvd* m_Pvd = nullptr;
	physx::PxPvdSceneClient* m_PVDClient = nullptr;
	static std::map<iCharacterCTRL*, physx::PxController*> m_MapControllers;

	std::vector<RigidBody*> m_actor;
	std::vector<CharacterCTRL*> m_CharacterControllers;
	std::map<physx::PxRigidActor*, physx::PxTransform> m_mapTransform;

	CollisionHandler* m_collisionHandler;
	iCollisionListener* m_collisionListener;

	void addRigid(iCollision* body);
	void removeRigid(iCollision* body);

	PhysicWorld(const PhysicWorld&) {}
	PhysicWorld& operator=(const PhysicWorld&) 
	{
		return *this;
	}
};

