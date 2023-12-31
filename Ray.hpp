#pragma once

#include "phys.hpp"
#include "Sphere.hpp"

#include <optional>

class Ray
{
public:
	// Constructor
	Ray(Vector3D origin, Vector3D direction);

	// Accessors
	Vector3D getOrigin() const;
	void setOrigin(const Vector3D& new_origin);

	Vector3D getDirection() const;
	void setDirection(const Vector3D& new_direction);

	// Methods
	std::optional<double> hit(const Sphere& s) const;
	std::optional<double> hitBB(const BoundingBox& bb) const;

private:
	Vector3D m_origin;
	Vector3D m_direction;
};

void test_bb();