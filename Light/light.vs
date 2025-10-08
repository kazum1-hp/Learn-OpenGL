#version 330 core
layout(location = 0) in vec3 lightPos;

uniform mat4 lightModel;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	gl_Position = projection * view * lightModel * vec4(lightPos, 1.0);
}