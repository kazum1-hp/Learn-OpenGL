#include <iostream>
#include "Window.h"
#include "InputManager.h"

bool InputManager::firstMouse = true;
float InputManager::lastX = 0.0f;
float InputManager::lastY = 0.0f;

InputManager::InputManager(Camera& cam)
	:camera(cam) {
}

void InputManager::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = static_cast<float>(xpos);
		lastY = static_cast<float>(ypos);
		firstMouse = false;
	}

	float xoffset = static_cast<float>(xpos - lastX);
	float yoffset = static_cast<float>(lastY - ypos); // y-axis reverse
	lastX = static_cast<float>(xpos);
	lastY = static_cast<float>(ypos);

	// Get a Window instance
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (!win) {
		std::cerr << "Failed to get Window pointer in scroll_callback" << std::endl;
		return;
	}

	// Get InputManager through Window
	InputManager& input = win->getInput();
	input.camera.ProcessMouseMovement(xoffset, yoffset);
	
}

void InputManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (!win) {
		std::cerr << "Failed to get Window pointer in scroll_callback" << std::endl;
		return;
	}

	// Get InputManager through Window
	InputManager& input = win->getInput();
	input.camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void InputManager::moveControl(GLFWwindow* window, Camera& camera, float deltaTime)
{
	static float lastFrame = 0.0f;

	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	float speed = 50.0f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.MoveForward(speed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.MoveBackward(speed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.MoveLeft(speed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.MoveRight(speed);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.MoveUp(speed);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.MoveDown(speed);
}

void InputManager::lightControl(GLFWwindow* window)
{
	bool parallelPressed = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
	bool pointPressed = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;
	bool spotPressed = glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS;

	if (parallelPressed && !lastParallelKey)
		parallelLightOn = !parallelLightOn;
	if (pointPressed && !lastPointKey)
		pointLightOn = !pointLightOn;
	if (spotPressed && !lastSpotKey)
		spotLightOn = !spotLightOn;

	lastParallelKey = parallelPressed;
	lastPointKey = pointPressed;
	lastSpotKey = spotPressed;
}

void InputManager::closeWindow(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}