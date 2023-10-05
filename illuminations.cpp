#include "illuminations.hpp"

void illum_zero()
{
	// Image
	unsigned int image_width = 800;
	unsigned int image_height = 600;
	Magick::Image image(Magick::Geometry(image_width, image_height), Magick::ColorRGB(0, 0, 0));

	// objects
	const Sphere sphere1(Vector3D(400, 300, 400), 100);

	const Light light1(Vector3D(200, -800, 0), 1000000);

	for (unsigned int x = 0; x < image_height; x++)
	{
		for (unsigned int y = 0; y < image_width; y++)
		{
			const Ray r(Vector3D(x, y, 0), Vector3D(0, 0, 1));

			std::optional<double> t = r.hit(sphere1);

			if (t.has_value())
			{
				Vector3D inter = (t.value() * r.getDirection() + r.getOrigin());

				Vector3D light_dir = inter - light1.getPosition();

				const Ray lightray(light1.getPosition(), light_dir/light_dir.norme());

				double dist_to_light = (inter - light1.getPosition()).norme() -0.001;

				std::optional<double> t_light = lightray.hit(sphere1);

				if (t_light.has_value() && t_light.value() >= dist_to_light)
				{
					Vector3D norm = sphere1.normal(inter);

					double dp = dotProduct(norm, lightray.getDirection());		// == cos(angle)

					if (dp < 0)
					{
						dp = -dp;
					}

					double r = light1.getIntensity() * dp / (dist_to_light * dist_to_light);
					double g = light1.getIntensity() * dp / (dist_to_light * dist_to_light);
					double b = light1.getIntensity() * dp / (dist_to_light * dist_to_light);

					image.pixelColor(x, y, Magick::ColorRGB(r, g, b));
				}
			}
		}
	}

	std::string filename = "output/illum_zero.png";
	image.write(filename);
}

void illum_one()
{
	// Image
	unsigned int image_width = 800;
	unsigned int image_height = 600;
	Magick::Image image(Magick::Geometry(image_width, image_height), Magick::ColorRGB(0, 0, 0));

	// objects
	const Sphere sphere1(Vector3D(250, 400, 300), 100);

	// lights
	std::vector<Light> light_vec;
	light_vec.emplace_back(Vector3D(200, -800, 0), 1000000);
	light_vec.emplace_back(Vector3D(600, 800, 0), 50000);

	for (unsigned int x = 0; x < image_height; x++)
	{
		for (unsigned int y = 0; y < image_width; y++)
		{
			const Ray r(Vector3D(x, y, 0), Vector3D(0, 0, 1));

			std::optional<double> t = r.hit(sphere1);
			double dist = -1;

			if (t.has_value())
			{
				Vector3D inter = (t.value() * r.getDirection() + r.getOrigin());

				double r = 0;
				double g = 0;
				double b = 0;

				for (const Light& light : light_vec)
				{
					Vector3D light_dir = inter - light.getPosition();
					const Ray lightray(light.getPosition(), light_dir / light_dir.norme());

					double dist_to_light = t.value() - 0.001;

					std::optional<double> t_light = lightray.hit(sphere1);

					if (t_light.has_value() && t_light.value() >= dist_to_light)
					{
						Vector3D norm = sphere1.normal(inter);

						double dp = dotProduct(norm, lightray.getDirection());		// == cos(angle)
						dp = std::abs(dp);

						r += light.getIntensity() * dp / (dist_to_light * dist_to_light);
						g += light.getIntensity() * dp / (dist_to_light * dist_to_light);
						b += light.getIntensity() * dp / (dist_to_light * dist_to_light);
					}
				}

				image.pixelColor(x, y, Magick::ColorRGB(r, g, b));
			}
		}
	}

	std::string filename = "output/illum_one.png";
	image.write(filename);
}


void illum_two()
{
	// Image
	unsigned int image_width = 800;
	unsigned int image_height = 600;
	Magick::Image image(Magick::Geometry(image_width, image_height), Magick::ColorRGB(0, 0, 0));

	// objects
	std::vector<Sphere> sphere_list;
	sphere_list.emplace_back(Vector3D(600, 300, 400), 100);
	sphere_list.emplace_back(Vector3D(200, 300, 400), 100);
	sphere_list.emplace_back(Vector3D(image_width/2, image_height/2, 0), 2000);

	const Light light1(Vector3D(200, -800, 0), 1000000);

	for (unsigned int x = 0; x < image_width; x++)
	{
		for (unsigned int y = 0; y < image_height; y++)
		{
			const Ray r(Vector3D(x, y, 0), Vector3D(0, 0, 1));

			Vector3D closest_hit(0, 0, 0);
			double dist_to_closest_hit = std::numeric_limits<double>::infinity();
			Vector3D normal_at_closest_hit(0, 0, 0);
			bool intersection = false;

			for (const Sphere& s : sphere_list)
			{
				std::optional<double> t = r.hit(s);

				if (t.has_value() && t.value() < dist_to_closest_hit && t.value() >= 0)
				{
					Vector3D inter = (t.value() * r.getDirection() + r.getOrigin());

					closest_hit = inter;
					normal_at_closest_hit = s.normal(inter);
					dist_to_closest_hit = t.value();
					intersection = true;
				}
			}

			if (intersection)
			{
				Vector3D light_dir = closest_hit - light1.getPosition();

				const Ray lightray(light1.getPosition(), light_dir / light_dir.norme());

				double dist_to_light = (closest_hit - light1.getPosition()).norme() - 0.001;

				bool illumined = true;

				for (const Sphere& s : sphere_list)
				{
					std::optional<double> t_light = lightray.hit(s);

					if (t_light.has_value() && t_light.value() <= dist_to_light && t_light >= 0)
					{
						illumined = false;
					}
				}

				double dp = dotProduct(normal_at_closest_hit, lightray.getDirection());		// == cos(angle)
				dp = std::abs(dp);

				if (illumined)
				{
					double red = light1.getIntensity() * dp / (dist_to_light * dist_to_light);
					double green = light1.getIntensity() * dp / (dist_to_light * dist_to_light);
					double blue = light1.getIntensity() * dp / (dist_to_light * dist_to_light);

					image.pixelColor(x, y, Magick::ColorRGB(red, green, blue));
				}
				
			}
		}
	}

	std::string filename = "output/illum_two.png";
	image.write(filename);
}
