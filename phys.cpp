#include "phys.hpp"

// Constructors
Vector3D::Vector3D(double x, double y, double z)
	: m_x(x), m_y(y), m_z(z)
{
}

Vector3D::Vector3D(const Vector3D& v)
	: m_x(v.m_x), m_y(v.m_y), m_z(v.m_z)
{

}

// Methods
double Vector3D::normeSQ() const
{
	return m_x * m_x + m_y * m_y + m_z * m_z;
}

double Vector3D::norme() const
{
	return sqrt(normeSQ());
}

// Operators
Vector3D& Vector3D::operator+=(const Vector3D& rhs)
{
	m_x += rhs.m_x;
	m_y += rhs.m_y;
	m_z += rhs.m_z;

	return *this;
}

Vector3D operator+(Vector3D lhs, const Vector3D& rhs)
{
	lhs += rhs;
	return lhs;
}

Vector3D& Vector3D::operator-=(const Vector3D& rhs)
{
	m_x -= rhs.m_x;
	m_y -= rhs.m_y;
	m_z -= rhs.m_z;

	return *this;
}

Vector3D operator-(Vector3D lhs, const Vector3D& rhs)
{
	lhs -= rhs;

	return lhs;
}

Vector3D& Vector3D::operator*=(const double s)
{
	m_x *= s;
	m_y *= s;
	m_z *= s;

	return *this;
}

Vector3D operator*(Vector3D lhs, const double s)
{
	lhs *= s;
	return lhs;
}

Vector3D operator*(const double s, Vector3D rhs)
{
	rhs *= s;
	return rhs;
}

Vector3D& Vector3D::operator/=(const double s)
{
	m_x /= s;
	m_y /= s;
	m_z /= s;

	return *this;
}

Vector3D operator/(Vector3D lhs, const double s)
{
	lhs /= s;
	return lhs;
}

std::ostream& operator<<(std::ostream& os, const Vector3D& rhs)
{
	os << rhs.m_x << ", " << rhs.m_y << ", " << rhs.m_z;
	return os;
}

double Vector3D::getX() const
{
	return m_x;
}

double Vector3D::getY() const
{
	return m_y;
}

double Vector3D::getZ() const
{
	return m_z;
}

// Fonctions

double dotProduct(const Vector3D& v1, const Vector3D& v2)
{
	return v1.m_x * v2.m_x + v1.m_y * v2.m_y + v1.m_z * v2.m_z;
}

Vector3D frottements(const Vector3D& vit, double Cd, double area)
{
	return Cd * area * vit * vit.norme();
}

Vector3D gravite(const Vector3D& dir, double masse, double grav_const)
{
	return dir * grav_const * masse;
}

void simu_chute()
{
	// parachutiste
	const double masse = 90;		// kg
	const double area = 1;		// m²

	// air
	const double Cd = 0.3;

	const double g = 9.81;

	Vector3D position_0(0, 0, 4000);
	Vector3D vitesse_0(50, 0, 0);
	Vector3D acceleration_0(0, 0, -g);

	double t = 0;
	const double tmax = 60;
	const double dt = 0.01;

	Vector3D position(position_0);
	Vector3D vitesse(vitesse_0);
	Vector3D acceleration(acceleration_0);

	while (t < tmax)
	{
		//Vector3D f = frottements(vitesse, Cd, area, masse);
		//std::cout << f << std::endl;

		acceleration = gravite(Vector3D(0, 0, -1), masse, g) + frottements(vitesse, Cd, area);
		acceleration /= masse;
		vitesse += acceleration * dt;
		position += vitesse * dt;

		t += dt;
	}

	std::cout << "Vitesse finale : " << vitesse.norme() << std::endl;
	std::cout << position << std::endl;
}