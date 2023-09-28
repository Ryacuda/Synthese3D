#pragma once

#include "phys.hpp"

class Sphere
{
public:
	// Constructor
	Sphere(Vector3D center, double radius);

	// Accessors
	Vector3D getCenter() const;
	void setCenter(const Vector3D& new_center);
	
	double getRadius() const;
	void setRadius(double new_radius);

	// Methods
	Vector3D normal(Vector3D p) const;

private:
	Vector3D m_center;
	double m_radius;
};