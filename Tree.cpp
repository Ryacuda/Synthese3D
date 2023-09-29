#include "Tree.hpp"


void test_tree()
{
	const int n = 1000;
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

	for (int i = 0; i < v.size(); i++)
	{
		const Vector3D closest = t->findClosestPoint(p);
		std::cout << closest << std::endl;
		std::cout << (closest - p).norme() << "\n";
	}
}