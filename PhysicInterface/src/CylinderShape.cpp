#include "CylinderShape.h"

CylinderShape::CylinderShape(const Vec3& halfExtents)
	: iConvexShape(ShapeType::Cylinder)
	, m_HalfExtents(halfExtents)
{ }

CylinderShape::~CylinderShape()
{ }

const Vec3& CylinderShape::GetHalfExtents() const
{
	return m_HalfExtents;
}

CylinderShape* CylinderShape::Cast(iShape* shape)
{
	return dynamic_cast<CylinderShape*>(shape);
}
