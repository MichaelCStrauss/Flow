#include <SFML/Graphics.hpp>
#include <Flow.h>

int main()
{
	const int WindowWidth = 600, WindowHeight = 600;

	sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "SFML works!");
	sf::Clock deltaClock;

	auto system = Flow::FluidSystem();
	system.Init();
	auto particles = system.Particles;

	int frames = 0;
	float t = 0;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

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
		system.Update(time);

		window.clear();

		for (auto p : *particles)
		{
			auto circle = sf::CircleShape(5);
			circle.setFillColor(sf::Color::Blue);
			circle.setPosition(
				sf::Vector2f(p.Position.x * WindowWidth - 5, WindowHeight * (1.f - p.Position.y) - 5));

			window.draw(circle);
		}

		window.display();
	}

	return 0;
}