#include "Sphere.hpp"

// Constructor
Sphere::Sphere(Vector3D center, double radius)
	: m_center(center), m_radius(radius), m_boundingbox(m_center - m_radius * Vector3D(1, 1, 1), m_center + m_radius * Vector3D(1, 1, 1))
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

BoundingBox Sphere::getBoundingBox() const
{
	return m_boundingbox;
}

void Sphere::setBoundingBox(const BoundingBox& bb)
{
	m_boundingbox = BoundingBox(bb);
}

// Methods
Vector3D Sphere::normal(Vector3D p) const
{
	Vector3D n = p - m_center;
	return n/n.norme();
}