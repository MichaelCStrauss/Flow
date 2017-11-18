#include <Renderer/BasicRenderer.h>

using namespace std;

Flow::BasicRenderer::BasicRenderer(shared_ptr<FluidSystem> system) : FluidRenderer(system)
{
	system_ = system;
	InitGL();
}


Flow::BasicRenderer::~BasicRenderer()
{
}

void Flow::BasicRenderer::InitGL()
{
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	gladLoadGL();

	glEnable(GL_PROGRAM_POINT_SIZE);

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	//compile the two basic shaders
	auto vertexShaderSrc = Utilities::LoadShaderFromFile(vertexShaderFile_);
	auto _src = vertexShaderSrc.c_str();
	vertexShader_ = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader_, 1, &_src, NULL);
	glCompileShader(vertexShader_);
	if (!Utilities::CheckShaderError(vertexShader_))
		throw "error compiling vertex shader";

	auto fragmentShaderSrc = Utilities::LoadShaderFromFile(fragmentShaderFile_);
	_src = fragmentShaderSrc.c_str();
	fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader_, 1, &_src, NULL);
	glCompileShader(fragmentShader_);
	if (!Utilities::CheckShaderError(fragmentShader_))
		throw "error compiling fragment shader";

	shaderProgram_ = glCreateProgram();
	glAttachShader(shaderProgram_, vertexShader_);
	glAttachShader(shaderProgram_, fragmentShader_);
	glLinkProgram(shaderProgram_);

	posAttrib_ = glGetAttribLocation(shaderProgram_, "position");
	glEnableVertexAttribArray(posAttrib_);
	glVertexAttribPointer(posAttrib_, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	colorAttrib_ = glGetAttribLocation(shaderProgram_, "color");
	glEnableVertexAttribArray(colorAttrib_);
	glVertexAttribPointer(colorAttrib_, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2*sizeof(float)));

	glUseProgram(shaderProgram_);
}

void Flow::BasicRenderer::PrepareGeometry()
{
	vertexData.clear();

	for (auto p : *(system_)->Particles)
	{
		vertexData.push_back(0.f);
		vertexData.push_back(0.f);
		vertexData.push_back(0.f);
		vertexData.push_back(0.f);
		vertexData.push_back(1.0f);
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData.size(), &vertexData[0], GL_DYNAMIC_DRAW);
}

void Flow::BasicRenderer::Draw()
{
	glBindVertexArray(vao_);
	PrepareGeometry();
	glDrawArrays(GL_POINT, 0, system_->getParticles()->size());
}