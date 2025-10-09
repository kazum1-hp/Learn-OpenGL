#include "Light.h"
#include <glm/gtc/matrix_transform.hpp>

Light::Light()
	:color(glm::vec3(1.0f)),
	 ambientStrength(0.1f)
{

}

glm::vec3 Light::getAmbient() const
{
	return ambientStrength * color;
}