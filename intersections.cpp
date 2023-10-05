#include "intersections.hpp"

#include "Magick++.h"

bool sphereIntersectsRay_bool(const Sphere& s, Ray r)
{
	double a = r.getDirection().normeSQ();
	double b = 2 * dotProduct(r.getDirection(), r.getOrigin() - s.getCenter());
	double c = (r.getOrigin() - s.getCenter()).normeSQ() - (s.getRadius() * s.getRadius());

	double delta = (b * b) - (4 * a * c);

	if (delta == 0)
	{
		return ((-b / (2 * a)) > 0);
	}
	else if (delta > 0)
	{
		double t1 = (-b - sqrt(delta)) / (2 * a);
		double t2 = (-b + sqrt(delta)) / (2 * a);

		return (t1 > 0 || t2 > 0);
	}

	return false;
}

double sphereIntersectsRay_depth(const Sphere& s, Ray r)
{
	double a = r.getDirection().normeSQ();
	double b = 2 * dotProduct(r.getDirection(), r.getOrigin() - s.getCenter());
	double c = (r.getOrigin() - s.getCenter()).normeSQ() - (s.getRadius() * s.getRadius());

	double delta = (b * b) - (4 * a * c);

	if (delta == 0)
	{
		return -b / (2 * a);
	}
	else if (delta > 0)
	{
		double t1 = (-b - sqrt(delta)) / (2 * a);
		double t2 = (-b + sqrt(delta)) / (2 * a);

		if (t1 >= 0)
		{
			if (t2 >= 0)
			{
				return std::min(t1, t2);
			}
			else
			{
				return t1;
			}
		}
		else
		{
			return t2;
		}
	}
	else
	{
		return -1;
	}
}

void test_scene_inter_bool()
{
	// Image
	unsigned int image_width = 800;
	unsigned int image_height = 600;
	Magick::Image image(Magick::Geometry(image_width, image_height), Magick::ColorRGB(0, 0, 0));

	// objects
	Sphere sphere1(Vector3D(400, 300, 400), 100);


	for (unsigned int x = 0; x < image_height; x++)
	{
		for (unsigned int y = 0; y < image_width; y++)
		{
			const Ray r(Vector3D(x, y, 0), Vector3D(0, 0, 1));

			if (sphereIntersectsRay_bool(sphere1, r))
			{
				image.pixelColor(x, y, Magick::ColorRGB(1, 1, 1));
			}
		}
	}

	std::string filename = "output/sphere_bool.png";
	image.write(filename);
}

void test_scene_inter_bool_bb()
{
	// Image
	unsigned int image_width = 800;
	unsigned int image_height = 600;
	Magick::Image image(Magick::Geometry(image_width, image_height), Magick::ColorRGB(0, 0, 0));

	// objects
	Sphere sphere1(Vector3D(400, 300, 400), 100);


	for (unsigned int x = 0; x < image_height; x++)
	{
		for (unsigned int y = 0; y < image_width; y++)
		{
			const Ray r(Vector3D(x, y, 0), Vector3D(0, 0, 1));

			std::optional<double> t = r.hitBB(sphere1.getBoundingBox());

			if (t.has_value() && t.value() >= 0)
			{
				image.pixelColor(x, y, Magick::ColorRGB(1, 1, 1));
			}
		}
	}

	std::string filename = "output/sphere_bool_bb.png";
	image.write(filename);
}

void test_scene_inter_bool_bb_2()
{
	// Image
	unsigned int image_width = 800;
	unsigned int image_height = 600;
	Magick::Image image(Magick::Geometry(image_width, image_height), Magick::ColorRGB(0, 0, 0));

	// objects
	Sphere sphere1(Vector3D(400, 300, 400), 100);


	for (unsigned int x = 0; x < image_width; x++)
	{
		for (unsigned int y = 0; y < image_height; y++)
		{
			const Ray r(Vector3D(x, y, 0), Vector3D(0, 0, 1));

			std::optional<double> t = r.hitBB(sphere1.getBoundingBox());
			
			if (t.has_value() && t.value() >= 0)
			{
				image.pixelColor(x, y, Magick::ColorRGB(0.5, 0.5, 0.5));
			}
		}
	}

	std::string filename = "output/sphere_bool_bb_2.png";
	image.write(filename);
}

