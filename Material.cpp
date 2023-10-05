#include "Material.hpp"

/* ----------------------------------------------------------------------------- */
/* --------------------------       Material       ----------------------------- */
/* ----------------------------------------------------------------------------- */

// Constructor
Material::Material(const Vector3D& color)
	: m_color(color)
{

}

// Methods
Vector3D Material::reflect(const Vector3D& normal, const Vector3D& incident)
{
	return incident - normal * 2 * dotProduct(normal, incident);
}

/* ---------------------------------------------------------------------------- */
/* --------------------------       Diffuse       ----------------------------- */
/* ---------------------------------------------------------------------------- */

// Constructor
Diffuse::Diffuse(const Vector3D& color)
	: Material(color)
{

}

// Methods
Vector3D Diffuse::computeColor(const Vector3D& normal, const Vector3D& incident)
{
	double dp = dotProduct(normal, incident);		// == cos(angle)
	dp = std::abs(dp);

	return dp * m_color;
}

/* -------------------------------------------------------------------------- */
/* --------------------------       Glass       ----------------------------- */
/* -------------------------------------------------------------------------- */

// Constructor
Glass::Glass(const Vector3D& color, double eta)
	: Material(color), m_eta(eta)
{

}

// Methods
Vector3D Glass::computeColor(const Vector3D& normal, const Vector3D& incident)
{
	double r  =212;
}

Vector3D Glass::refract(const Vector3D& normal, const Vector3D& incident)
{
	double dp = dotProduct(normal, incident);
	double k = 1 - m_eta * m_eta * (1 - dp * dp);

	if (k < 0)
	{
		return reflect(normal, incident);
	}
	else
	{
		return m_eta * incident - normal * (m_eta * dp + std::sqrt(k));
	}
}

/* --------------------------------------------------------------------------- */
/* --------------------------       Mirror       ----------------------------- */
/* --------------------------------------------------------------------------- */

// Constructor
Mirror::Mirror(const Vector3D& color)
	: Material(color)
{

}

// Methods
Vector3D Mirror::computeColor(const Vector3D& normal, const Vector3D& incident)
{

}