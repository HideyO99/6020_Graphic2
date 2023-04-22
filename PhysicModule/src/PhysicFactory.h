#pragma once
#include <iPhysicFactory.h>
#include "PhysicWorld.h"

class PhysicFactory :    public iPhysicFactory
{
public:
	PhysicFactory();
	virtual ~PhysicFactory();

	virtual iPhysicWorld* createWorld() override;
	virtual iRigidBody* createRigidBody(const iRigidBodyDesc& desc, iShape* shape) override;
	virtual iCharacterCTRL* CreateCharacterController(iShape* shape, Vec3& pos, const Quat& rotate) override;

//private:
//	iPhysicWorld* phyworld;
//	iCharacterCTRL* charCTRL;
};

