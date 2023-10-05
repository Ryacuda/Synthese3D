#pragma once

#include <vector>
#include <chrono>

#include "Sphere.hpp"
#include "Ray.hpp"
#include "ObjectTree.hpp"
#include "Light.hpp"

bool sphereIntersectsRay_bool(const Sphere& s, Ray r);

double sphereIntersectsRay_depth(const Sphere& s, Ray r);

void test_scene_inter_bool();

void test_scene_inter_bool_bb();

void test_scene_inter_bool_bb_2();

void test_scene_inter_depth();

void test_scene_inter_norm();

void test_scene_inter_norm_optio();

void test_inter_tree_bool(int n);

void test_inter_benchmark_bool(int n);

void test_inter_benchmark_regular(std::size_t n, std::size_t m);

void test_tree_lights(int n);