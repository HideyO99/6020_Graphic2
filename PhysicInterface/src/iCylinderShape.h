#pragma once
#include "iShape.h"

class iCylinderShape : public iConvexShape
{
public:
	iCylinderShape(const Vec3& halfExtent);
	virtual ~iCylinderShape();

	const Vec3& getHalfExtents() const;

	static iCylinderShape* cast(iShape* shape);

protected:
	iCylinderShape(shapeType type) : iConvexShape(type) {}

private:
	Vec3 m_halfExtent;

	iCylinderShape(const iCylinderShape&) : iConvexShape(shapeType::Cylinder) {}

	iCylinderShape& operator=(const iCylinderShape&)
	{
		return *this;
	}
};

