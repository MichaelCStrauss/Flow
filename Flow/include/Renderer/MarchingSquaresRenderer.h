#pragma once
#include <Renderer/FluidRenderer.h>
#include <Utilities.h>
#include <Renderer/Color.h>

namespace Flow
{
	class MarchingSquaresRenderer : public FluidRenderer
	{
	public:
		FLOW_API MarchingSquaresRenderer(std::shared_ptr<FluidSystem> system);
		FLOW_API ~MarchingSquaresRenderer();

		FLOW_API void Draw();

	private:

		//Parameters
		const unsigned int Resolution = 50;
		const float ParticleRadius = 0.015; //this in sim terms

		const std::string vertexShaderFile_ = "marching_squares/vertex.glsl";
		const std::string geometryShaderFile_ = "marching_squares/geometry.glsl";
		const std::string fragmentShaderFile_ = "marching_squares/fragment.glsl";

		//Initialization methods
		void InitGL();

		//Render methods

		//For each point on the grid, evaluate the value of the metaballs
		void EvaluateGrid();

		//OpenGL
		GLuint vbo_, vao_, vertexShader_, geometryShader_, fragmentShader_,
			   shaderProgram_, posAttrib_, fieldAttrib_, colorAttrib_,
			   cellW_uniform_, cellH_uniform_;

		//Data for the simulation
		std::vector<float> fieldValues_, renderData_;
	};
}

