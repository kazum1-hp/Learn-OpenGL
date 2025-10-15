#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 fragColor;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 color;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;

uniform sampler2D texture1;
uniform vec3 viewPos;

void main()
{
	// texture
	vec3 texColor = texture(texture1, TexCoord).rgb;
	
	// ambient light
	vec3 ambient = (light.ambient * light.color) * material.ambient;

	// diffuse reflection
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (light.diffuse * light.color) * (diff * material.diffuse);

	// specular reflection
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (light.specular * light.color) * (spec * material.specular);

	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result * texColor, 1.0);
}