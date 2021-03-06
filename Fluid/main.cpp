#ifdef _WIN32
#define APIENTRY __stdcall
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Flow.h>
#include <chrono>

bool useBasic = false;
bool paused = true;

void error_callback(int error, const char* description)
{
	std::cout << "Error: %s\n" << description;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
		useBasic = !useBasic;
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		paused = !paused;
}

int main()
{
	std::cout << "Beginning Flow..." << std::endl;
	const int WindowWidth = 1800, WindowHeight = 900;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		std::cout << "Couldn't init GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);


	auto window = glfwCreateWindow(WindowWidth, WindowHeight, "Fluid", nullptr, nullptr); // Windowed
	if (!window)
	{
		std::cout << "window creation failed" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	if (!gladLoadGL())
		return -1;

	glEnable(GL_MULTISAMPLE);

	auto system = std::make_shared<Flow::FluidSystem>();
	system->Init();

	auto bRenderer = Flow::BasicRenderer(system);
	auto msRenderer = Flow::ThreadedMS(system);
	bRenderer.setColorFunction(Flow::BasicRenderer::DensityGradient);

	glfwSetKeyCallback(window, key_callback);

	float deltaTime = 0, t = 0;
	int frames = 0;

	while (!glfwWindowShouldClose(window))
	{
		auto start = std::chrono::high_resolution_clock::now();
		glfwPollEvents();

		if (!paused)
			system->Update(deltaTime);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		if (useBasic)
			bRenderer.Draw();
		else
			msRenderer.Draw();

		glfwSwapBuffers(window);

		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		deltaTime = elapsed_seconds.count();

		//If running in debug, run at constant timestep
		t += deltaTime;
		frames++;
		if (t > 1)
		{
			t = 0;
			std::cout << frames << std::endl;
			frames = 0;
		}
	}

	glfwTerminate();

	return 0;
}
