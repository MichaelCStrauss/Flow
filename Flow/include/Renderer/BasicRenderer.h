#pragma once
#include <Renderer/FluidRenderer.h>
#include <Utilities.h>
#include <Renderer/Color.h>

namespace Flow
{
	class BasicRenderer : public FluidRenderer
	{
	public:
		FLOW_API BasicRenderer(std::shared_ptr<FluidSystem> system);
		FLOW_API ~BasicRenderer();

		FLOW_API void Draw();

		FLOW_API void setColorFunction(std::function<Color3f(Particle)> func);

		FLOW_API static Color3f PlainBlue(Particle p);
		FLOW_API static Color3f DensityGradient(Particle p);

	private:

		const std::string vertexShaderFile_ = "basic_vertex.glsl";
		const std::string fragmentShaderFile_ = "basic_fragment.glsl";

		//Initialization methods
		void InitGL();

		//Render methods
		void PrepareGeometry();

		GLuint vbo_, vao_, vertexShader_, fragmentShader_, shaderProgram_, posAttrib_, colorAttrib_;
		std::vector<float> vertexData;

		std::function<Color3f(Particle)> colorFunction_;
	};
}

