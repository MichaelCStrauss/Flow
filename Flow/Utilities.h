#pragma once
#include "dll.h"
#include <sstream>
#include <fstream>

namespace Flow
{
	class Utilities
	{
	public:
		FLOW_API static const char* LoadShaderFromFile(char* filename);
		FLOW_API static float LERP(float x0, float x1, float y0, float y1, float x);
	};
}