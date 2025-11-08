#pragma once
#include <GLFW/glfw3.h>
#include "../external/imgui/imgui.h"
#include "Camera.h"

class Window;

class InputManager
{
public:
	InputManager(Camera& cam);

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	void cursorState(GLFWwindow* window);
	void moveControl(GLFWwindow* window, Camera& camera, float deltaTime = 0.0f);
	void lightControl(GLFWwindow* window);
	void closeWindow(GLFWwindow* window);
	void onImGuiRender();

	bool parallelLightOn = false;
	bool pointLightOn = false;
	bool spotLightOn = false;

private:
	static bool firstMouse;
	static float lastX, lastY;

	float moveSpeed = 50.0f;

	bool lastParallelKey = false;
	bool lastPointKey = false;
	bool lastSpotKey = false;

	bool cursorVisible = false;
	bool altPressed = false;
	bool mouseOverImGui = false;

	Camera& camera;
};

