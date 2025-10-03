#include "GLUtils.h"
#include <iostream>

int main() {
	GLFWwindow* window = initWindow("LearnOpenGL"); //initialize window
	if (!window) return -1;

	// ===== write code here ===== //



	// ===== Render Loop =====
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// Clear screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //custom color for screen clean
		glClear(GL_COLOR_BUFFER_BIT); //buffer clear

		// Draw 


		// Swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate(); //release resources
	return 0;
}