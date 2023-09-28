#pragma once

class Color
{
public:
	// Constructor
	Color(double wavelength);

	// Accessors
	double getWavelength() const;
	void setWavelength(double new_wavelength);

private:
	double m_wavelength;	// in nm
};