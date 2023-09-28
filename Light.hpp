#pragma once 

#include "phys.hpp"

class Light
{
public:
	// Constructor
	Light(Vector3D position , double intensity);

	// Accessors
	double getIntensity() const;
	void setIntensity(double new_intensity);

	Vector3D getPosition() const;
	void setPosition(const Vector3D& new_position);

private:
	double m_intensity;

	Vector3D m_position;
};