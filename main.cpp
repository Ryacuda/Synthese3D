#include <vector>
#include <string>
#include <random>

#include "phys.hpp"
#include "intersections.hpp"
#include "illuminations.hpp"
#include "Tree.hpp"
#include "ObjectTree.hpp"

#include "Magick++.h"

int main()
{
	Magick::InitializeMagick("C:\\Program Files\\ImageMagick-7.1.1-Q16-HDRI");

	//test_scene_inter_norm_optio();
	
	//illum_zero();

	//illum_one();

	//illum_two();

	//test_scene_inter_bool_bb_2();

	//test_tree_object();

	//test_inter_benchmark_regular(16,16);

	test_inter_benchmark_bool(1);
	test_inter_benchmark_bool(10);
	test_inter_benchmark_bool(100);
	test_inter_benchmark_bool(1000);
	test_inter_benchmark_bool(10000);
	test_inter_benchmark_bool(100000);

	//tree_metrics(100);

	return 0;
}