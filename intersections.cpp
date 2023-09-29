#include "intersections.hpp"
#include "Magick++.h"

bool sphereIntersectsRay_bool(Sphere s, Ray r)
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

double sphereIntersectsRay_depth(Sphere s, Ray r)
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

			if (r.hitBB(sphere1))
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

			std::optional<Vector3D> p_i = r.hitBB(sphere1);
			
			double dist = 1000;
			if (p_i.has_value())
			{
				dist = (p_i.value() - r.getOrigin()).norme();
			}

			if (dist >= 0)
			{
				image.pixelColor(x, y, Magick::ColorRGB(dist/1000, dist/1000, dist/1000));
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

			std::optional<Vector3D> p_i = r.hit(sphere1);
			double dist = -1;

			if (p_i.has_value())
			{
				dist = (r.getOrigin() - p_i.value()).normeSQ();
			}

			if (dist >= 0)
			{
				image.pixelColor(x, y, Magick::ColorRGB(1, 1, 1));
			}
		}
	}

	std::string filename = "output/sphere_norm_optio.png";
	image.write(filename);
}