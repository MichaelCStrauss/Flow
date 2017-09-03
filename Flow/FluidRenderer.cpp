#include "FluidRenderer.h"

using namespace Flow;
using namespace std;

FluidRenderer::FluidRenderer()
{
}


FluidRenderer::~FluidRenderer()
{
}

void FluidRenderer::Init(shared_ptr<FluidSystem> system)
{
	System = system;
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	gladLoadGL();

	InitGeometry();
	InitShaders();

	CellsX = System->Width * Resolution;
	CellsY = System->Height * Resolution;

	CellW = 2 / ((float)CellsX - 1);
	CellH = 2 / ((float)CellsY - 1);

	return;
}

void FluidRenderer::InitGeometry()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	float _vertices[] = {
	 0.0f,  0.5f,  0.5f,  0.5f,  0.5f, // Vertex 1 (X, Y)
	 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, // Vertex 2 (X, Y)
	-0.5f, -0.5f, 0.5f, 0.5f, 0.5f  // Vertex 3 (X, Y)
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);
}

void FluidRenderer::InitShaders()
{
	auto vert_shader_src = Utilities::LoadShaderFromFile("basic_vertex.glsl");
	auto frag_shader_src = Utilities::LoadShaderFromFile("basic_fragment.glsl");

	auto c_str = vert_shader_src.c_str();
	auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &c_str, NULL);
	glCompileShader(vertexShader);
	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		std::cout << buffer << std::endl;
		exit(-1);
	}

	c_str = frag_shader_src.c_str();
	auto fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &c_str, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		glGetShaderInfoLog(fragShader, 512, NULL, buffer);
		std::cout << buffer << std::endl;
		exit(-1);
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);

	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);
	//GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	//glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2 * sizeof(float)));
	//glEnableVertexAttribArray(colAttrib);
}

void FluidRenderer::Draw()
{
	UpdateGeometry();
	glDrawArrays(GL_TRIANGLES, 0, (int)(vertices.size() / 2));

	return;
}

