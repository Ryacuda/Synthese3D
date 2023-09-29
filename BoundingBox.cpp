#include "BoundingBox.hpp"

// Constructors
BoundingBox::BoundingBox(const Vector3D& lb, const Vector3D& ub)
	: m_lowerbound(lb), m_upperbound(ub)
{
	
}

// Methods
bool BoundingBox::inside(const Vector3D& p)
{
	return p.getX() > m_lowerbound.getX() && p.getX() < m_upperbound.getX()
		&& p.getY() > m_lowerbound.getY() && p.getY() < m_upperbound.getY()
		&& p.getZ() > m_lowerbound.getZ() && p.getZ() < m_upperbound.getZ();
}