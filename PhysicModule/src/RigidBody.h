#pragma once
#include <iRigidBody.h>
#include <iRigidBodyDesc.h>
#include <Physx/PxPhysics.h>
#include <Physx/PxPhysicsAPI.h>


class RigidBody : public iRigidBody
{
public:
	RigidBody(const iRigidBodyDesc& desc, iShape* shape);
	~RigidBody();

	static RigidBody* cast(iCollision* body);
	iShape* getShape();
	
	bool isStatic();
	virtual Vec3 getPosition() override;
	virtual void setPosition(const Vec3& pos) override;

	virtual Quat getRotation() override;
	virtual void setRotation(const Quat& rotate) override;

	virtual void addForce(const Vec3& force) override;
	virtual void addForceAP(const Vec3& force, const Vec3& at) override;
	
	virtual void addImpulse(const Vec3& impulse) override;
	virtual void addImpulseAP(const Vec3& impulse, const Vec3& at) override;

	virtual void addTorque(const Vec3& torque) override;
	virtual void addTorqueImpulse(const Vec3& torqueImpulse) override;

	virtual Vec3 getPositionFromPhysX() override;
	virtual Quat getRotationFromPhysX() override;


	void setGravity(const Vec3& gravity);
	void updateAcc();

	void verletStep1(float dt);
	void verletStep2(float dt);
	void verletStep3(float dt);


	void killForce();
	void addDamping(float dt);

	void setRenderPosition(Vec3* pos);
	void update(float step);

	friend class CollisionHandler;

	physx::PxShape* pShape = nullptr;
	physx::PxRigidActor* pActor = nullptr;

	float m_mass;
private:
	float m_inverseMass;
	float m_friction;
	float m_restitution;
	float m_linearDamping;
	float m_angularDamping;

	bool m_bStatic;

	Vec3 m_position;
	Vec3 m_prevPosition;
	Vec3 m_velocity;
	Vec3 m_acceleration;

	Vec3 m_force;
	Vec3 m_torque;
	Vec3 m_gravity;

	Quat m_rotation;
	Vec3 m_angularVelocity;
	Vec3 m_angularAcceleration;

	iShape* m_shape;

	Vec3* m_renderPosition;

	RigidBody(const RigidBody&) {}
	RigidBody& operator=(const RigidBody&)
	{
		return *this;
	}
};

