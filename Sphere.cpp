#include "Sphere.hpp"

// Constructor
Sphere::Sphere(Vector3D center, double radius)
	: m_center(center), m_radius(radius)
{

}

// Accessors
Vector3D Sphere::getCenter() const
{
	return m_center;
}

void Sphere::setCenter(const Vector3D& new_center)
{
	m_center = new_center;
}

double Sphere::getRadius() const
{
	return m_radius;
}

void Sphere::setRadius(double new_radius)
{
	m_radius = new_radius;
}

// Methods
Vector3D Sphere::normal(Vector3D p) const
{
	Vector3D n = p - m_center;
	return n/n.norme();
}