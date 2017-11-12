#include "Utilities.h"

std::string Flow::Utilities::LoadShaderFromFile(std::string filename)
{
	std::ifstream file(filename);
	// If we couldn't open the file we'll bail out
	if ( !file.good() )
	{
		throw std::runtime_error("Failed to open file: " + filename);
	}

	std::stringstream stream;
	stream << file.rdbuf();
	file.close();
	return stream.str();
}

int Flow::Utilities::CheckShaderError(GLuint shader)
{
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		glGetShaderInfoLog(shader, 512, NULL, buffer);
		std::cout << buffer << std::endl;
		return 0;
	}
	return 1;
}

float Flow::Utilities::LERP(float x0, float x1, float y0, float y1, float x)
{
	return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}
