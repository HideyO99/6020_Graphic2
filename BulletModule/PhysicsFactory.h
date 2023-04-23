#pragma once

#include <iPhysicsFactory.h>
#include <bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <iPhysicsWorld.h>

class btVehicleRaycaster;

class PhysicsFactory : public iPhysicsFactory
{
public:
	PhysicsFactory();
	virtual ~PhysicsFactory();

	virtual iPhysicsWorld* CreateWorld() override;
	virtual iRigidBody* CreateRigidBody(const RigidBodyDesc& desc, iShape* shape) override;
	virtual iVehicle* CreateVehicle(iRigidBody* chassis) override;
	virtual iCharacterCTRL* CreateCharacterController(
		iConvexShape* shape, float stepHeight, const Vec3& up) override;

private:
	btVehicleRaycaster* m_VehicleRaycaster;
	btDiscreteDynamicsWorld* m_DynamicsWorld;
};
