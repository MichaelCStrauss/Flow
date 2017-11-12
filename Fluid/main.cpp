#ifdef _WIN32
    #define APIENTRY __stdcall
#endif

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <SFML\System.hpp>
#include <Flow.h>

int main()
{
	const int Scale = 800;
	const int WindowWidth = 1000, WindowHeight = 666;

	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);


	auto window = glfwCreateWindow(WindowWidth, WindowHeight, "Fluid", nullptr, nullptr); // Windowed
	if (!window)
	{
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
	auto particles = system->Particles;

	auto renderer = Flow::BasicRenderer(system);

	sf::Clock deltaClock; 
	int frames = 0;
	float t = 0;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		//If running in debug, run at constant timestep
#ifdef NDEBUG
		float time = deltaClock.restart().asSeconds();
		t += time;
#else
		float time = 0.015;
		t += deltaClock.restart().asSeconds();
#endif
		frames++;
		if (t > 1)
		{
			t = 0;
			std::cout << frames << std::endl;
			frames = 0;
		}
		system->Update(time);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		renderer.Draw();

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}