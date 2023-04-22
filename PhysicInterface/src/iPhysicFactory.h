#pragma once
#include "iPhysicsWorld.h"
#include "iRigidBody.h"
#include "RigidBodyDesc.h"
#include "iCharacterCTRL.h"
#include "iVehicle.h"
#include "global.h"

class iPhysicsFactory
{
public:
	virtual ~iPhysicsFactory() {}

	virtual iPhysicsWorld* CreateWorld() = 0;
	virtual iRigidBody* CreateRigidBody(const RigidBodyDesc& desc, iShape* shape) = 0;
	virtual iVehicle* CreateVehicle(iRigidBody* chassis) = 0;
	virtual iCharacterCTRL* CreateCharacterController(
		iConvexShape* shape, float stepHeight, const Vec3& up) = 0;

protected:
	iPhysicsFactory() {}

private:
	iPhysicsFactory(const iPhysicsFactory&) {}
	iPhysicsFactory& operator=(const iPhysicsFactory&) {
		return *this;
	}
};

