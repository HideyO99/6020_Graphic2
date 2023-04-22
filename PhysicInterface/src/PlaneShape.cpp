#include "PlaneShape.h"

PlaneShape::PlaneShape(float dotProduct, const Vec3& normal)
	: iShape(ShapeType::Plane)
	, m_DotProduct(dotProduct)
	, m_Normal(normal)
{ }

PlaneShape::~PlaneShape()
{ }

const Vec3& PlaneShape::GetNormal() const
{
	return m_Normal;
}
float PlaneShape::GetDotProduct() const
{
	return m_DotProduct;
}

PlaneShape* PlaneShape::Cast(iShape* shape)
{
	return dynamic_cast<PlaneShape*>(shape);
}