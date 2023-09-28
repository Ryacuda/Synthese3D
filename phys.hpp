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

	template <int axis>
	friend bool compare(const Vector3D& p1, const Vector3D& p2)
	{
		bool res = false;
		switch (axis)
		{
		case 0:
			res = p1.m_x < p2.m_x;
			break;
		case 1:
			res = p1.m_y < p2.m_y;
			break;
		case 2:
			res = p1.m_z < p2.m_z;
			break;
		default:
			break;
		}

		return res;
	}

private:
	// Members
	double m_x;
	double m_y;
	double m_z;
};

Vector3D frottements(const Vector3D& vit, double Cd, double area);

Vector3D gravite(const Vector3D& dir, double masse, double grav_const);

void simu_chute();