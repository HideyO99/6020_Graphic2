#pragma once
#include "iShape.h"

class iCapsuleShape : public iConvexShape
{
public:
	iCapsuleShape(float radius, float height);
	virtual ~iCapsuleShape();

	float GetRadius() const;
	float GetHeight() const;

	static iCapsuleShape* Cast(iShape* shape);

protected:
	iCapsuleShape(shapeType shapeType)
		: iConvexShape(shapeType) {}

private:
	float m_Radius;
	float m_Height;

	iCapsuleShape(const iCapsuleShape&) : iConvexShape(shapeType::Capsule) {}
	iCapsuleShape& operator=(const iCapsuleShape&) {
		return *this;
	}
};

