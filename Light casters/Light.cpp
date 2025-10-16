#include "Light.h"

Light::Light()
	:ambient(0.2f, 0.2f, 0.2f),
	 diffuse(0.5f, 0.5f, 0.5f),
	 specular(1.0f, 1.0f, 1.0f),
	 color(glm::vec3(1.0f)),
	 position(glm::vec3(1.0f)),
	 direction(-0.2f, -1.0f, -0.3f) {}

void Light::update()
{
	float t = static_cast<float>(glfwGetTime());
	color = glm::vec3(
		1.0f
	);
	position = glm::vec3(5.0f * sin(glfwGetTime()), 4.0f * sin(glfwGetTime()), 2.0f);
}