#include "Light.hpp"

// Constructor
Light::Light(Vector3D position, double intensity)
	: m_position(position), m_intensity(intensity)
{

}

// Accessors
double Light::getIntensity() const
{
	return m_intensity;
}

void Light::setIntensity(double new_intensity)
{
	m_intensity = new_intensity;
}

Vector3D Light::getPosition() const
{
	return m_position;
}

void Light::setPosition(const Vector3D& new_position)
{
	m_position = new_position;
}