void test_scene_inter_depth()
{
	// Image
	unsigned int image_width = 800;
	unsigned int image_height = 600;
	Magick::Image image(Magick::Geometry(image_width, image_height), Magick::ColorRGB(0, 0, 0));

	// objects
	Sphere sphere1(Vector3D(400, 300, 400), 100);
	double min_color = 0.5;
	double max_color = 1;

	std::vector< std::vector<double> > distances(image_height, std::vector<double>(image_width, -1));
	double dist_min = std::numeric_limits<double>::infinity();
	double dist_max = -std::numeric_limits<double>::infinity();

	for (unsigned int x = 0; x < image_height; x++)
	{
		for (unsigned int y = 0; y < image_width; y++)
		{
			const Ray r(Vector3D(x, y, 0), Vector3D(0, 0, 1));

			double dist = sphereIntersectsRay_depth(sphere1, r);
			if (dist >= 0)
			{
				distances[x][y] = dist;

				if (dist < dist_min)
				{
					dist_min = dist;
				}
				else if (dist > dist_max)
				{
					dist_max = dist;
				}
			}
		}
	}

	for (unsigned int x = 0; x < image_height; x++)
	{
		for (unsigned int y = 0; y < image_width; y++)
		{
			double dist = distances[x][y];
			if (dist >= 0)
			{
				dist -= dist_min;
				dist /= dist_max - dist_min;
				dist = 1 - dist;					// to invert the intensity
				dist *= max_color - min_color;
				dist += min_color;

				image.pixelColor(x, y, Magick::ColorRGB(dist, dist, dist));
			}
		}
	}

	std::string filename = "output/sphere_depth.png";
	image.write(filename);
}


void test_scene_inter_norm()
{
	// Image
	unsigned int image_width = 800;
	unsigned int image_height = 600;
	Magick::Image image(Magick::Geometry(image_width, image_height), Magick::ColorRGB(0, 0, 0));

	// objects
	const Sphere sphere1(Vector3D(400, 300, 400), 100);
	double min_color = 0.5;
	double max_color = 1;

	for (unsigned int x = 0; x < image_height; x++)
	{
		for (unsigned int y = 0; y < image_width; y++)
		{
			const Ray r(Vector3D(x, y, 0), Vector3D(0, 0, 1));

			double dist = sphereIntersectsRay_depth(sphere1, r);
			if (dist >= 0)
			{
				Vector3D point(r.getOrigin() + r.getDirection() * dist);
				Vector3D normal(point - sphere1.getCenter());
				normal /= normal.norme();

				double dp = dotProduct(normal, r.getDirection());

				if (dp < 0)
				{
					image.pixelColor(x, y, Magick::ColorRGB(std::abs(normal.getX()), std::abs(normal.getY()), std::abs(normal.getZ())));
				}
			}
		}
	}

	std::string filename = "output/sphere_norm.png";
	image.write(filename);
}

void test_scene_inter_norm_optio()
{
	// Image
	unsigned int image_width = 800;
	unsigned int image_height = 600;
	Magick::Image image(Magick::Geometry(image_width, image_height), Magick::ColorRGB(0, 0, 0));

	// objects
	const Sphere sphere1(Vector3D(400, 300, 400), 100);
	double min_color = 0.5;
	double max_color = 1;

	for (unsigned int x = 0; x < image_height; x++)
	{
		for (unsigned int y = 0; y < image_width; y++)
		{
			const Ray r(Vector3D(x, y, 0), Vector3D(0, 0, 1));

			std::optional<double> t = r.hit(sphere1);

			if (t.has_value() && t.value() >=0)
			{
				image.pixelColor(x, y, Magick::ColorRGB(1, 1, 1));
			}
		}
	}

	std::string filename = "output/sphere_norm_optio.png";
	image.write(filename);
}

void test_inter_tree_bool(int n)
{
	// clock
	typedef std::chrono::high_resolution_clock myclock;
	myclock::time_point beginning = myclock::now();

	// Image
	int image_width = 800;
	int image_height = 600;
	Magick::Image image(Magick::Geometry(image_width, image_height), Magick::ColorRGB(0, 0, 0));

	// objects
	std::vector<Sphere> sphere_list;
	sphere_list.reserve(n);

	std::default_random_engine generator;
	std::uniform_real_distribution<double> coord_distrib(-500, 500);
	std::uniform_real_distribution<double> radius_distrib(20, 50);

	for (int i = 0; i < n; i++)
	{
		sphere_list.emplace_back(Vector3D(1000, coord_distrib(generator), coord_distrib(generator)), radius_distrib(generator));
	}

	std::unique_ptr<ObjectTree> t = ObjectTree::makeTree(sphere_list);

	/* --------------------------------------------------------------------------------------------- */
	myclock::time_point start_rendering_tree = myclock::now();

	for (int x = 0; x < image_width; x++)
	{
		for (int y = 0; y < image_height; y++)
		{
			const Ray r(Vector3D(0, x - image_width / 2, y - image_height / 2), Vector3D(1, 0, 0));

			std::optional<Vector3D> inter = t->findIntersection(r);

			if (inter.has_value())
			{
				image.pixelColor(x, y, Magick::ColorRGB(1, 1, 1));
			}
		}
	}

	myclock::time_point end_tree_rendering = myclock::now();


	std::string filename = "output/bench_tree.png";
	image.write(filename);

	std::cout << "--------- n = " << n << " -------- - " << std::endl;
	std::cout << "Time to render with tree : \t" << std::chrono::duration_cast<std::chrono::duration<double>>(end_tree_rendering - start_rendering_tree).count() << " seconds" << std::endl;
}



