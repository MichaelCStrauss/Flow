#include <Renderer/MarchingSquaresRenderer.h>

using namespace std;

Flow::MarchingSquaresRenderer::MarchingSquaresRenderer(shared_ptr<FluidSystem> system) : FluidRenderer(system)
{
	system_ = system;
	InitGL();
	fieldValues_ = std::vector<float>(Resolution * Resolution * system_->Width * system_->Height);
	renderData_ = std::vector<float>(6 * Resolution * Resolution * system_->Width * system_->Height);
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
	for (float i = 0; i < system_->Width * (float)Resolution; i++)
	{
		for (float j = 0; j < system_->Height * (float)Resolution; j++)
		{
			auto baseIndex = (i * system_->Height * (float)Resolution + j);
			float sim_x = i / (float)Resolution;
			float sim_y = j / (float)Resolution;

			auto indices = system_->GetNearbyParticles(sim_x, sim_y);
			float value = 0;
			for (auto index : indices)
			{
				auto particle = (*system_->getParticles())[index];
				value += (ParticleRadius * ParticleRadius) / (pow(sim_x - particle.Position.x, 2) + pow(sim_y - particle.Position.y, 2));
			}
			fieldValues_[baseIndex] = value;

			float screen_x = 2.f * sim_x / system_->Width - 1.f + (0.5f / Resolution);
			float screen_y = 2.f * sim_y / system_->Width - 1.f + (0.5f / Resolution);
			renderData_[6 * baseIndex] = screen_x;
			renderData_[6 * baseIndex + 1] = screen_y;
			renderData_[6 * baseIndex + 2] = fieldValues_[baseIndex];
			renderData_[6 * baseIndex + 3] = fieldValues_[i * system_->Height * (float)Resolution + j - 1];
			renderData_[6 * baseIndex + 4] = fieldValues_[(i - 1) * system_->Height * (float)Resolution + j];
			renderData_[6 * baseIndex + 5] = fieldValues_[(i - 1) * system_->Height * (float)Resolution + j - 1];
		}
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * renderData_.size(), &renderData_[0], GL_STREAM_DRAW);
}

void Flow::MarchingSquaresRenderer::Draw()
{
	glBindVertexArray(vao_);
	glUseProgram(shaderProgram_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	EvaluateGrid();
	glDrawArrays(GL_POINTS, 0, renderData_.size() / 6);
}

