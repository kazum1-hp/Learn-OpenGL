#ifndef GL_UTILS_H
#define GL_UTILS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"


extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* initWindow(const char* title);
void processInput(GLFWwindow* window);

#endif

