#pragma once
#include "Renderer.h"

class Application
{
private:
	GLFWwindow* window;
	Renderer renderer;
	bool running;

public:
	Application(const char* title = "LearnOpenGL");
	void run();
	~Application();
};

