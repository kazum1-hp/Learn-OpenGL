#include "Application.h"

Application::Application(const char* title)
	: window(initWindow(title)), running(true)
{
	if (!window)
	{
		running = false;
		return;
	}
}

void Application::run()
{
	if (!running) return;

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// Clear screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //custom color for screen clean
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //buffer clear

		renderer.render();

		// Swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

Application::~Application()
{
	glfwTerminate(); //release resources
}