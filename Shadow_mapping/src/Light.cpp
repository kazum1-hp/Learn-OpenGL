#include "../head/Light.h"

Light::Light()
	:ambient(0.05f, 0.05f, 0.05f),
	 diffuse(1.0f, 1.0f, 1.0f),
	 specular(0.3f, 0.3f, 0.3f),
	 color(glm::vec3(1.0f)),
	 position(glm::vec3(-2.0f, 4.0f, -1.0f)),
	 direction(glm::vec3(-0.2f, -1.0f, -0.3f)) { }

void Light::Update()
{
	float time = glfwGetTime();
	float angle = time * speed;
	direction = glm::normalize(glm::vec3(
		sin(angle),             // 固定 X 方向（东-西 方向）
		-1.0f,      // 高度：sin 控制上下
		cos(angle)
	));
}

void Light::onImGuiRender()
{
	ImGui::ColorEdit3("Light Color", glm::value_ptr(color));
	//ImGui::DragFloat3("Light Position", glm::value_ptr(position));
	ImGui::SliderFloat("Sun Speed", &speed, 0.0f, 1.0f);
	ImGui::SliderFloat("Sun Distance", &distance, 5.0f, 10.0f);
	ImGui::SliderFloat("orthoRange", &orthoRange, 1.0f, 20.0f);
	ImGui::SliderFloat("nearPlane", &nearPlane, 0.1f, 5.0f);
	ImGui::SliderFloat("farPlane", &farPlane, 10.0f, 50.0f);
}