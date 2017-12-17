#include <Renderer/ThreadedMS.h>

using namespace std;

Flow::ThreadedMS::ThreadedMS(shared_ptr<FluidSystem> system) : MarchingSquaresRenderer(system)
{
	system_ = system;

	running_.store(true);
	spawnThreads();
}


Flow::ThreadedMS::~ThreadedMS()
{
	//Sign to running threads that the program is terminating
	running_.store(false);
	threadStatus_.store(beginFlag_);
	condition_.notify_all();
	for (auto &thread : threads_)
		thread.join();
}

void Flow::ThreadedMS::spawnThreads()
{
	int mask = 1;
	for (int i = 0; i < num_threads_; i++)
	{
		threads_.push_back(std::thread(&ThreadedMS::workerThread, this, i, mask));
		beginFlag_ += mask;
		mask = mask << 1;
	}
}

void Flow::ThreadedMS::workerThread(int num, int mask)
{
	std::cout << "Thread " << std::this_thread::get_id() << " started. Mask: " << mask << std::endl;

	int beginning = num * (cellsX_ / num_threads_);
	int end = (num + 1) * (cellsX_ / num_threads_);
	std:: cout << "b: " << beginning << " e: " << end << std::endl;

	while (running_.load())
	{
		std::unique_lock<std::mutex> lock(statusMutex_);
		condition_.wait(lock, [&status = this->threadStatus_, mask] {
					return (status.load() & mask) != 0;
				});

		lock.unlock();

		this->evaluateFieldValues(beginning, end);

		threadStatus_.store(threadStatus_ & (~mask));
		condition_.notify_all();
	}
}

void Flow::ThreadedMS::evaluateFieldValues(int beginning, int end)
{
	for (float i = beginning; i < end; i++)
	{
		for (float j = 0; j < cellsY_; j++)
		{
			auto baseIndex = (i * cellsY_ + j);
			float sim_x = i / (float)Resolution;
			float sim_y = j / (float)Resolution;

			auto positions = grid_.getPositions(sim_x, sim_y);
			float value = 0;
			for (auto pos : positions)
			{
				value += (ParticleRadius * ParticleRadius) / ((sim_x - pos.x) * (sim_x - pos.x) + (sim_y - pos.y) * (sim_y - pos.y));
			}
			fieldValues_[baseIndex] = value;
		}
	}
}

void Flow::ThreadedMS::preparePoints(int beginning, int end)
{
	for (float i = beginning; i < end; i++)
	{
		for (float j = 0; j < cellsY_; j++)
		{
			auto baseIndex = (i * cellsY_ + j);
			if (i == 0 || j == 0) {
				renderData_[6 * baseIndex] = -2;
				renderData_[6 * baseIndex + 1] = -2;
				continue;
			}
			float screen_x = i * cellW_ - 1.f + (0.5f / Resolution);
			float screen_y = j * cellH_ - 1.f + (0.5f / Resolution);
			renderData_[6 * baseIndex] = screen_x;
			renderData_[6 * baseIndex + 1] = screen_y;
			renderData_[6 * baseIndex + 2] = fieldValues_[baseIndex];
			renderData_[6 * baseIndex + 3] = fieldValues_[baseIndex - 1];
			renderData_[6 * baseIndex + 4] = fieldValues_[(i - 1) * cellsY_ + j];
			renderData_[6 * baseIndex + 5] = fieldValues_[(i - 1) * cellsY_ + j - 1];
		}
	}
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * renderData_.size(), &renderData_[0]);
}

void Flow::ThreadedMS::Draw()
{
	frames_++;
	Utilities::ResetClock();

	grid_.Update();
	gridElapsed_ += Utilities::Clock();

	bindBuffers();
	prepareGeometry();

	glUniform1f(cellW_uniform_, cellW_);
	glUniform1f(cellH_uniform_, cellH_);
	glDrawArrays(GL_POINTS, 0, renderData_.size() / 6);
}

void Flow::ThreadedMS::prepareGeometry()
{
	std::unique_lock<std::mutex> lock(statusMutex_);
	threadStatus_.store(beginFlag_);
	condition_.notify_all();

	condition_.wait(lock, [&status = this->threadStatus_] { return status.load() == 0; } );

	fieldElapse_ += Utilities::Clock();

	preparePoints(0, cellsX_);
	pointsElapsed_ += Utilities::Clock();
}
