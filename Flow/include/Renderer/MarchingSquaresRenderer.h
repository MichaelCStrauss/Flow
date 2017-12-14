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

	private:

		//Parameters
		const unsigned int Resolution = 200;
		const float ParticleRadius = 0.010; //this in sim terms

		const std::string vertexShaderFile_ = "marching_squares/vertex.glsl";
		const std::string geometryShaderFile_ = "marching_squares/geometry.glsl";
		const std::string fragmentShaderFile_ = "marching_squares/fragment.glsl";

		//Initialization methods
		void InitGL();

		//Render methods

		//For each point on the grid, evaluate the value of the metaballs
		void evaluateFieldValues(int beginning, int end);
		void preparePoints(int beginning, int end);

		//Private parameters
		int cellsX_, cellsY_;
		float cellW_, cellH_;

		//OpenGL
		GLuint vbo_, vao_, vertexShader_, geometryShader_, fragmentShader_,
			   shaderProgram_, posAttrib_, fieldAttrib_, colorAttrib_,
			   cellW_uniform_, cellH_uniform_;
		
		//simulation
		CopyBasedGrid grid_;
		

		//threads
		int num_threads_ = 4;
		int beginFlag_ = 0;
		std::atomic<bool> running_;
		std::atomic<unsigned int> threadStatus_;
		std::mutex statusMutex_;
		std::condition_variable condition_;
		std::vector<std::thread> threads_;
		void spawnThreads();
		void workerThread(int num, int mask);

		//timing information
		double fieldElapse_, pointsElapsed_, gridElapsed_;
		int frames_;

		//Data for the simulation
		std::vector<float> fieldValues_, renderData_;
	};
}

