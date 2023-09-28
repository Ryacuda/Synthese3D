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

			std::optional<Vector3D> p_i = r.hit(sphere1);

			if (p_i.has_value())
			{
				Vector3D light_dir = p_i.value() - light1.getPosition();

				const Ray lightray(light1.getPosition(), light_dir/light_dir.norme());

				double dist_to_light = (p_i.value() - light1.getPosition()).norme() -0.001;

				std::optional<Vector3D> p_light = lightray.hit(sphere1);

				if (p_light.has_value() && (p_light.value() - light1.getPosition()).norme() >= dist_to_light)
				{
					Vector3D norm = sphere1.normal(p_i.value());

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

			std::optional<Vector3D> p_i = r.hit(sphere1);
			double dist = -1;

			if (p_i.has_value())
			{
				double r = 0;
				double g = 0;
				double b = 0;

				for (const Light& light : light_vec)
				{
					Vector3D light_dir = p_i.value() - light.getPosition();
					const Ray lightray(light.getPosition(), light_dir / light_dir.norme());

					double dist_to_light = (p_i.value() - light.getPosition()).norme() - 0.001;

					std::optional<Vector3D> p_light = lightray.hit(sphere1);

					if (p_light.has_value() && (p_light.value() - light.getPosition()).norme() >= dist_to_light)
					{
						Vector3D norm = sphere1.normal(p_i.value());

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
	std::vector<Sphere> sphere_vec;
	sphere_vec.emplace_back(Vector3D(400, 300, 400), 100);

	const Light light1(Vector3D(200, -800, 0), 1000000);

	for (unsigned int x = 0; x < image_height; x++)
	{
		for (unsigned int y = 0; y < image_width; y++)
		{
			double red = 0;
			double green = 0;
			double blue = 0;

			const Ray r(Vector3D(x, y, 0), Vector3D(0, 0, 1));

			bool hit = false;
			Vector3D closest_hit(0,0,0);
			double squared_dist_from_closest_hit = std::numeric_limits<double>::infinity();
			Vector3D normal_at_closest_hit(0,0,0);

			for (const Sphere& sphere : sphere_vec)
			{
				std::optional<Vector3D> p_i = r.hit(sphere);

				if (p_i.has_value() && (p_i.value() - r.getOrigin()).normeSQ() < squared_dist_from_closest_hit)
				{
					hit = true;
					closest_hit = p_i.value();
					squared_dist_from_closest_hit = (p_i.value() - r.getOrigin()).normeSQ();
					normal_at_closest_hit = sphere.normal(p_i.value());
				}
			}
			
			Vector3D light_dir = closest_hit - light1.getPosition();

			const Ray lightray(light1.getPosition(), light_dir / light_dir.norme());

			double dist_to_light = (closest_hit - light1.getPosition()).norme() - 0.001;

			for (const Sphere& sphere : sphere_vec)
			{
				std::optional<Vector3D> p_light = lightray.hit(sphere);

				if (p_light.has_value() && (p_light.value() - light1.getPosition()).norme() >= dist_to_light)
				{
					double dp = dotProduct(normal_at_closest_hit, lightray.getDirection());		// == cos(angle)

					if (dp < 0)
					{
						dp = -dp;
					}

					red += light1.getIntensity() * dp / (dist_to_light * dist_to_light);
					green += light1.getIntensity() * dp / (dist_to_light * dist_to_light);
					blue += light1.getIntensity() * dp / (dist_to_light * dist_to_light);
				}
			}

			image.pixelColor(x, y, Magick::ColorRGB(red, green, blue));
		}
	}

	std::string filename = "output/illum_two.png";
	image.write(filename);
}
