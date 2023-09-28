#include "Color.hpp"

// Constructor
Color::Color(double wavelength)
	: m_wavelength(wavelength)
{

}

// Accessors
double Color::getWavelength() const
{
	return m_wavelength;
}

void Color::setWavelength(double new_wavelength)
{
	m_wavelength = new_wavelength;
}
