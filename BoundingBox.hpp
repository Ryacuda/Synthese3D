#pragma once

#include <optional>

#include "phys.hpp"

class BoundingBox
{
public:
	// Constructors
	BoundingBox(const Vector3D& lb, const Vector3D& ub);

	// Methods
	bool inside(const Vector3D& p);

	// Attributes
	Vector3D m_lowerbound;
	Vector3D m_upperbound;
};

std::optional<BoundingBox> BBUnion(std::vector<BoundingBox>&& bbs);