#version 330 core

in VS_OUT{
	vec2 TexCoord;
	vec3 Normal;
	vec3 FragPos;
} fs_in;

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
uniform sampler2D textures;
uniform samplerCube shadowMap;
uniform vec3 viewPos;
uniform bool useBlinnPhong;
uniform bool useQuadratic;

uniform float far_plane;
uniform bool shadows;

// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

vec3 CalPointLight(PointLight pointLight, vec3 norm, vec3 viewDir, vec3 texColor, float shadow);

float pointShadowCalculation(vec3 fragPos);

void main()
{
	vec3 norm = normalize(fs_in.Normal);
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);

	vec4 texColor = texture(textures, fs_in.TexCoord);
	float alpha = texColor.a;

	if (alpha < 0.1)
		discard;

	float shadow = shadows ? pointShadowCalculation(fs_in.FragPos) : 0.0;

	vec3 textureColor = CalPointLight(pointLight, norm, viewDir, texColor.rgb, shadow) * modelLight;

	FragColor = vec4(textureColor, alpha);
}

// pointLight	
vec3 CalPointLight(PointLight pointLight, vec3 norm, vec3 viewDir, vec3 texColor, float shadow)
{
	float distance = length(pointLight.position - fs_in.FragPos);
	float attenuation;
	if (useQuadratic)
		attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));
	else
		attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance);
	
	vec3 pointAmbient = (attenuation) *  (pointLight.ambient) * texColor;

	vec3 pointLightDir = normalize(pointLight.position - fs_in.FragPos);
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
		pointLightColor = (pointLight.enabled? (pointAmbient + (1.0 - shadow) * (pointDiffuse + pointSpecular)) : vec3(0.0));
	else
		pointLightColor = (pointLight.enabled? (pointAmbient + (1.0 - shadow) * (pointDiffuse + pointBPSpecular)) : vec3(0.0));

	return pointLightColor;
}

float pointShadowCalculation(vec3 fragPos)
{
	vec3 fragToLight = fragPos - pointLight.position;

	float currentDepth = length(fragToLight);

	float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(shadowMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
        
    // display closestDepth as debug (to visualize depth cubemap)
    // FragColor = vec4(vec3(closestDepth / far_plane), 1.0);    
        
    return shadow;
}

