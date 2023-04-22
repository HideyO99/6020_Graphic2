#pragma once

#include <iRigidBody.h>
#include <RigidBodyDesc.h>
#include <iCollisionBody.h>
#include <glm/gtx/quaternion.hpp>

#include <bullet/btBulletDynamicsCommon.h>

class RigidBody : public iRigidBody
{
public:
	RigidBody(const RigidBodyDesc& desc, iShape* shape);
	~RigidBody();

	static RigidBody* Cast(iCollisionBody* body);

	virtual void GetPosition(Vec3& position) override;
	virtual void SetPosition(const Vec3& position) override;

	virtual void GetRotation(glm::quat& rotation) override;
	virtual void SetRotation(const glm::quat& rotation) override;

	virtual void ApplyForce(const Vec3& force) override;
	virtual void ApplyImpulse(const Vec3& impulse) override;

	virtual void ApplyForceAtPoint(const Vec3& force, const Vec3& relativePoint) override;
	virtual void ApplyImpulseAtPoint(const Vec3& impulse, const Vec3& relativePoint) override;

	virtual void ApplyTorque(const Vec3& torque) override;
	virtual void ApplyTorqueImpulse(const Vec3& torqueImpulse) override;

	virtual void GetTransform(glm::mat4& transform) override;

	btRigidBody* GetBulletBody(void) { return m_BulletRigidBody; }
private:

	btRigidBody* m_BulletRigidBody;

	RigidBody(const RigidBody&) { }
	RigidBody& operator=(const RigidBody&) {
		return *this;
	}
};
