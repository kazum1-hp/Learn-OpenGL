#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct ParallelLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	bool enabled;
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

struct SpotLight {
	vec3 position;
    vec3 direction;
    float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	bool enabled;
};

uniform Material material;

uniform ParallelLight parallelLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

uniform float modelLight;

uniform vec3 viewPos;


vec3 CalParallelLight(ParallelLight parallelLight, vec3 norm, vec3 viewDir);
vec3 CalPointLight(PointLight pointLight, vec3 norm, vec3 viewDir);
vec3 CalSpotLight(SpotLight spotLight, vec3 norm, vec3 viewDir);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	
	// result
	vec4 baseDiffuse = texture(material.diffuse, TexCoord);
	vec4 baseSpecular = texture(material.specular, TexCoord);

	vec3 textureColor = (baseDiffuse.rgb + baseSpecular.rgb) * modelLight; 
	float alpha = baseDiffuse.a; 

	if (alpha < 0.1)
		discard;

	textureColor += CalParallelLight(parallelLight, norm, viewDir) +CalPointLight(pointLight, norm, viewDir) + CalSpotLight(spotLight, norm, viewDir);

	FragColor = vec4(textureColor, alpha);
}

// parallelLight
vec3 CalParallelLight(ParallelLight parallelLight, vec3 norm, vec3 viewDir)
{
	vec3 parallelAmbient = (parallelLight.ambient) * vec3(texture(material.diffuse, TexCoord));
	
	vec3 parallelLightDir = normalize(-parallelLight.direction);
	float parallelDiff = max(dot(norm, parallelLightDir), 0.0);
	vec3 parallelDiffuse = (parallelLight.diffuse) * (parallelDiff * vec3(texture(material.diffuse, TexCoord)));

	vec3 parallelReflectDir = reflect(-parallelLightDir, norm);
	float parallelSpec = pow(max(dot(viewDir, parallelReflectDir), 0.0), material.shininess);
	vec3 parallelSpecular = (parallelLight.specular) * (parallelSpec * vec3(texture(material.specular, TexCoord)));

	vec3 parallelLightColor = (parallelLight.enabled? (parallelAmbient + parallelDiffuse + parallelSpecular) * 0.5 : vec3(0.0));

	return parallelLightColor;
}

// pointLight	
vec3 CalPointLight(PointLight pointLight, vec3 norm, vec3 viewDir)
{
	float distance = length(pointLight.position - FragPos);
	float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));

	vec3 pointAmbient = (attenuation) * (pointLight.ambient) * vec3(texture(material.diffuse, TexCoord));

	vec3 pointLightDir = normalize(pointLight.position - FragPos);
	float pointDiff = max(dot(norm, pointLightDir), 0.0);
	vec3 pointDiffuse = (attenuation) * (pointLight.diffuse * 10.0) * (pointDiff * vec3(texture(material.diffuse, TexCoord)));

	vec3 pointReflectDir = reflect(-pointLightDir, norm);
	float pointSpec = pow(max(dot(viewDir, pointReflectDir), 0.0), material.shininess);
	vec3 pointSpecular = (attenuation) * (pointLight.specular) * (pointSpec * vec3(texture(material.specular, TexCoord)));

	vec3 pointLightColor = (pointLight.enabled? (pointAmbient + pointDiffuse + pointSpecular) : vec3(0.0));

	return pointLightColor;
}

// spotLight
vec3 CalSpotLight(SpotLight spotLight, vec3 norm, vec3 viewDir)
{
	vec3 spotAmbient = (spotLight.ambient) * vec3(texture(material.diffuse, TexCoord));
	vec3 spotLightDir = normalize(spotLight.position - FragPos);
	float theta = dot(spotLightDir, normalize(-spotLight.direction));
	float epsilon = spotLight.cutOff - spotLight.outerCutOff;
	float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

	float spotDiff = max(dot(norm, spotLightDir), 0.0);
	vec3 spotDiffuse = (intensity) * (spotLight.diffuse) * (spotDiff * vec3(texture(material.diffuse, TexCoord)));

	vec3 spotReflectDir = reflect(-spotLightDir, norm);
	float spotSpec = pow(max(dot(viewDir, spotReflectDir), 0.0), material.shininess);
	vec3 spotSpecular = (intensity) * (spotLight.specular) * (spotSpec * vec3(texture(material.specular, TexCoord)));

	vec3 spotLightColor = (spotLight.enabled? (spotAmbient + spotDiffuse + spotSpecular) : vec3(0.0));

	return spotLightColor;
}