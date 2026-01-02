#version 330 core

uniform sampler2D debugTex;
uniform int u_DebugMode;

uniform float fov;
uniform float nearPlane;
uniform float farPlane;

in vec2 TexCoords;
out vec4 FragColor;

float LinearizeDepth(float depth);

void main()
{
    if (u_DebugMode == 3)
    {
        float d = texture(debugTex, TexCoords).r;
        float linear = LinearizeDepth(d) / farPlane;
        FragColor = vec4(vec3(linear), 1.0);
    }
    else
    {
        FragColor = texture(debugTex, TexCoords);
    }
}

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // [0,1] ¡ú NDC [-1,1]
    return (2.0 * nearPlane * farPlane) /
           (farPlane + nearPlane - z * (farPlane - nearPlane));
}
