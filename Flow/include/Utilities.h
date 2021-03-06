#pragma once
#include "stdafx.h"
#include <glad/glad.h>
#include <string>
#include <sstream>
#include <fstream>
#include <chrono>

namespace Flow
{
	class Utilities
	{
	public:
		FLOW_API static GLuint CompileShader(std::string filename, GLenum type); 
		FLOW_API static std::string LoadShaderFromFile(std::string filename);
		FLOW_API static int CheckShaderError(GLuint shader);
		FLOW_API static float LERP(float x0, float x1, float y0, float y1, float x);

		FLOW_API static void ResetClock();
		FLOW_API static double Clock();

	private:
		FLOW_API static std::chrono::high_resolution_clock::time_point last;
	};
}
