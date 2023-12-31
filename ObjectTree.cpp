#include "ObjectTree.hpp"

// Constructors
ObjectTree::ObjectTree(std::unique_ptr<ObjectTree> left, std::unique_ptr<ObjectTree> right, const Sphere& key, int axis)
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

std::optional<Vector3D> ObjectTree::findIntersection(const Ray& r)
{
	if (r.hitBB(m_boundingbox).has_value())
	{
		return findIntersection(r, {}, std::numeric_limits<double>::infinity());
	}
	
	return {};
}

std::optional< std::pair<double, Vector3D> > ObjectTree::findNormalAtIntersection(const Ray& r)
{
	if (r.hitBB(m_boundingbox).has_value())
	{
		return findNormalAtIntersection(r, {}, std::numeric_limits<double>::infinity());
	}

	return {};
}

int ObjectTree::depth()
{
	int depth_left = (m_left_tree ? m_left_tree->depth() : 0);
	int depth_right = (m_right_tree ? m_right_tree->depth() : 0);

	return 1 + std::max(depth_left, depth_right);
}

void ObjectTree::displayBBVolumes()
{
	std::cout << "--------------------" << std::endl;
	std::cout << "Boundingbox vol : \t" << m_boundingbox.volume() << std::endl;

	std::cout << "left BB vol : \t" << (m_left_tree ? m_left_tree->getBoundingbox().volume() : 0) << std::endl;
	std::cout << "right BB vol : \t" << (m_right_tree ? m_right_tree->getBoundingbox().volume() : 0) << std::endl << std::endl;

	if (m_left_tree)
	{
		m_left_tree->displayBBVolumes();
	}

	if (m_right_tree)
	{
		m_right_tree->displayBBVolumes();
	}
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


std::optional<Vector3D> ObjectTree::findIntersection(const Ray& r, std::optional<Vector3D> closest_intersection, double dist_to_closest)
{
	// check intersection with the object
	std::optional<double> t = r.hit(m_key);
	if (t.has_value() && t.value() < dist_to_closest && t.value() >= 0)
	{
		closest_intersection = t.value() * r.getDirection() + r.getOrigin();
		dist_to_closest = t.value();
	}
	
	if (m_left_tree)
	{
		std::optional<double> t = r.hitBB(m_left_tree->getBoundingbox());
		if (t.has_value() && (t.value() < dist_to_closest) && t.value() >= 0)
		{
			std::optional<Vector3D> p_i_left = m_left_tree->findIntersection(r, closest_intersection, dist_to_closest);

			if (p_i_left.has_value())
			{
				double dist = (p_i_left.value() - r.getOrigin()).normeSQ();

				if (dist < dist_to_closest)
				{
					closest_intersection = p_i_left;
					dist_to_closest = dist;
				}
			}
		}
	}
	
	if(m_right_tree)
	{
		std::optional<double> t = r.hitBB(m_right_tree->getBoundingbox());
		if (t.has_value() && t.value() < dist_to_closest && t.value() >= 0)
		{
			std::optional<Vector3D> p_i_right = m_right_tree->findIntersection(r, closest_intersection, dist_to_closest);

			if (p_i_right.has_value())
			{
				double dist = (p_i_right.value() - r.getOrigin()).normeSQ();

				if (dist < dist_to_closest)
				{
					closest_intersection = p_i_right;
					dist_to_closest = dist;
				}
			}
		}
	}

	return closest_intersection;
}


std::optional< std::pair<double, Vector3D> > ObjectTree::findNormalAtIntersection(const Ray& r, std::optional<Vector3D> normal_at_intersection, double dist_to_closest)
{
	// check intersection with the object
	std::optional<double> t = r.hit(m_key);

	if (t.has_value() && t.value() < dist_to_closest && t.value() >= 0)
	{
		normal_at_intersection = m_key.normal(t.value() * r.getDirection() + r.getOrigin());
		dist_to_closest = t.value();
	}

	if (m_left_tree)
	{
		std::optional<double> t_left_BB = r.hitBB(m_left_tree->getBoundingbox());
		if (t_left_BB.has_value() && (t_left_BB.value() < dist_to_closest) && t_left_BB.value() >= 0)
		{
			std::optional< std::pair<double, Vector3D> > normal_left = m_left_tree->findNormalAtIntersection(r, normal_at_intersection, dist_to_closest);

			if (normal_left.has_value() && normal_left.value().first >= 0 && normal_left.value().first < dist_to_closest)
			{
				dist_to_closest = normal_left.value().first;
				normal_at_intersection = normal_left.value().second;
			}
		}
	}

	if (m_right_tree)
	{
		std::optional<double> t_right_BB = r.hitBB(m_right_tree->getBoundingbox());
		if (t_right_BB.has_value() && t_right_BB.value() < dist_to_closest && t_right_BB.value() >= 0)
		{
			std::optional< std::pair<double, Vector3D> > normal_right = m_right_tree->findNormalAtIntersection(r, normal_at_intersection, dist_to_closest);

			if (normal_right.has_value() && normal_right.value().first >= 0 && normal_right.value().first < dist_to_closest)
			{
				dist_to_closest = normal_right.value().first;
				normal_at_intersection = normal_right.value().second;
			}
		}
	}

	if (normal_at_intersection.has_value())
	{
		return std::pair < double, Vector3D >(dist_to_closest, normal_at_intersection.value());
	}
	else
	{
		return {};
	}
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


void test_inter_tree()
{
	const int n = 10;
	std::vector<Sphere> v;
	v.reserve(n);

	std::default_random_engine generator;
	std::uniform_real_distribution<double> coord_distrib(-1000, 1000);
	std::uniform_real_distribution<double> radius_distrib(20, 60);

	for (int i = 0; i < n; i++)
	{
		v.emplace_back(Vector3D(1000, coord_distrib(generator), coord_distrib(generator)), radius_distrib(generator));
	}

	v.emplace_back(Vector3D(1000, 80, 80), 100);

	std::unique_ptr<ObjectTree> t = ObjectTree::makeTree(v);

	const Ray r(Vector3D(0, 0, 0), Vector3D(1, 0, 0));

	std::optional<Vector3D> inter = t->findIntersection(r);

	std::cout << inter.has_value() << std::endl;
}

void tree_metrics(int n)
{
	std::vector<Sphere> v;
	v.reserve(n);

	std::default_random_engine generator;
	std::uniform_real_distribution<double> coord_distrib(-1000, 1000);
	std::uniform_real_distribution<double> radius_distrib(20, 60);

	for (int i = 0; i < n; i++)
	{
		v.emplace_back(Vector3D(1000, coord_distrib(generator), coord_distrib(generator)), radius_distrib(generator));
	}

	v.emplace_back(Vector3D(1000, 80, 80), 100);

	std::unique_ptr<ObjectTree> t = ObjectTree::makeTree(v);

	std::cout << "depth " << t->depth() << std::endl;

	t->displayBBVolumes();
}