void test_inter_benchmark_bool(int n)
{
	// clock
	typedef std::chrono::high_resolution_clock myclock;
	myclock::time_point beginning = myclock::now();

	// Image
	int image_width = 16;
	int image_height = 9;
	Magick::Image image(Magick::Geometry(image_width, image_height), Magick::ColorRGB(0, 0, 0));

	// objects
	std::vector<Sphere> sphere_list;
	sphere_list.reserve(n);

	std::default_random_engine generator;
	std::uniform_real_distribution<double> coord_distrib(-500, 500);
	std::uniform_real_distribution<double> radius_distrib(20, 50);

	for (int i = 0; i < n; i++)
	{
		sphere_list.emplace_back(Vector3D(1000, coord_distrib(generator), coord_distrib(generator)), radius_distrib(generator));
	}

	std::unique_ptr<ObjectTree> t = ObjectTree::makeTree(sphere_list);

	/* --------------------------------------------------------------------------------------------- */
	myclock::time_point start_rendering_tree = myclock::now();

	for (int x = 0; x < image_width; x++)
	{
		for (int y = 0; y < image_height; y++)
		{
			const Ray r(Vector3D(0, x - image_width/2, y - image_height/2), Vector3D(1, 0, 0));

			std::optional<Vector3D> inter = t->findIntersection(r);

			if (inter.has_value())
			{
				image.pixelColor(x, y, Magick::ColorRGB(1, 1, 1));
			}
		}
	}

	myclock::time_point end_tree_rendering = myclock::now();


	std::string filename = "output/bench_tree.png";
	image.write(filename);
	/* --------------------------------------------------------------------------------------------- */

	myclock::time_point start_rendering_vec = myclock::now();

	for (int x = 0; x < image_width; x++)
	{
		for (int y = 0; y < image_height; y++)
		{
			const Ray r(Vector3D(0, x - image_width / 2, y - image_height / 2), Vector3D(1, 0, 0));

			bool inter = false;

			for (const Sphere& s : sphere_list)
			{
				std::optional<double> t = r.hit(s);

				if (t.has_value() && t >= 0)
				{
					inter = true;
				}
			}

			if (inter)
			{
				image.pixelColor(x, y, Magick::ColorRGB(1, 1, 1));
			}
		}
	}

	myclock::time_point end_rendering_vec = myclock::now();

	filename = "output/bench_vec.png";
	image.write(filename);

	std::cout << "--------- n = " << n << " -------- - " << std::endl;
	std::cout << "Time to render with tree : \t" << std::chrono::duration_cast< std::chrono::duration<double> >(end_tree_rendering - start_rendering_tree).count() << " seconds" << std::endl;
	std::cout << "Time to render with vec : \t" << std::chrono::duration_cast<std::chrono::duration<double>>(end_rendering_vec - start_rendering_vec).count() << " seconds" << std::endl << std::endl;
}


