#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float outlineThickness; 

void main()
{
	vec3 Pos = aPos + aNormal * outlineThickness;
	gl_Position = projection * view * model * vec4(Pos, 1.0);
}