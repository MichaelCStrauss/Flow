#include <Renderer/MarchingSquaresRenderer.h>

using namespace std;

Flow::MarchingSquaresRenderer::MarchingSquaresRenderer(shared_ptr<FluidSystem> system) : FluidRenderer(system)
{
	system_ = system;
	InitGL();
}


Flow::MarchingSquaresRenderer::~MarchingSquaresRenderer()
{
	glDeleteProgram(shaderProgram_);
	glDeleteShader(vertexShader_);
	glDeleteShader(fragmentShader_);
	glDeleteBuffers(1, &vbo_);
	glDeleteVertexArrays(1, &vao_);
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

	glUseProgram(shaderProgram_);
}

void Flow::MarchingSquaresRenderer::EvaluateGrid()
{
	fieldValues_.clear();
	fieldValues_.reserve(Resolution * Resolution);
	renderData_.clear();
	renderData_.reserve(6 * Resolution * Resolution);

	for (int i = 0; i < system_->Width * Resolution; i++)
	{
		for (int j = 0; j < system_->Height * Resolution; j++)
		{
			float sim_x = (float)i / Resolution;
			float sim_y = (float)j / Resolution;

			auto indices = system_->GetNearbyParticles(sim_x, sim_y);
			float value = 0;
			for (auto index : indices)
			{
				auto particle = (*system_->getParticles())[index];
				value += (ParticleRadius * ParticleRadius) / (pow(sim_x - particle.Position.x, 2) + pow(sim_y - particle.Position.y, 2));
			}
			fieldValues_.push_back(value);

			if (i == 0 || j == 0) continue;

			float screen_x = 2.f * sim_x / system_->Width - 1.f;
			float screen_y = 2.f * sim_y / system_->Height - 1.f;
			renderData_.push_back(screen_x);
			renderData_.push_back(screen_y);
			renderData_.push_back(
					fieldValues_[i * system_->Height * Resolution + j]);
			renderData_.push_back(
					fieldValues_[i * system_->Height * Resolution + j - 1]);
			renderData_.push_back(
					fieldValues_[(i - 1) * system_->Height * Resolution + j]);
			renderData_.push_back(
					fieldValues_[(i - 1) * system_->Height * Resolution + j - 1]);
		}
	}
	std::cout << renderData_.size() << std::endl;
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * renderData_.size(), &renderData_[0], GL_STREAM_DRAW);
}

void Flow::MarchingSquaresRenderer::Draw()
{
	glBindVertexArray(vao_);
	EvaluateGrid();
	glDrawArrays(GL_POINTS, 0, system_->getParticles()->size());
}

