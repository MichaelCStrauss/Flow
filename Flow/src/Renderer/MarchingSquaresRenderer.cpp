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

	fieldValues_ = std::vector<float>(cellsX_ * cellsY_);
	renderData_ = std::vector<float>(6 * cellsX_ * cellsY_);
	initGL();
}


Flow::MarchingSquaresRenderer::~MarchingSquaresRenderer()
{
	glDeleteProgram(shaderProgram_);
	glDeleteShader(vertexShader_);
	glDeleteShader(fragmentShader_);
	glDeleteBuffers(1, &vbo_);
	glDeleteVertexArrays(1, &vao_);

	std::cout << "Average Renderer Grid Evaluation Time: " << gridElapsed_ / (double)frames_ << std::endl;
	std::cout << "Average Field Evaluation Time: " << fieldElapse_ / (double)frames_ << std::endl;
	std::cout << "Average Points Preparation Time: " << pointsElapsed_ / (double)frames_ << std::endl;
}

void Flow::MarchingSquaresRenderer::initGL()
{
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	gladLoadGL();

	generateBuffers();
	compileShaders();
	linkShaders();
	enableAttributes();

	glUseProgram(shaderProgram_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * renderData_.size(), &renderData_[0], GL_STREAM_DRAW);
}

void Flow::MarchingSquaresRenderer::generateBuffers()
{
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
}

void Flow::MarchingSquaresRenderer::compileShaders()
{
	//compile the three basic shaders
	vertexShader_ = Utilities::CompileShader(vertexShaderFile_, GL_VERTEX_SHADER);
	geometryShader_ = Utilities::CompileShader(geometryShaderFile_, GL_GEOMETRY_SHADER);
	fragmentShader_ = Utilities::CompileShader(fragmentShaderFile_, GL_FRAGMENT_SHADER);
}

void Flow::MarchingSquaresRenderer::linkShaders()
{
	shaderProgram_ = glCreateProgram();
	glAttachShader(shaderProgram_, vertexShader_);
	glAttachShader(shaderProgram_, geometryShader_);
	glAttachShader(shaderProgram_, fragmentShader_);
	glBindFragDataLocation(shaderProgram_, 0, "outColor");
	glLinkProgram(shaderProgram_);
}

void Flow::MarchingSquaresRenderer::enableAttributes()
{
	posAttrib_ = glGetAttribLocation(shaderProgram_, "position");
	glEnableVertexAttribArray(posAttrib_);
	glVertexAttribPointer(posAttrib_, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	fieldAttrib_ = glGetAttribLocation(shaderProgram_, "field");
	glEnableVertexAttribArray(fieldAttrib_);
	glVertexAttribPointer(fieldAttrib_, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2*sizeof(float)));

	cellW_uniform_ = glGetUniformLocation(shaderProgram_, "cellW");
	cellH_uniform_ = glGetUniformLocation(shaderProgram_, "cellH");
}

void Flow::MarchingSquaresRenderer::Draw()
{
	frames_++;
	Utilities::ResetClock();

	grid_.Update();
	gridElapsed_ += Utilities::Clock();

	bindBuffers();
	prepareGeometry();

	glUniform1f(cellW_uniform_, cellW_);
	glUniform1f(cellH_uniform_, cellH_);
	glDrawArrays(GL_POINTS, 0, cellsX_ * cellsY_);
}

void Flow::MarchingSquaresRenderer::bindBuffers()
{
	glBindVertexArray(vao_);
	glUseProgram(shaderProgram_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
}

void Flow::MarchingSquaresRenderer::prepareGeometry()
{
}
