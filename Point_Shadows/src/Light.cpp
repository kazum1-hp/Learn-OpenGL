#include "../head/Light.h"

Light::Light()
	:ambient(glm::vec3(0.05f)),
	 diffuse(1.0f, 1.0f, 1.0f),
	 specular(0.3f, 0.3f, 0.3f),
	 color(glm::vec3(1.0f)),
	 position(glm::vec3(0.0f, 0.0f, 0.0f)),
	 direction(glm::vec3(-0.2f, -1.0f, -0.3f)) { }

void Light::Update()
{
	float time = glfwGetTime();
	float angle = time * speed;
	direction = glm::normalize(glm::vec3(
		sin(angle),
		sin(angle),
		cos(angle)
	));
}

void Light::onImGuiRender()
{
	ImGui::ColorEdit3("Light Color", glm::value_ptr(color));
	ImGui::DragFloat3("Light Position", glm::value_ptr(position));
	ImGui::SliderFloat("Sun Speed", &speed, 0.0f, 1.0f);
	ImGui::SliderFloat("Sun Distance", &distance, 5.0f, 50.0f);
	//ImGui::SliderFloat("orthoRange", &orthoRange, 5.0f, 50.0f);
	ImGui::SliderFloat("nearPlane", &near, 0.1f, 10.0f);
	ImGui::SliderFloat("farPlane", &far, 5.0f, 500.0f);
}