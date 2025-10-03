#include "GLUtils.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

const char* uniformShaderSource = R"(
	#version 330 core
	out vec4 FragColor;

	uniform vec4 ourColor;
	
	void main()
	{
		FragColor = ourColor;
	}
)";

int main() {
	GLFWwindow* window = initWindow("LearnOpenGL"); //initialize window
	if (!window) return -1;

	// ===== write code here ===== //
	float vertices[] = {
		//position			//color
		 0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f
	};

	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile("vertex.shader");
	std::ifstream fShaderFile("fragment.shader");

	std::stringstream vShaderStream, fShaderStream;
	vShaderStream << vShaderFile.rdbuf();
	fShaderStream << fShaderFile.rdbuf();

	vShaderFile.close();
	fShaderFile.close();

	vertexCode = vShaderStream.str();
	fragmentCode = fShaderStream.str();

	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragShaderSource = fragmentCode.c_str();

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// ===== Render Loop =====
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// Clear screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //custom color for screen clean
		glClear(GL_COLOR_BUFFER_BIT); //buffer clear

		// Draw 
		glUseProgram(shaderProgram);

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		float base = (sin(timeValue * 2.0f) / 2.0f + 0.5f);
		float pulse = abs(sin(timeValue * 5.0f));

		vertices[3] = base;
		vertices[10] = base;
		vertices[17] = base;
		vertices[4] = pulse;
		vertices[11] = pulse;
		vertices[15] = 1.0f - base;
		vertices[5] = 1.0f - base;
		vertices[9] = 1.0f - pulse;
		vertices[16] = 1.0f - pulse;

		/*double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		float normalizedX = (xpos / SCR_WIDTH) * 2.0f - 1.0f;
		float normalizedY = 1.0f - (ypos / SCR_HEIGHT) * 2.0f;

		vertices[3] = normalizedX;
		vertices[4] = normalizedY;
		vertices[5] = abs(sin(timeValue));

		vertices[10] = normalizedX;
		vertices[11] = normalizedY;
		vertices[9] = abs(sin(timeValue));

		vertices[17] = normalizedX;
		vertices[15] = normalizedY;
		vertices[16] = abs(sin(timeValue));*/


		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate(); //release resources
	return 0;
}