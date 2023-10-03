#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <random>

#include "Sphere.hpp"
#include "Ray.hpp"

class ObjectTree
{
public:

	// Constructors
	ObjectTree(std::unique_ptr<ObjectTree> left, std::unique_ptr<ObjectTree> right, Sphere key, int axis);

	// Accessors
	BoundingBox getBoundingbox();

	// Methods
	static std::unique_ptr<ObjectTree> makeTree(std::vector<Sphere>& keys);

	std::optional<Vector3D> findIntersection(const Ray& r);

	int depth();

	void displayBBVolumes();

private:
	// Attributes
	std::unique_ptr<ObjectTree> m_left_tree;
	std::unique_ptr<ObjectTree> m_right_tree;
	Sphere m_key;
	BoundingBox m_boundingbox;
	int m_axis;

	// Private methods
	static std::unique_ptr<ObjectTree> makeTree(std::vector<Sphere> keys, int axis);

	BoundingBox computeBB();

	std::optional<Vector3D> findIntersection(const Ray& r, std::optional<Vector3D> closest_intersection, double dist_to_closest);
};

void test_tree_object();

void test_inter_tree();

void tree_metrics(int n);