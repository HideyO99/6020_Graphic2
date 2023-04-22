#pragma once
#include "iCollisionBody.h"
#include "iShape.h"
#include <glm/gtx/quaternion.hpp>

class iRigidBody : public iCollisionBody
{
public:
	virtual ~iRigidBody() {}

	virtual void GetPosition(Vec3& position) = 0;
	virtual void SetPosition(const Vec3& position) = 0;

	virtual void GetRotation(glm::quat& rotation) = 0;
	virtual void SetRotation(const glm::quat& rotation) = 0;

	virtual void ApplyForce(const Vec3& force) = 0;
	virtual void ApplyForceAtPoint(const Vec3& force, const Vec3& relativePoint) = 0;

	virtual void ApplyImpulse(const Vec3& impulse) = 0;
	virtual void ApplyImpulseAtPoint(const Vec3& impulse, const Vec3& relativePoint) = 0;

	virtual void ApplyTorque(const Vec3& torque) = 0;
	virtual void ApplyTorqueImpulse(const Vec3& torqueImpulse) = 0;

	virtual void GetTransform(glm::mat4& transform) = 0;

protected:
	iRigidBody() : iCollisionBody(BodyType::RigidBody) {}

private:
	iRigidBody(const iRigidBody&) : iCollisionBody(BodyType::RigidBody) { }
	iRigidBody& operator=(const iRigidBody&) {
		return *this;
	}
};

