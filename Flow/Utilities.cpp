#include "Utilities.h"

const char * Flow::Utilities::LoadShaderFromFile(char * filename)
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
	const char* c_str = (char*)malloc(sizeof(char) * stream.str().length());
	memcpy((void*)c_str, stream.str().c_str(), sizeof(char) * (stream.str().length() + 4));
	return c_str;
}

float Flow::Utilities::LERP(float x0, float x1, float y0, float y1, float x)
{
	return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}
