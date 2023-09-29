#pragma once

#include <vector>

#include "Sphere.hpp"
#include "Ray.hpp"

bool sphereIntersectsRay_bool(Sphere s, Ray r);

double sphereIntersectsRay_depth(Sphere s, Ray r);

void test_scene_inter_bool();

void test_scene_inter_bool_bb();

void test_scene_inter_bool_bb_2();

void test_scene_inter_depth();

void test_scene_inter_norm();

void test_scene_inter_norm_optio();