void test_inter_benchmark_regular(std::size_t n, std::size_t m)
{
	// clock
	typedef std::chrono::high_resolution_clock myclock;
	myclock::time_point beginning = myclock::now();

	// Image
	int image_width = 800;
	int image_height = 450;
	Magick::Image image(Magick::Geometry(image_width, image_height), Magick::ColorRGB(0, 0, 0));

	// objects
	std::vector<Sphere> sphere_list;
	sphere_list.reserve(n*m);

	std::default_random_engine generator;
	std::uniform_real_distribution<double> coord_distrib(-500, 500);
	std::uniform_real_distribution<double> radius_distrib(20, 50);

	double sphere_radius = std::min(image_width / (n * 2), image_height / (m * 2));
	double density = 3;		// inverse of density :)

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			sphere_list.emplace_back(Vector3D(1000, sphere_radius * density * i, sphere_radius * density * j), sphere_radius);
		}
	}

	std::unique_ptr<ObjectTree> t = ObjectTree::makeTree(sphere_list);

	/* --------------------------------------------------------------------------------------------- */
	myclock::time_point start_rendering_tree = myclock::now();

	for (int x = 0; x < image_width; x++)
	{
		for (int y = 0; y < image_height; y++)
		{
			const Ray r(Vector3D(0, x, y), Vector3D(1, 0, 0));

			std::optional<Vector3D> inter = t->findIntersection(r);

			if (inter.has_value())
			{
				image.pixelColor(x, y, Magick::ColorRGB(1, 1, 1));
			}
		}
	}

	myclock::time_point end_tree_rendering = myclock::now();


	std::string filename = "output/bench_tree_reg.png";
	image.write(filename);
	/* --------------------------------------------------------------------------------------------- */

	myclock::time_point start_rendering_vec = myclock::now();

	for (int x = 0; x < image_width; x++)
	{
		for (int y = 0; y < image_height; y++)
		{
			const Ray r(Vector3D(0, x, y), Vector3D(1, 0, 0));

			bool inter = false;

			for (const Sphere& s : sphere_list)
			{
				std::optional<double> t = r.hit(s);

				if (t.has_value())
				{
					inter = true;
				}
			}

			if (inter)
			{
				image.pixelColor(x, y, Magick::ColorRGB(1, 1, 1));
			}
		}
	}

	myclock::time_point end_rendering_vec = myclock::now();

	filename = "output/bench_vec_reg.png";
	image.write(filename);

	std::cout << "--------- n = " << n*m << " ---------- " << std::endl;
	std::cout << "Time to render with tree : \t" << std::chrono::duration_cast<std::chrono::duration<double>>(end_tree_rendering - start_rendering_tree).count() << " seconds" << std::endl;
	std::cout << "Time to render with vec : \t" << std::chrono::duration_cast<std::chrono::duration<double>>(end_rendering_vec - start_rendering_vec).count() << " seconds" << std::endl << std::endl;
}


void test_tree_lights(int n)
{
	// clock
	typedef std::chrono::high_resolution_clock myclock;
	myclock::time_point beginning = myclock::now();

	// Image
	int image_width = 800;
	int image_height = 450;
	Magick::Image image(Magick::Geometry(image_width, image_height), Magick::ColorRGB(0, 0, 0));

	// objects
	std::vector<Sphere> sphere_list;
	sphere_list.reserve(n);

	std::default_random_engine generator;
	std::uniform_real_distribution<double> coord_yz_distrib(-500, 500);
	std::uniform_real_distribution<double> coord_x_distrib(980, 1020);
	std::uniform_real_distribution<double> radius_distrib(20, 50);
	std::uniform_real_distribution<double> color_distrib(0.5, 1);

	for (int i = 0; i < n; i++)
	{
		sphere_list.emplace_back(Vector3D(coord_x_distrib(generator), coord_yz_distrib(generator), coord_yz_distrib(generator)),
								 radius_distrib(generator));
	}


	std::unique_ptr<ObjectTree> tree = ObjectTree::makeTree(sphere_list);

	// lights
	const Light light1(Vector3D(500, 0, 0), 100000);

	/* --------------------------------------------------------------------------------------------- */
	myclock::time_point start_rendering_tree = myclock::now();
	
	for (int x = 0; x < image_width; x++)
	{
		for (int y = 0; y < image_height; y++)
		{
			const Ray r(Vector3D(0, x - image_width / 2, y - image_height / 2), Vector3D(1, 0, 0));

			std::optional< std::pair<double, Vector3D> >normal = tree->findNormalAtIntersection(r);

			if (normal.has_value())
			{				
				Vector3D inter = (normal.value().first * r.getDirection() + r.getOrigin());

				Vector3D light_dir = inter - light1.getPosition();

				const Ray lightray(light1.getPosition(), light_dir / light_dir.norme());

				double dist_to_light = (inter - light1.getPosition()).norme() - 0.001;

				std::optional<Vector3D> light_inter = tree->findIntersection(lightray);

				if (light_inter.has_value() && (light1.getPosition() - light_inter.value()).norme() > dist_to_light)
				{
					double dp = dotProduct(normal.value().second, lightray.getDirection());		// == cos(angle)
					dp = std::abs(dp);

					double red = light1.getIntensity() * dp / (dist_to_light * dist_to_light);
					double green = light1.getIntensity() * dp / (dist_to_light * dist_to_light);
					double blue = light1.getIntensity() * dp / (dist_to_light * dist_to_light);

					image.pixelColor(x, y, Magick::ColorRGB(red, green, blue));

				}
				
			}
		}
	}

	myclock::time_point end_tree_rendering = myclock::now();


	std::string filename = "output/tree_lights_render.png";
	image.write(filename);

	std::cout << "--------- n = " << n << " ---------- " << std::endl;
	std::cout << "Time to render with tree : \t" << std::chrono::duration_cast<std::chrono::duration<double>>(end_tree_rendering - start_rendering_tree).count() << " seconds" << std::endl;
}