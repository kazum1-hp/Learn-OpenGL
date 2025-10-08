#version 330 core
in vec2 TexCoord;

out vec4 fragColor;

uniform sampler2D texture1;
uniform vec3 lightColor;

void main()
{
	vec3 texColor = texture(texture1, TexCoord).rgb;
	fragColor = vec4(lightColor * texColor, 1.0);
}