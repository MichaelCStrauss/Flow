#pragma once
#include <Renderer/MarchingSquaresRenderer.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace Flow
{
	class ThreadedMS : public MarchingSquaresRenderer
	{
	public:
		FLOW_API ThreadedMS(std::shared_ptr<FluidSystem> system);
		FLOW_API ~ThreadedMS();

		FLOW_API void Draw();

	private:

		//Parameters
		const unsigned int Resolution = 200;
		const float ParticleRadius = 0.010; //this in sim terms

		const std::string vertexShaderFile_ = "marching_squares/vertex.glsl";
		const std::string geometryShaderFile_ = "marching_squares/geometry.glsl";
		const std::string fragmentShaderFile_ = "marching_squares/fragment.glsl";

		void prepareGeometry();

		//For each point on the grid, evaluate the value of the metaballs
		void evaluateFieldValues(int beginning, int end);
		void preparePoints(int beginning, int end);

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
	};
}

