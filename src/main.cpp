#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "Window.h"

constexpr auto RESOLUTION_WIDTH  = 160;
constexpr auto RESOLUTION_HEIGHT = 144;
constexpr auto WINDOW_SCALING    = 5;

oter::Game game{ RESOLUTION_WIDTH, RESOLUTION_HEIGHT };

int main()
{
	// Set up logging
	spdlog::set_pattern("%^[%T.%e] <%l>%$ %v");
	spdlog::set_level(spdlog::level::level_enum::debug);

	// Set up GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const oter::Window window{ RESOLUTION_WIDTH * WINDOW_SCALING, RESOLUTION_HEIGHT * WINDOW_SCALING, "Oracles" };
	window.MakeCurrent();
	oter::Window::SetGame(&game);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		spdlog::critical("Failed to initialize GLAD");
		glfwTerminate();
		return -1;
	}

	game.Init();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	constexpr oter::f64 maxFPS    = 60.0;
	constexpr oter::f64 maxPeriod = 1.0 / maxFPS;

	oter::f64 lastFrame = 0.0;
	while (!window.IsClosing())
	{
		const oter::f64 currentFrame = glfwGetTime();
		const oter::f64 deltaTime    = currentFrame - lastFrame;
		const oter::f64 deltaTimeMs  = deltaTime * 1000.0;
		if (deltaTime < maxPeriod)
			continue;
		lastFrame = currentFrame;
		glfwPollEvents();

		game.ProcessInput(deltaTimeMs);
		game.Update(deltaTimeMs);
		game.Render();

		glViewport(0, 0, RESOLUTION_WIDTH * WINDOW_SCALING, RESOLUTION_HEIGHT * WINDOW_SCALING);
		window.Clear(0.2f, 0.3f, 0.3f);
		game.Draw();
		window.Display();
	}

	glfwTerminate();
	return 0;
}
