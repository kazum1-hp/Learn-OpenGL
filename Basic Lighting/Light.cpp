#include "Light.h"
#include <glm/gtc/matrix_transform.hpp>

Light::Light()
	:ambientStrength(0.1f),
	 color(glm::vec3(1.0f)),
	 position(glm::vec3(1.0f))
{

}
