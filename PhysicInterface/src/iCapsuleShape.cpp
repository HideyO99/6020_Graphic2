#include "iCapsuleShape.h"

iCapsuleShape::iCapsuleShape(float radius, float height)
	: iConvexShape(shapeType::Capsule)
	, m_Radius(radius)
	, m_Height(height)
{
}

iCapsuleShape::~iCapsuleShape()
{
}

float iCapsuleShape::GetRadius() const
{
	return m_Radius;
}

float iCapsuleShape::GetHeight() const
{
	return m_Height;
}

iCapsuleShape* iCapsuleShape::Cast(iShape* shape)
{
	return dynamic_cast<iCapsuleShape*>(shape);
}
