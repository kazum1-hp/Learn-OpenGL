#pragma once
#include <GLFW/glfw3.h>
#include "Camera.h"

class Window;

class InputManager
{
public:
	InputManager(Camera& cam);

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void process(GLFWwindow* window, Camera& camera, float deltaTime = 0.0f);
	void closeWindow(GLFWwindow* window);

private:
	static bool firstMouse;
	static float lastX, lastY;

	Camera& camera;
};