void FluidRenderer::UpdateGeometry()
{
	vertices = std::vector<float>(); //empty vector with enough reserved memory
	vertices.reserve((CellsX) * (CellsY) * 2);

	// pre filled vector
	cells = std::vector<float>((CellsX) * (CellsY));

	for (int i = 0; i < CellsX; i++)
	{
		for (int j = 0; j < CellsY; j++)
		{
			float screen_x = ((float)i * CellW) - 1.0f;
			float screen_y = ((float)j * CellH) - 1.0f;

			//evaluate the value of the field and store it
			auto sim_pos = ScreenCoordsToSim(Vector2f(screen_x, screen_y));
			auto value = EvaluateField(System->GetNearbyParticles(sim_pos.x, sim_pos.y), sim_pos.x, sim_pos.y);
			cells[i * CellsY + j] = value;

			//if we're on an edge, continue
			if (i == 0 || j == 0)
				continue;

			//the value of the field at the corners
			auto fieldNE = cells[(i)* CellsY + j];
			auto fieldNW = cells[(i - 1) * CellsY + j];
			auto fieldSW = cells[(i - 1) * CellsY + j - 1];
			auto fieldSE = cells[(i)* CellsY + j - 1];

			//the configuration of the desired geometry
			int configuration = 0;
			configuration += fieldSW >= 1;
			configuration += 2 * (fieldSE >= 1);
			configuration += 4 * (fieldNE >= 1);
			configuration += 8 * (fieldNW >= 1);

			AddVertices(configuration, screen_x, screen_y, i, j);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
}

float FluidRenderer::EvaluateField(vector<int> &indices, float x, float y)
{
	if (indices.size() == 0)
		return 0;

	auto particles = System->getParticles();
	float value = 0;
	for (auto index : indices)
	{
		auto particle = (*particles)[index];
		value += (ParticleRadius * ParticleRadius) / (pow(x - particle.Position.x, 2) + pow(y - particle.Position.y, 2));
	}
	return value;
}

void FluidRenderer::AddVertices(int configuration, float screen_x, float screen_y, int i, int j)
{
	//the value of the field at the corners
	auto fieldNE = cells[(i)* CellsY + j];
	auto fieldNW = cells[(i - 1) * CellsY + j];
	auto fieldSW = cells[(i - 1) * CellsY + j - 1];
	auto fieldSE = cells[(i)* CellsY + j - 1];

	Vector2f N, NE, E, SE, S, SW, W, NW;
	N = Vector2f(screen_x - 0.5 * CellW, screen_y);
	NE = Vector2f(screen_x, screen_y);
	E = Vector2f(screen_x, screen_y - 0.5 * CellH);
	SE = Vector2f(screen_x, screen_y - CellH);
	S = Vector2f(screen_x - 0.5 * CellW, screen_y - CellH);
	SW = Vector2f(screen_x - CellW, screen_y - CellH);
	W = Vector2f(screen_x - CellW, screen_y - 0.5 * CellH);
	NW = Vector2f(screen_x - CellW, screen_y);

	if ((configuration & 1) != (configuration & 2))
		S.x = Utilities::LERP(fieldSW, fieldSE, SW.x, SE.x, 1);
	if ((configuration & 1) != (configuration & 8))
		W.y = Utilities::LERP(fieldSW, fieldNW, SW.y, NW.y, 1);
	if ((configuration & 2) != (configuration & 4))
		E.y = Utilities::LERP(fieldSE, fieldNE, SE.y, NE.y, 1);
	if ((configuration & 4) != (configuration & 8))
		N.x = Utilities::LERP(fieldNW, fieldNE, NW.x, NE.x, 1);

	switch (configuration)
	{
	case 0:
		break;
	case 1:
		AddVert(W);
		AddVert(S);
		AddVert(SW);
		break;
	case 2:
		AddVert(SE);
		AddVert(S);
		AddVert(E); //bottom right
		break;
	case 3:
		AddVert(E);
		AddVert(W);
		AddVert(SW); //bottom left

		AddVert(E);
		AddVert(SE); //bottom left
		AddVert(SW); //bottom right
		break;
	case 4:
		AddVert(NE);
		AddVert(E);
		AddVert(N); //top right
		break;
	case 5: //needs saddle detection
		AddVert(SW); //bottom left
		AddVert(W);
		AddVert(S);

		AddVert(W);
		AddVert(N);
		AddVert(S);

		AddVert(S);
		AddVert(E);
		AddVert(N);

		AddVert(NE); //top right
		AddVert(N);
		AddVert(E);
		break;
	case 6:
		AddVert(N);
		AddVert(S);
		AddVert(NE); //top right

		AddVert(NE);
		AddVert(SE);
		AddVert(S); //bottom right
		break;
	case 7:
		AddVert(W);
		AddVert(SW); //bottom left
		AddVert(SE); //bottom right

		AddVert(W);
		AddVert(N);
		AddVert(SE); //bottom right

		AddVert(N);
		AddVert(NE); //top right
		AddVert(SE); //bottom right
		break;
	case 8:
		AddVert(NW);
		AddVert(W);
		AddVert(N); //top left
		break;
	case 9:
		AddVert(N);
		AddVert(S);
		AddVert(NW); //top left

		AddVert(NW);
		AddVert(SW);
		AddVert(S); //bottom left
		break;
	case 10:
		AddVert(NW);
		AddVert(N);
		AddVert(W); //bottom right

		AddVert(N);
		AddVert(W);
		AddVert(S);

		AddVert(S);
		AddVert(E);
		AddVert(N);

		AddVert(SE);
		AddVert(S);
		AddVert(E); //top left
		break;
	case 11:
		AddVert(NW); //top left
		AddVert(SW); //bottom left
		AddVert(N);

		AddVert(SW);
		AddVert(N);
		AddVert(E); //bottom left

		AddVert(SW);
		AddVert(E); //bottom right
		AddVert(SE); //bottom left
		break;
	case 12:
		AddVert(E);
		AddVert(W);
		AddVert(NE); //bottom left

		AddVert(NW);
		AddVert(NE);
		AddVert(W); //bottom right
		break;
	case 13:
		AddVert(NE); //top left
		AddVert(E); //bottom left
		AddVert(NW); //middle bottom

		AddVert(NW);
		AddVert(E);
		AddVert(S); //top left

		AddVert(S);
		AddVert(SW); //top left
		AddVert(NW); //top right
		break;
	case 14:
		AddVert(NE); //top left
		AddVert(NW); //top right
		AddVert(W); //middle left

		AddVert(NE); //middle left
		AddVert(W); //middle bottom
		AddVert(S); //top right

		AddVert(S); //middle bottom
		AddVert(NE); //top right
		AddVert(SE); //bottom right
		break;
	case 15:
		AddVert(NE); //top right
		AddVert(NW); //top left
		AddVert(SE); //bottom right


		AddVert(SE); //bottom right
		AddVert(SW); //bottom left
		AddVert(NW); //top left
		break;
	}
}

Vector2f Flow::FluidRenderer::ScreenCoordsToSim(Vector2f screen)
{
	auto sim = screen + Vector2f(1, 1);
	sim *= 0.5f;
	sim.x *= System->Width;
	sim.y *= System->Height;
	return sim;
}

inline void FluidRenderer::AddVert(Vector2f pos)
{
	vertices.push_back(pos.x);
	vertices.push_back(pos.y);
}

inline void FluidRenderer::AddVert(float x, float y)
{
	vertices.push_back(x);
	vertices.push_back(y);
}

void FluidRenderer::End()
{
}

