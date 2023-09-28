#include "Ray.hpp"


// Constructor
Ray::Ray(Vector3D origin, Vector3D direction)
	: m_origin(origin), m_direction(direction)
{

}


// Accessors
Vector3D Ray::getOrigin() const
{
	return m_origin;
}

void Ray::setOrigin(const Vector3D& new_origin)
{
	m_origin = new_origin;
}

Vector3D Ray::getDirection() const
{
	return m_direction;
}

void Ray::setDirection(const Vector3D& new_direction)
{
	m_direction = new_direction;
}

// Methods
std::optional<Vector3D> Ray::hit(const Sphere& s) const
{
	double t = -std::numeric_limits<double>::infinity();

	double a = m_direction.normeSQ();
	double b = 2 * dotProduct(m_direction, m_origin - s.getCenter());
	double c = (m_origin - s.getCenter()).normeSQ() - (s.getRadius() * s.getRadius());

	double delta = (b * b) - (4 * a * c);

	if (delta == 0)
	{
		t =  -b / (2 * a);
	}
	else if (delta > 0)
	{
		double t1 = (-b - sqrt(delta)) / (2 * a);
		double t2 = (-b + sqrt(delta)) / (2 * a);

		if (t1 >= 0)
		{
			if (t2 >= 0)
			{
				t = std::min(t1, t2);
			}
			else
			{
				t = t1;
			}
		}
		else
		{
			t = t2;
		}
	}

	// t may be < 0
	if (t > 0)
	{
		return t * m_direction + m_origin;
	}
	else
	{
		return {};
	}
}