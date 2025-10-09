#pragma once
#include "glm/glm.hpp"

class Light
{
public:
	float ambientStrength;
	glm::vec3 color;
	glm::vec3 position;

	Light();

	glm::vec3 getAmbient() const;


	//void updateColor();
	//void applyToShader();
};

