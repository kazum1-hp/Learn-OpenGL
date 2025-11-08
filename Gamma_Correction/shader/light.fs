#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

struct Material {

	float shininess;
};

struct PointLight {
	vec3 position;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic;

	bool enabled;
};

uniform Material material;

uniform PointLight pointLight;

uniform float modelLight;
uniform sampler2D plane;
uniform vec3 viewPos;
uniform bool useBlinnPhong;
uniform bool useQuadratic;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	
	float distance = length(pointLight.position - FragPos);
	float attenuation;
	if (useQuadratic)
		attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));
	else
		attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance);

	vec3 texColor = vec3(texture(plane, TexCoord));

	vec3 pointAmbient = (attenuation) *  (pointLight.ambient) * texColor;

	vec3 pointLightDir = normalize(pointLight.position - FragPos);
	float pointDiff = max(dot(norm, pointLightDir), 0.0);
	vec3 pointDiffuse = (attenuation) * (pointLight.diffuse ) * (pointDiff * texColor);

	vec3 pointReflectDir = reflect(-pointLightDir, norm);
	float pointSpec = pow(max(dot(viewDir, pointReflectDir), 0.0), material.shininess / 4.0);
	vec3 pointSpecular = (attenuation) * (pointLight.specular) * (pointSpec);

	vec3 pointHalfVec = normalize(pointLightDir + viewDir);
	float pointBPSpec = pow(max(dot(norm, pointHalfVec), 0.0), material.shininess / 2.0);
	vec3 pointBPSpecular = (attenuation) * (pointLight.specular) * (pointBPSpec);

	vec3 pointLightColor = vec3(0.0);
	if (!useBlinnPhong)
		pointLightColor = (pointLight.enabled? (pointAmbient + pointDiffuse + pointSpecular) : vec3(0.0));
	else
		pointLightColor = (pointLight.enabled? (pointAmbient + pointDiffuse + pointBPSpecular) : vec3(0.0));

	vec3 textureColor = pointLightColor;

	FragColor = vec4(textureColor, 1.0);
}


