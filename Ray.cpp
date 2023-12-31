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
std::optional<double> Ray::hit(const Sphere& s) const
{
	std::optional<double> t = {};

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

	return t;
}

std::optional<double> Ray::hitBB(const BoundingBox& bb) const
{
	double dist_min = std::numeric_limits<double>::infinity();

	if (m_direction.getX())
	{
		Vector3D p_x = m_origin + ((bb.m_lowerbound.getX() - m_origin.getX()) / m_direction.getX()) * m_direction;

		if (p_x.getY() > bb.m_lowerbound.getY() && p_x.getY() < bb.m_upperbound.getY()
			&& p_x.getZ() > bb.m_lowerbound.getZ() && p_x.getZ() < bb.m_upperbound.getZ())
		{
			double x_dist = (p_x - m_origin).norme();
			if (x_dist < dist_min)
			{
				dist_min = x_dist;
			}
		}

		p_x = m_origin + ((bb.m_upperbound.getX() - m_origin.getX()) / m_direction.getX()) * m_direction;

		if (p_x.getY() > bb.m_lowerbound.getY() && p_x.getY() < bb.m_upperbound.getY()
			&& p_x.getZ() > bb.m_lowerbound.getZ() && p_x.getZ() < bb.m_upperbound.getZ())
		{
			double x_dist = (p_x - m_origin).norme();
			if (x_dist < dist_min)
			{
				dist_min = x_dist;
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
			if (y_dist < dist_min)
			{
				dist_min = y_dist;
			}
		}

		p_y = m_origin + ((bb.m_upperbound.getY() - m_origin.getY()) / m_direction.getY()) * m_direction;

		if (p_y.getX() > bb.m_lowerbound.getX() && p_y.getX() < bb.m_upperbound.getX()
			&& p_y.getZ() > bb.m_lowerbound.getZ() && p_y.getZ() < bb.m_upperbound.getZ())
		{
			double y_dist = (p_y - m_origin).normeSQ();
			if (y_dist < dist_min)
			{
				dist_min = y_dist;
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
			if (z_dist < dist_min)
			{
				dist_min = z_dist;
			}
		}

		p_z = m_origin + ((bb.m_upperbound.getZ() - m_origin.getZ()) / m_direction.getZ()) * m_direction;

		if (p_z.getX() > bb.m_lowerbound.getX() && p_z.getX() < bb.m_upperbound.getX()
			&& p_z.getY() > bb.m_lowerbound.getY() && p_z.getY() < bb.m_upperbound.getY())
		{
			double z_dist = (p_z - m_origin).normeSQ();
			if (z_dist < dist_min)
			{
				dist_min = z_dist;
			}
		}
	}

	if (dist_min < std::numeric_limits<double>::infinity())
	{
		return dist_min;
	}
	else
	{
		return {};
	}
}

// Functions

void test_bb()
{
	BoundingBox bb(Vector3D(-1, -1, -1), Vector3D(1,1,1));
	Ray r1(Vector3D(2, 0, 0), Vector3D(1, 0, 0));
	Ray r2(Vector3D(2, 0, 0), Vector3D(-1, 0, 0));

	std::optional<double> r1_t = r1.hitBB(bb);
	std::optional<double> r2_t = r2.hitBB(bb);

	if (r1_t.has_value())
	{
		std::cout << "r1 : " << (r1_t.value() > 0) << " should be 0" << std::endl;
	}

	if (r2_t.has_value())
	{
		std::cout << "r2 : " << (r2_t.value() > 0) << " should be 1" << std::endl;
	}
}