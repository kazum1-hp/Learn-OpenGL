#include "Window.h"
#include <iostream>


Window::Window(const char* title, Camera& cam, int width, int height)
	:camera(cam), input(cam), SCR_WIDTH(width), SCR_HEIGHT(height)
{
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW." << std::endl;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD." << std::endl;
	}

	glViewport(0, 0, static_cast<GLsizei>(SCR_WIDTH), static_cast<GLsizei>(SCR_HEIGHT));
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 捕获鼠标
	glfwSetScrollCallback(window, scroll_callback);

	// --- depth test ---
	glEnable(GL_DEPTH_TEST);
}

Window::~Window()
{
	
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (win)
	{
		win -> SCR_WIDTH = width;
		win -> SCR_HEIGHT = height;
		// 更新 camera 宽高比
		win -> camera.aspect = (float)width / (float)height;
	}
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (win)
		win->getInput().mouse_callback(window, xpos, ypos);
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (win)
		win->getInput().scroll_callback(window, xoffset, yoffset);
}