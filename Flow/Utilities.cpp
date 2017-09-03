#include "Utilities.h"

std::string Flow::Utilities::LoadShaderFromFile(char * filename)
{
	std::ifstream file(filename);
	// If we couldn't open the file we'll bail out
	if ( !file.good() )
	{
		throw std::runtime_error("Failed to open file: " + std::string(filename));
	}

	std::stringstream stream;
	stream << file.rdbuf();
	file.close();
	return stream.str();
}

float Flow::Utilities::LERP(float x0, float x1, float y0, float y1, float x)
{
	return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}
