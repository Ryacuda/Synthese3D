#include <vector>
#include <string>

#include "phys.hpp"
#include "intersections.hpp"
#include "illuminations.hpp"

#include "Magick++.h"

int main()
{
	Magick::InitializeMagick("C:\\Program Files\\ImageMagick-7.1.1-Q16-HDRI");

	//test_scene_inter_norm_optio();
	
	
	//illum_zero();

	illum_one();

	//illum_two();

	return 0;
}