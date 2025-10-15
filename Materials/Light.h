#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "GLFW/glfw3.h"

class Light
{
public:
	Light();

	glm::vec3 getAmbient() const {
		return ambient;
	}
	glm::vec3 getDiffuse() const {
		return diffuse;
	}
	glm::vec3 getSpecular() const {
		return specular;
	}
	glm::vec3 getColor() const {
		return color;
	}
	glm::vec3 getLightPos() const {
		return position;
	}
	void setLightPos(glm::vec3 pos) { position = pos; };
	void update();

private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 color;
	glm::vec3 position;
};

