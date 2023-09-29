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

// Fonctions

std::optional<BoundingBox> BBUnion(std::vector<BoundingBox>&& bbs)
{
	if (bbs.size() == 0)
	{
		return {};
	}

	Vector3D lb(bbs[0].m_lowerbound);
	Vector3D ub(bbs[0].m_upperbound);

	for (std::size_t i = 1; i < bbs.size(); i++)
	{
		lb = Vector3D(std::min(lb.getX(), bbs[i].m_lowerbound.getX()),
			std::min(lb.getY(), bbs[i].m_lowerbound.getY()),
			std::min(lb.getZ(), bbs[i].m_lowerbound.getZ()));

		ub = Vector3D(std::max(ub.getX(), bbs[i].m_upperbound.getX()),
			std::max(ub.getY(), bbs[i].m_upperbound.getY()),
			std::max(ub.getZ(), bbs[i].m_upperbound.getZ()));
	}

	return BoundingBox(lb, ub);
}