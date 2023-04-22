#pragma once
#include "iPhysicWorld.h"
#include "iRigidBody.h"
#include "iRigidBodyDesc.h"
#include "iCharacterCTRL.h"

class iPhysicFactory
{
public:
	virtual ~iPhysicFactory() {}

	virtual iPhysicWorld* createWorld() = 0;
	virtual iRigidBody* createRigidBody(const iRigidBodyDesc& desc, iShape* shape) = 0;
	virtual iCharacterCTRL* CreateCharacterController(iConvexShape* shape, float stepHeight, const Vec3& up) = 0;

private:
	iPhysicFactory(const iPhysicFactory&) {}
	iPhysicFactory& operator=(const iPhysicFactory&)
	{
		return *this;
	}

protected:
	iPhysicFactory() {}
};

