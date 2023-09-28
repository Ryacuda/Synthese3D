#pragma once

#include <iostream>
#include <cmath>
#include "Magick++.h"

class Vector3D
{
public:
	// Constructors
	Vector3D(double x, double y, double z);
	Vector3D(const Vector3D& v);

	// Methods
	double normeSQ() const;
	double norme() const;

	// Operators
	Vector3D& operator+=(const Vector3D& rhs);
	friend Vector3D operator+(Vector3D lhs, const Vector3D& rhs);

	Vector3D& operator-=(const Vector3D& rhs);
	friend Vector3D operator-(Vector3D lhs, const Vector3D& rhs);

	Vector3D& operator*=(const double s);
	friend Vector3D operator*(Vector3D lhs, const double s);
	friend Vector3D operator*(const double s, Vector3D rhs);

	Vector3D& operator/=(const double s);
	friend Vector3D operator/(Vector3D lhs, const double s);

	friend std::ostream& operator<<(std::ostream& os, const Vector3D& rhs);

	double getX() const;
	double getY() const;
	double getZ() const;

	//Friend Fonctions
	friend double dotProduct(const Vector3D& v1, const Vector3D& v2);

private:
	// Members
	double m_x;
	double m_y;
	double m_z;
};

Vector3D frottements(const Vector3D& vit, double Cd, double area);

Vector3D gravite(const Vector3D& dir, double masse, double grav_const);

void simu_chute();