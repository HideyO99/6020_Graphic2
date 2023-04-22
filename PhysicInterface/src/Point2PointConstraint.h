#pragma once

#include "iConstraint.h"
#include "iRigidBody.h"
#include "global.h"

class Point2PointConstraint : public iConstraint
{
public:
	Point2PointConstraint(iRigidBody* rbA, iRigidBody* rbB, Vec3 pointA, Vec3 pointB);
	Point2PointConstraint(iRigidBody* rbA, Vec3 pointA);

	virtual ~Point2PointConstraint();

	static Point2PointConstraint* Cast(iConstraint* constraint);

	iRigidBody* GetRididBodyA() const { return m_RigidBodyA; }
	iRigidBody* GetRididBodyB() const { return m_RigidBodyB; }
	const Vec3& GetPointA() const { return m_PointA; }
	const Vec3& GetPointB() const { return m_PointB; }

protected:
	Point2PointConstraint() = delete;

private:
	iRigidBody* m_RigidBodyA;
	iRigidBody* m_RigidBodyB;
	Vec3 m_PointA;
	Vec3 m_PointB;

	Point2PointConstraint(const Point2PointConstraint&) : iConstraint(ConstraintType::Point2Point) {}
	Point2PointConstraint& operator=(const Point2PointConstraint&) {
		return *this;
	}

};
