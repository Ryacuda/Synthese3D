#include "ObjectTree.hpp"

// Constructors
ObjectTree::ObjectTree(std::unique_ptr<ObjectTree> left, std::unique_ptr<ObjectTree> right, Sphere key, int axis)
	: m_left_tree(std::move(left)), m_right_tree(std::move(right)), m_key(key), m_axis(axis), m_boundingbox(Vector3D(0,0,0), Vector3D(0,0,0))
{

}

// Accessors
BoundingBox ObjectTree::getBoundingbox()
{
	return m_boundingbox;
}

// Methods
std::unique_ptr<ObjectTree> ObjectTree::makeTree(std::vector<Sphere>& keys)
{
	std::unique_ptr<ObjectTree> t = makeTree(keys, 0);
	t->computeBB();
	return t;
}

// Private methods
std::unique_ptr<ObjectTree> ObjectTree::makeTree(std::vector<Sphere> keys, int axis)
{
	if (keys.size() == 0)
	{
		return nullptr;
	}

	// sort spheres depending on their center coordinates
	std::sort(keys.begin(), keys.end(), [&axis](const Sphere& s1, const Sphere& s2)
		{	// sorting along a different axis on each recursive call
			Vector3D v1 = s1.getCenter();
			Vector3D v2 = s2.getCenter();

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

	std::unique_ptr<ObjectTree> left_tree = makeTree(std::vector<Sphere>(keys.begin(), keys.begin() + middle), (axis + 1) % 3);
	std::unique_ptr<ObjectTree> right_tree = makeTree(std::vector<Sphere>(keys.begin() + middle + 1, keys.end()), (axis + 1) % 3);

	return std::make_unique<ObjectTree>(std::move(left_tree), std::move(right_tree), keys[middle], axis);
}

BoundingBox ObjectTree::computeBB()
{
	std::optional<BoundingBox> bb_union = {};

	if (m_left_tree)
	{
		if (m_right_tree)
		{
			bb_union = BBUnion(std::vector<BoundingBox>({m_left_tree->computeBB(), m_right_tree->computeBB(), m_key.getBoundingBox()}));
		}
		else
		{
			bb_union = BBUnion(std::vector<BoundingBox>({ m_left_tree->computeBB(), m_key.getBoundingBox() }));
		}
	}
	else
	{
		if (m_right_tree)
		{
			bb_union = BBUnion(std::vector<BoundingBox>({ m_right_tree->computeBB(), m_key.getBoundingBox() }));
		}
		else
		{
			bb_union = m_key.getBoundingBox();
		}
	}

	m_boundingbox = bb_union.value();

	return m_boundingbox;
}

// Functions

void test_tree_object()
{
	const int n = 3;
	std::vector<Sphere> v;
	v.reserve(n);

	std::default_random_engine generator;
	std::uniform_real_distribution<double> coord_distrib(100, 1000);
	std::uniform_real_distribution<double> radius_distrib(20, 60);

	for (int i = 0; i < n; i++)
	{
		v.emplace_back(Vector3D(coord_distrib(generator), coord_distrib(generator), coord_distrib(generator)), radius_distrib(generator));
	}

	std::unique_ptr<ObjectTree> t = ObjectTree::makeTree(v);

	//std::cout << closest << std::endl;
	//std::cout << (closest - p).norme() << "\n";
}