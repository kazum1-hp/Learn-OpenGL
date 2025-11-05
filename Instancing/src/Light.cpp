#include "../head/Light.h"

Light::Light()
	:ambient(0.2f, 0.2f, 0.2f),
	 diffuse(0.5f, 0.5f, 0.5f),
	 specular(1.0f, 1.0f, 1.0f),
	 color(glm::vec3(1.0f)),
	 position(glm::vec3(-90.0f, 15.0f, -20.0f)),
	 direction(-0.2f, -1.0f, -0.3f) {}

void Light::onImGuiRender()
{
	ImGui::ColorEdit3("Light Color", glm::value_ptr(color));
	//ImGui::DragFloat3("Light Position", glm::value_ptr(position));
}