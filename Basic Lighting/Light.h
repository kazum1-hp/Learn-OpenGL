#pragma once
#include "glm/glm.hpp"

class Light
{
public:
	Light();

	glm::vec3 getAmbient() const {
		return ambientStrength * color;
	}
	glm::vec3 getColor() const {
		return color;
	}
	glm::vec3 getLightPos() const {
		return position;
	}
	void setLightPos(glm::vec3 pos) { position = pos; };

private:
	float ambientStrength;
	glm::vec3 color;
	glm::vec3 position;
};

