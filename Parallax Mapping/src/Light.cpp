#include "../head/Light.h"

Light::Light(glm::vec3 colour, glm::vec3 pos, glm::vec3 dir)
	:ambient(glm::vec3(0.05f)),
	 diffuse(1.0f, 1.0f, 1.0f),
	 specular(0.3f, 0.3f, 0.3f),
	 color(colour),
	 position(pos),
	 direction(dir) { }

void Light::Update()
{
	float time = static_cast<float>(glfwGetTime());
	float angle = time * speed;
	direction = glm::normalize(glm::vec3(
		sin(angle),
		-1.0f,
		cos(angle)
	));
}

void Light::onImGuiRender()
{
	ImGui::ColorEdit3("Light Color", glm::value_ptr(color));
	ImGui::DragFloat3("Light Direction", glm::value_ptr(direction));
	ImGui::DragFloat3("Light Position", glm::value_ptr(position));
	/*ImGui::SliderFloat("Sun Speed", &speed, 0.0f, 1.0f);
	ImGui::SliderFloat("Sun Distance", &distance, 5.0f, 50.0f);
	ImGui::SliderFloat("orthoRange", &orthoRange, 5.0f, 50.0f);
	ImGui::SliderFloat("nearPlane", &nearPlane, 0.1f, 10.0f);
	ImGui::SliderFloat("farPlane", &farPlane, 5.0f, 500.0f);
	ImGui::SliderFloat("near", &near, 0.1f, 10.0f);
	ImGui::SliderFloat("far", &far, 5.0f, 500.0f);*/
}