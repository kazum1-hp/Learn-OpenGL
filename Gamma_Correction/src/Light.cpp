#include "../head/Light.h"

Light::Light()
	:ambient(0.05f, 0.05f, 0.05f),
	 diffuse(1.0f, 1.0f, 1.0f),
	 specular(0.3f, 0.3f, 0.3f),
	 color(glm::vec3(1.0f)),
	 position(glm::vec3(0.0f)),
	 direction(-0.2f, -1.0f, -0.3f) {}

void Light::onImGuiRender()
{
	ImGui::ColorEdit3("Light Color", glm::value_ptr(color));
	ImGui::DragFloat3("Light Position", glm::value_ptr(position));
}