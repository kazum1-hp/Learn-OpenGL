#pragma once
#include "Renderer.h"
#include "Camera.h"
#include "Window.h"
#include "InputManager.h"

class Application
{
private:
	Camera camera;
	Window window;
	Renderer renderer;
	InputManager input;
	bool running;

public:
	Application(const char* title);
	void run();
	~Application();
};

