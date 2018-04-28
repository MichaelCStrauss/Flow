#pragma once
#include <thread>
#include <chrono>
#include <Renderer/FluidRenderer.h>
#include <Utilities.h>
#include <Renderer/Color.h>
#include <Simulation/CopyBasedGrid.h>

namespace Flow
{
	class MarchingSquaresRenderer : public FluidRenderer
	{
	public:
		FLOW_API MarchingSquaresRenderer(std::shared_ptr<FluidSystem> system);
		FLOW_API ~MarchingSquaresRenderer();

		FLOW_API void Draw();

	protected:

		//Parameters
		const unsigned int Resolution = 200;
		const float ParticleRadius = 0.015; //this in sim terms

		const std::string vertexShaderFile_ = "marching_squares/vertex.glsl";
		const std::string geometryShaderFile_ = "marching_squares/geometry.glsl";
		const std::string fragmentShaderFile_ = "marching_squares/fragment.glsl";

		//Initialization methods
		void initGL();
		void generateBuffers();
		void compileShaders();
		void linkShaders();
		void enableAttributes();

		//Render methods
		void bindBuffers();
		void prepareGeometry();

		//clean up
		void deleteShaders();
		void deleteBuffers();

		//Private parameters
		int cellsX_, cellsY_;
		float cellW_, cellH_;

		//OpenGL
		GLuint vbo_, vao_, vertexShader_, geometryShader_, fragmentShader_,
			   shaderProgram_, posAttrib_, fieldAttrib_, colorAttrib_,
			   cellW_uniform_, cellH_uniform_;
		
		//simulation
		CopyBasedGrid grid_;

		//timing information
		double fieldElapse_, pointsElapsed_, gridElapsed_;
		int frames_;

		//Data for the simulation
		std::vector<float> fieldValues_, renderData_;
	};
}

