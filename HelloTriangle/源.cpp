#include "GLUtils.h"
#include <iostream>

// ===================== Shader Source =====================
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
// vertex shader source code

const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 Fragcolor;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	Fragcolor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";
// fragment shader source code

// ===== Vertex Data =====
float vertices[] = {
	 0.5f,  0.5f, 0.0f, // upper right
	 0.5f, -0.5f, 0.0f, // lower right
	-0.5f, -0.5f, 0.0f, // lower left
	-0.5f,  0.5f, 0.0f  // upper left
};

unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

int main() {
	GLFWwindow* window = initWindow("LearnOpenGL"); //initialize window
	if (!window) return -1;

	// ===== Compile Vertex Shader =====
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // create vertex shader

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // set source code
	glCompileShader(vertexShader); // compile vertex shader

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); //success check

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// ===== Compile Fragment Shader =====
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); 

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); 

	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// ===== Link Shaders to Program =====
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); 

	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader); //delete shader object

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO); // create buffer object
	glGenBuffers(1, &EBO);

	// Bind VAO first, then bind & set VBO, and configure vertex attributes
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copy data to buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Vertex Attribute: location = 0, size = 3, type = float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //parsing vertxe data
	glEnableVertexAttribArray(0);
	
	// Unbind VBO & VAO (optional)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// ===== Render Loop =====
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// Clear screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //custom color for screen clean
		glClear(GL_COLOR_BUFFER_BIT); //buffer clear

		// Draw
		glUseProgram(shaderProgram); //activate program object
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	// Cleanup
	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate(); //release resources
	return 0;
}