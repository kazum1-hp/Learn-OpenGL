#include "Application.h"

Application::Application(const char* title)
	: camera(),
	  window(title, camera, input),  
	  input(camera),
	  renderer(camera, input, { "Assets/raiden/raiden.pmx" }),
	  running(true)
{
	if (!window.getWindow())
	{
		running = false;
		return;
	}
}

void Application::run()
{
	if (!running) return;

	while (!glfwWindowShouldClose(window.getWindow())) {
		input.moveControl(window.getWindow(), camera);
		input.lightControl(window.getWindow());

		// Clear screen
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //custom color for screen clean
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //buffer clear

		renderer.render();

		// Swap buffers and poll IO events
		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();

		input.closeWindow(window.getWindow());
	}
}

Application::~Application()
{
	glfwTerminate(); //release resources
}