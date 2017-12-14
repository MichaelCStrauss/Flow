#include <Renderer/MarchingSquaresRenderer.h>

using namespace std;

Flow::MarchingSquaresRenderer::MarchingSquaresRenderer(shared_ptr<FluidSystem> system) : FluidRenderer(system)
{
	system_ = system;
	grid_ = CopyBasedGrid(system_, 1 / system_->h);
	cellsX_ = (float)Resolution * system_->Width;
	cellsY_ = (float)Resolution * system_->Height;
	cellW_  = 2.f / (float)cellsX_;
	cellH_  = 2.f / (float)cellsY_;

	InitGL();

	fieldValues_ = std::vector<float>(cellsX_ * cellsY_);
	renderData_ = std::vector<float>(6 * cellsX_ * cellsY_);

	running_.store(true);
	spawnThreads();
}


Flow::MarchingSquaresRenderer::~MarchingSquaresRenderer()
{
	//Sign to running threads that the program is terminating
	running_.store(false);
	threadStatus_.store(beginFlag_);
	condition_.notify_all();
	for (auto &thread : threads_)
		thread.join();


	glDeleteProgram(shaderProgram_);
	glDeleteShader(vertexShader_);
	glDeleteShader(fragmentShader_);
	glDeleteBuffers(1, &vbo_);
	glDeleteVertexArrays(1, &vao_);

	std::cout << "Average Renderer Grid Evaluation Time: " << gridElapsed_ / (double)frames_ << std::endl;
	std::cout << "Average Field Evaluation Time: " << fieldElapse_ / (double)frames_ << std::endl;
	std::cout << "Average Points Preparation Time: " << pointsElapsed_ / (double)frames_ << std::endl;
}

void Flow::MarchingSquaresRenderer::InitGL()
{
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	gladLoadGL();

	glEnable(GL_PROGRAM_POINT_SIZE);

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	//compile the three basic shaders
	auto vertexShaderSrc = Utilities::LoadShaderFromFile(vertexShaderFile_);
	auto _src = vertexShaderSrc.c_str();
	vertexShader_ = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader_, 1, &_src, NULL);
	glCompileShader(vertexShader_);
	if (!Utilities::CheckShaderError(vertexShader_))
		throw "error compiling vertex shader";

	auto geometryShaderSrc = Utilities::LoadShaderFromFile(geometryShaderFile_);
	_src = geometryShaderSrc.c_str();
	geometryShader_ = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometryShader_, 1, &_src, NULL);
	glCompileShader(geometryShader_);
	if (!Utilities::CheckShaderError(geometryShader_))
		throw "error compiling fragment shader";

	auto fragmentShaderSrc = Utilities::LoadShaderFromFile(fragmentShaderFile_);
	_src = fragmentShaderSrc.c_str();
	fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader_, 1, &_src, NULL);
	glCompileShader(fragmentShader_);
	if (!Utilities::CheckShaderError(fragmentShader_))
		throw "error compiling fragment shader";

	shaderProgram_ = glCreateProgram();
	glAttachShader(shaderProgram_, vertexShader_);
	glAttachShader(shaderProgram_, geometryShader_);
	glAttachShader(shaderProgram_, fragmentShader_);
	glBindFragDataLocation(shaderProgram_, 0, "outColor");
	glLinkProgram(shaderProgram_);

	posAttrib_ = glGetAttribLocation(shaderProgram_, "position");
	glEnableVertexAttribArray(posAttrib_);
	glVertexAttribPointer(posAttrib_, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	fieldAttrib_ = glGetAttribLocation(shaderProgram_, "field");
	glEnableVertexAttribArray(fieldAttrib_);
	glVertexAttribPointer(fieldAttrib_, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2*sizeof(float)));

	cellW_uniform_ = glGetUniformLocation(shaderProgram_, "cellW");

	cellH_uniform_ = glGetUniformLocation(shaderProgram_, "cellH");

	glUseProgram(shaderProgram_);
}

void Flow::MarchingSquaresRenderer::spawnThreads()
{
	int mask = 1;
	for (int i = 0; i < num_threads_; i++)
	{
		threads_.push_back(std::thread(&MarchingSquaresRenderer::workerThread, this, i, mask));
		beginFlag_ += mask;
		mask = mask << 1;
	}
}

void Flow::MarchingSquaresRenderer::workerThread(int num, int mask)
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

void Flow::MarchingSquaresRenderer::evaluateFieldValues(int beginning, int end)
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

void Flow::MarchingSquaresRenderer::preparePoints(int beginning, int end)
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * renderData_.size(), &renderData_[0], GL_STREAM_DRAW);
}

void Flow::MarchingSquaresRenderer::Draw()
{
	glBindVertexArray(vao_);
	glUseProgram(shaderProgram_);
	glUniform1f(cellW_uniform_, cellW_);
	glUniform1f(cellH_uniform_, cellH_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	auto start = std::chrono::high_resolution_clock::now();

	grid_.Update();
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> s = end - start;
	gridElapsed_ += s.count();
	start = std::chrono::high_resolution_clock::now();

	std::unique_lock<std::mutex> lock(statusMutex_);
	threadStatus_.store(beginFlag_);
	condition_.notify_all();

	condition_.wait(lock, [&status = this->threadStatus_] { return status.load() == 0; } );

	end = std::chrono::high_resolution_clock::now();
	s = end - start;
	fieldElapse_ += s.count();
	start = std::chrono::high_resolution_clock::now();

	preparePoints(0, cellsX_);
	end = std::chrono::high_resolution_clock::now();
	s = end - start;
	pointsElapsed_ += s.count();
	frames_++;

	glDrawArrays(GL_POINTS, 0, renderData_.size() / 6);
}
