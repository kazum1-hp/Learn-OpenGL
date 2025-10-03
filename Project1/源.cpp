#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const unsigned int SCR_WIDTH = 800; //screen width
const unsigned int SCR_HEIGHT = 600; //screen height

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

GLFWwindow* initWindow() {
	glfwInit(); //initialize GLFW
	
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW." << std::endl;
		return nullptr;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //run at version3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //core-profile
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); run at Mac OS

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD." << std::endl;
		return nullptr;
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); //set viewport dimension

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	return window;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void render(GLFWwindow* window) {
	//rendering code here
	while (!glfwWindowShouldClose(window)) {
		//input
		processInput(window);

		//command
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //custom color for screen clean
		glClear(GL_COLOR_BUFFER_BIT); //buffer clear

		//event check
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}


int main(){
	GLFWwindow* window = initWindow(); //initialize window
	if (!window) return -1;

	render(window); //rendering

	glfwTerminate(); //release resources
	return 0;
}