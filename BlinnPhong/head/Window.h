#pragma once
#include <glad/glad.h>
#include "InputManager.h"
#include "Camera.h"

class Window
{
public:
	Window(const char* title, Camera& cam, InputManager& input, int width = 1920, int height = 1080);
	~Window();

	GLFWwindow* getWindow() const {
		return window;
	}

	InputManager& getInput() { return input; }

	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	int getWidth() const { return SCR_WIDTH; }
	int getHeight() const { return SCR_HEIGHT; }

private:
	GLFWwindow* window;
	int SCR_WIDTH;
	int SCR_HEIGHT;
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	InputManager& input;
	Camera& camera;
};

