#include "Tree.hpp"

// Constructors
Tree::Tree(std::unique_ptr<Tree> left, std::unique_ptr<Tree> right, Vector3D key, int axis)
	: m_left_tree(std::move(left)), m_right_tree(std::move(right)), m_key(key), m_axis(axis)
{

}

// Accessors

// Methods
std::unique_ptr<Tree> Tree::makeTree(std::vector<Vector3D>& keys)
{
	return makeTree(keys, 0);
}

Vector3D Tree::findClosestPoint(const Vector3D& p)
{
	return findClosestPoint(p, m_key, (p - m_key).normeSQ());
}

// Private methods
std::unique_ptr<Tree> Tree::makeTree(std::vector<Vector3D> keys, int axis)
{
	if (keys.size() == 0)
	{
		return nullptr;
	}

	std::sort(keys.begin(), keys.end(), [&axis](const Vector3D& v1, const Vector3D& v2)
		{	// sorting along a different axis on each recursive call
			if (axis == 0)
			{
				return v1.getX() < v2.getX();
			}
			else if (axis == 1)
			{
				return v1.getY() < v2.getY();
			}
			else
			{
				return v1.getZ() < v2.getZ();
			}
		}
	);

	std::size_t middle = keys.size() / 2;

	std::unique_ptr<Tree> left_tree = makeTree(std::vector<Vector3D>(keys.begin(), keys.begin() + middle), (axis + 1) % 3);
	std::unique_ptr<Tree> right_tree = makeTree(std::vector<Vector3D>(keys.begin() + middle + 1, keys.end()), (axis + 1) % 3);

	return std::make_unique<Tree>(std::move(left_tree), std::move(right_tree), keys[middle], axis);
}

Vector3D Tree::findClosestPoint(const Vector3D& p, Vector3D closest_p, double dist_to_closest)
{
	double dist = (p - m_key).normeSQ();
	if (dist < dist_to_closest)
	{
		dist_to_closest = dist;
		closest_p = m_key;
	}

	if (m_axis == 0)
	{
		if (p.getX() < m_key.getX() && m_left_tree)
		{
			return m_left_tree->findClosestPoint(p, closest_p, dist_to_closest);
		}
		else if (m_right_tree)
		{
			return m_right_tree->findClosestPoint(p, closest_p, dist_to_closest);
		}
	}
	else if (m_axis == 1)
	{
		if (p.getY() < m_key.getY() && m_left_tree)
		{
			return m_left_tree->findClosestPoint(p, closest_p, dist_to_closest);
		}
		else if (m_right_tree)
		{
			return m_right_tree->findClosestPoint(p, closest_p, dist_to_closest);
		}
	}
	else
	{
		if (p.getZ() < m_key.getZ() && m_left_tree)
		{
			return m_left_tree->findClosestPoint(p, closest_p, dist_to_closest);
		}
		else if (m_right_tree)
		{
			return m_right_tree->findClosestPoint(p, closest_p, dist_to_closest);
		}
	}

	return closest_p;
}

// Functions

void test_tree()
{
	const int n = 1000000;
	std::vector<Vector3D> v;
	v.reserve(n);

	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution(0, 1);

	for (int i = 0; i < n; i++)
	{
		v.emplace_back(distribution(generator), distribution(generator), distribution(generator));
	}

	Vector3D p(0.5, 0.5, 0.5);

	//Vector3D closest = t.findClosestPoint(v, p);
	//std::cout << closest << std::endl;

	std::unique_ptr<Tree> t = Tree::makeTree(v);

	const Vector3D closest = t->findClosestPoint(p);
	std::cout << closest << std::endl;
	std::cout << (closest - p).norme() << "\n";
}