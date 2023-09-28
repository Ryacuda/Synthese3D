#include <vector>
#include <string>
#include <random>

#include "phys.hpp"
#include "intersections.hpp"
#include "illuminations.hpp"
#include "Tree.hpp"

#include "Magick++.h"

Vector3D& findClosestPoint(std::vector<Vector3D>& points, const Vector3D& p1)
{
	if (points.size() == 0)
	{
		throw "Erreur vecteur vide";
	}

	double dist_min = (points[0] - p1).normeSQ();
	Vector3D closest(points[0]);

	for (int i = 1; i < points.size(); i++)
	{
		double dist = (points[i] - p1).normeSQ();
		if (dist < dist_min)
		{
			dist_min = dist;
			closest = points[i];
		}
	}

	return closest;
}

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

	Vector3D closest = findClosestPoint(v, p);

	std::cout << closest << std::endl;
}

int main()
{
	Magick::InitializeMagick("C:\\Program Files\\ImageMagick-7.1.1-Q16-HDRI");

	//test_scene_inter_norm_optio();
	
	//illum_zero();

	//illum_one();

	//illum_two();

	test_tree();

	return 0;
}