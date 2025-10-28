#include "Light.h"

Light::Light()
	:ambient(0.2f, 0.2f, 0.2f),
	 diffuse(0.5f, 0.5f, 0.5f),
	 specular(1.0f, 1.0f, 1.0f),
	 color(glm::vec3(1.0f)),
	 position(glm::vec3(1.0f)) {}

void Light::update()
{
	float t = static_cast<float>(glfwGetTime());
	color = glm::vec3(
		1.0f
	);
}