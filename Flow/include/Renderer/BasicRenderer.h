#pragma once
#include "Renderer\FluidRenderer.h"
#include "Utilities.h"

namespace Flow
{
	class BasicRenderer : public FluidRenderer
	{
	public:
		FLOW_API BasicRenderer(std::shared_ptr<FluidSystem> system);
		FLOW_API ~BasicRenderer();

		FLOW_API void Draw();

	private:

		const std::string vertexShaderFile_ = "basic_vertex.glsl";
		const std::string fragmentShaderFile_ = "basic_fragment.glsl";

		//Initialization methods
		void InitGL();

		//Render methods
		void PrepareGeometry();

		GLuint vbo_, vao_, vertexShader_, fragmentShader_, shaderProgram_, posAttrib_, colorAttrib_;
		std::vector<float> vertexData;
	};
}

