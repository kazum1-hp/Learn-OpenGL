#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "GLFW/glfw3.h"
#include "../external/imgui/imgui.h"

class Light
{
public:
	Light();

	void Update();

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
	glm::mat4 getViewMatrix() const {
		return glm::lookAt(-direction * distance, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glm::mat4 getProjectionMatrix() const {
		return glm::ortho(-orthoRange, orthoRange, -orthoRange, orthoRange, nearPlane, farPlane);
	}
	void onImGuiRender();

private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 color;
	glm::vec3 position;
	glm::vec3 direction;

	float orthoRange = 100.0f;
	float nearPlane = 1.0f, farPlane = 100.0f;

	float speed = 0.2f;
	float distance = 100.0f;
};

