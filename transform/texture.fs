#version 330 core
in vec3 myColor;
in vec2 TexCoord;

out vec4 fragColor;

uniform sampler2D texture1;

void main()
{
	fragColor = texture(texture1, TexCoord);
}