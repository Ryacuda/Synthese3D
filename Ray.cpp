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

std::optional<Vector3D> Ray::hitBB(const BoundingBox& bb) const
{
	Vector3D intersection(0, 0, 0);
	double dist = std::numeric_limits<double>::infinity();

	if (m_direction.getX())
	{
		Vector3D p_x = m_origin + ((bb.m_lowerbound.getX() - m_origin.getX()) / m_direction.getX()) * m_direction;

		if (p_x.getY() > bb.m_lowerbound.getY() && p_x.getY() < bb.m_upperbound.getY()
			&& p_x.getZ() > bb.m_lowerbound.getZ() && p_x.getZ() < bb.m_upperbound.getZ())
		{
			double x_dist = (p_x - m_origin).normeSQ();
			if (x_dist < dist)
			{
				intersection = p_x;
				dist = x_dist;
			}
		}

		p_x = m_origin + ((bb.m_upperbound.getX() - m_origin.getX()) / m_direction.getX()) * m_direction;

		if (p_x.getY() > bb.m_lowerbound.getY() && p_x.getY() < bb.m_upperbound.getY()
			&& p_x.getZ() > bb.m_lowerbound.getZ() && p_x.getZ() < bb.m_upperbound.getZ())
		{
			double x_dist = (p_x - m_origin).normeSQ();
			if (x_dist < dist)
			{
				intersection = p_x;
				dist = x_dist;
			}
		}
	}

	// y
	if (m_direction.getY())
	{
		Vector3D p_y = m_origin + ((bb.m_lowerbound.getY() - m_origin.getY()) / m_direction.getY()) * m_direction;

		if (p_y.getX() > bb.m_lowerbound.getX() && p_y.getX() < bb.m_upperbound.getX()
			&& p_y.getZ() > bb.m_lowerbound.getZ() && p_y.getZ() < bb.m_upperbound.getZ())
		{
			double y_dist = (p_y - m_origin).normeSQ();
			if (y_dist < dist)
			{
				intersection = p_y;
				dist = y_dist;
			}
		}

		p_y = m_origin + ((bb.m_upperbound.getY() - m_origin.getY()) / m_direction.getY()) * m_direction;

		if (p_y.getX() > bb.m_lowerbound.getX() && p_y.getX() < bb.m_upperbound.getX()
			&& p_y.getZ() > bb.m_lowerbound.getZ() && p_y.getZ() < bb.m_upperbound.getZ())
		{
			double y_dist = (p_y - m_origin).normeSQ();
			if (y_dist < dist)
			{
				intersection = p_y;
				dist = y_dist;
			}
		}
	}

	// z
	if (m_direction.getZ())
	{
		Vector3D p_z = m_origin + ((bb.m_lowerbound.getZ() - m_origin.getZ()) / m_direction.getZ()) * m_direction;

		if (p_z.getX() > bb.m_lowerbound.getX() && p_z.getX() < bb.m_upperbound.getX()
			&& p_z.getY() > bb.m_lowerbound.getY() && p_z.getY() < bb.m_upperbound.getY())
		{
			double z_dist = (p_z - m_origin).normeSQ();
			if (z_dist < dist)
			{
				intersection = p_z;
				dist = z_dist;
			}
		}

		p_z = m_origin + ((bb.m_upperbound.getZ() - m_origin.getZ()) / m_direction.getZ()) * m_direction;

		if (p_z.getX() > bb.m_lowerbound.getX() && p_z.getX() < bb.m_upperbound.getX()
			&& p_z.getY() > bb.m_lowerbound.getY() && p_z.getY() < bb.m_upperbound.getY())
		{
			double z_dist = (p_z - m_origin).normeSQ();
			if (z_dist < dist)
			{
				intersection = p_z;
				dist = z_dist;
			}
		}
	}

	return intersection;
}

// Functions

void test_bb()
{
	BoundingBox bb(Vector3D(-1, -1, -1), Vector3D(1,1,1));
	Ray r1(Vector3D(2, 0, 0), Vector3D(1, 0, 0));
	Ray r2(Vector3D(2, 0, 0), Vector3D(-1, 0, 0));

	std::optional<Vector3D> r1_hit = r1.hitBB(bb);
	std::optional<Vector3D> r2_hit = r2.hitBB(bb);

	if (r1_hit || !r2_hit)
	{
		std::cout << "TEST FAILED" << std::endl;
	}

	if (r1_hit.has_value())
	{
		std::cout << "r1 : " << (dotProduct(r1_hit.value() - r1.getOrigin(), r1.getDirection()) > 0) << " should be 0" << std::endl;
	}

	if (r1_hit.has_value())
	{
		std::cout << "r2 : " << (dotProduct(r2_hit.value() - r2.getOrigin(), r2.getDirection()) > 0) << " should be 1" << std::endl;
	}
}