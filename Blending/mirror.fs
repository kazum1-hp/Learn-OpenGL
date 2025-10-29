#version 330 core

out vec4 FragColor;

uniform vec3 mirrorColor;

void main()
{
	FragColor = vec4(mirrorColor, 0.1);
}