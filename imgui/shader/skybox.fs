#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform float light;

void main()
{
	vec4 Color = texture(skybox, TexCoords);
	FragColor = vec4(Color.rgb * light, Color.a);
}