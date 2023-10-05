#pragma once

#include "phys.hpp"

class Material
{
public:
	// Constructor
	Material(const Vector3D& color);

	// Methods
	virtual Vector3D computeColor(const Vector3D& normal, const Vector3D& incident) = 0;

	Vector3D reflect(const Vector3D& normal, const Vector3D& incident);

	// Attributes
	Vector3D m_color;
};

class Diffuse : public Material
{
public:
	// Constructor
	Diffuse(const Vector3D& color);

	// Methods
	Vector3D computeColor(const Vector3D& normal, const Vector3D& incident);
};

class Glass : public Material
{
public:
	// Constructor
	Glass(const Vector3D& color, double eta);

	// Methods
	Vector3D computeColor(const Vector3D& normal, const Vector3D& incident);

	Vector3D refract(const Vector3D& normal, const Vector3D& incident);

private:
	double m_eta;
};

class Mirror : public Material
{
public:
	// Constructor
	Mirror(const Vector3D& color);

	// Methods
	Vector3D computeColor(const Vector3D& normal, const Vector3D& incident);
};