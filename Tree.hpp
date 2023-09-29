#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <random>

#include "phys.hpp"

class Tree
{
public:

	// Constructors
	Tree(std::unique_ptr<Tree> left, std::unique_ptr<Tree> right, Vector3D key, int axis);

	// Accessors

	// Methods
	static std::unique_ptr<Tree> makeTree(std::vector<Vector3D>& keys);

	Vector3D findClosestPoint(const Vector3D& p);

private:
	// Attributes
	std::unique_ptr<Tree> m_left_tree;
	std::unique_ptr<Tree> m_right_tree;
	Vector3D m_key;
	int m_axis;

	// Private methods
	static std::unique_ptr<Tree> makeTree(std::vector<Vector3D> keys, int axis);

	Vector3D findClosestPoint(const Vector3D& p, Vector3D closest_p, double dist_to_closest);
};

void test_tree();