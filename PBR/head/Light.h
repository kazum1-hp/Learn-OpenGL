#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "GLFW/glfw3.h"
#include "../external/imgui/imgui.h"
#include <array>

enum class LightType {
	Directional,
	Point
};

class Light
{
public:
	//Light(glm::vec3 colour = glm::vec3(1.0f), glm::vec3 pos = glm::vec3(-6.0f, 6.0f, 6.0f), glm::vec3 dir = glm::vec3(-2.2f, -2.0f, -2.3f));
	Light(glm::vec3 color, glm::vec3 dirOrPos, LightType type);

	void Update();

	void setColor(glm::vec3 colour) { color = colour; }
	void setLightPos(glm::vec3 pos) { position = pos; }
	void setLightDir(glm::vec3 dir) { direction = dir; }

	glm::vec3 getAmbient() const {
		return ambient * color;
	}
	glm::vec3 getDiffuse() const {
		return diffuse * color;
	}
	glm::vec3 getSpecular() const {
		return specular * color;
	}
	glm::vec3 getColor() const {
		return color;
	}
	glm::vec3 getLightPos() const {
		return position;
	}
	glm::vec3 getLightDir() const {
		return direction;
	}
	glm::mat4 getOrthoViewMatrix() const {
		return glm::lookAt(-direction * distance, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glm::mat4 getOrthoMatrix() const {
		return glm::ortho(-orthoRange, orthoRange, -orthoRange, orthoRange, nearPlane, farPlane);
	}
	glm::mat4 getPerspTransMatrix(GLuint i) const {
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);
		std::array<glm::mat4, 6> shadowTransforms{};
		shadowTransforms[0] = shadowProj * glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
		shadowTransforms[1] = shadowProj * glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
		shadowTransforms[2] = shadowProj * glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
		shadowTransforms[3] = shadowProj * glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
		shadowTransforms[4] = shadowProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
		shadowTransforms[5] = shadowProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));

		return shadowTransforms[i];
	}
	GLfloat getFar() const { return far; }
	bool lightOn() const { return enabled; }

	void dirOnImGuiRender();
	void pointOnImGuiRender(int index);

private:
	LightType type;

	glm::vec3 ambient = glm::vec3(0.05f);
	glm::vec3 diffuse = glm::vec3(1.0f);
	glm::vec3 specular = glm::vec3(0.3f);
	glm::vec3 color;
	glm::vec3 position;
	glm::vec3 direction;

	//ortho matrix
	GLfloat orthoRange = 10.0f;
	GLfloat nearPlane = 1.0f, farPlane = 50.0f;
	float distance = 10.0f;

	//projection matrix
	GLfloat near = 1.0f, far = 30.0f;
	GLfloat aspect = 1024.0f / 1024.0f;
	float speed = 0.2f;

	bool enabled = true;
};

