#pragma once

#include <iVehicle.h>
#include <global.h>

#include "Conversion.h"
#include <bullet/BulletDynamics/Vehicle/btRaycastVehicle.h>
#include <bullet/BulletDynamics/Vehicle/btWheelInfo.h>
#include <bullet/BulletCollision/CollisionDispatch/btCollisionObject.h>


class Vehicle : public iVehicle
{
public:
	Vehicle(btRigidBody* chassis, btVehicleRaycaster* vehicleRaycaster);
	virtual ~Vehicle();

	virtual void AddWheel(const Vec3& connectionPoint, const Vec3& wheelDirection,
		const Vec3& wheelAxle, float suspensionRestLength, float wheelRadius,
		bool isFrontWheel) override;

	virtual void ApplyEngineForce(float force, int wheelIndex) override;
	virtual void SetSteering(float value, int wheelIndex) override;
	virtual void SetBrake(float value, int wheelIndex) override;

	virtual void GetChassisTransform(Matrix4x4& matrix) const override;
	virtual void GetWheelTransform(Matrix4x4& matrix, int wheelIndex) const override;

	btRaycastVehicle* GetBulletVehicle() {
		return m_Vehicle;
	}

private:
	Vehicle() { }

	btRaycastVehicle* m_Vehicle;

	Vehicle(const Vehicle& other) { }
	Vehicle& operator=(const Vehicle& other0) {
		return *this;
	}
};
