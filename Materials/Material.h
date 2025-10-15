#pragma once
#include "glm/glm.hpp"

class Material {
private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

public:
	Material()
		:ambient(1.0f, 0.5f, 0.31f),
		 diffuse(1.0f, 0.5f, 0.31f),
		 specular(0.5f, 0.5f, 0.5f),
		 shininess(32.0f) {}

	glm::vec3 getAmbient() const {
		return ambient;
	}
	glm::vec3 getDiffuse() const {
		return diffuse;
	}
	glm::vec3 getSpecular() const {
		return specular;
	}
	float getShininess() const {
		return shininess;
	}
};

