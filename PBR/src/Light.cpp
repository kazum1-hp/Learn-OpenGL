#include "../head/Light.h"

Light::Light(glm::vec3 color, glm::vec3 dirOrPos, LightType type)
	:color(color), type(type)
{
	if (type == LightType::Directional) {
		direction = dirOrPos;
		position = glm::vec3(0.0f);
	}
	else { // point light
		position = dirOrPos;
		direction = glm::vec3(0.0f); 
	}
}

void Light::Update()
{
	//float time = static_cast<float>(glfwGetTime());
	//float angle = time * speed;
	//direction = glm::normalize(glm::vec3(
	//	sin(angle),
	//	-1.0f,
	//	cos(angle)
	//));
}

void Light::dirOnImGuiRender()
{
	ImGui::ColorEdit3("Light Color", glm::value_ptr(color));
	ImGui::DragFloat3("Light Direction", glm::value_ptr(direction));
}

void Light::pointOnImGuiRender(int index)
{
	ImGui::PushID(index);

	ImGui::Checkbox("Enabled", &enabled);
	ImGui::ColorEdit3("Light Color", glm::value_ptr(color));
	ImGui::DragFloat3("Light Position", glm::value_ptr(position));

	ImGui::PopID();
}