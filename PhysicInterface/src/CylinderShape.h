#pragma once

#include "iShape.h"

class CylinderShape : public iConvexShape
{
public:
	CylinderShape(const Vec3& halfExtents);
	virtual ~CylinderShape();

	const Vec3& GetHalfExtents() const;

	static CylinderShape* Cast(iShape* shape);

protected:
	CylinderShape(ShapeType shapeType)
		: iConvexShape(shapeType) {}

private:
	Vec3 m_HalfExtents;
	CylinderShape(const CylinderShape&) : iConvexShape(ShapeType::Cylinder) {}
	CylinderShape& operator=(const CylinderShape&) {
		return *this;
	}
};

