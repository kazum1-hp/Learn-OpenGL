#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

//out vec4 FragColor;
in vec2 TexCoords;

struct Material {

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

uniform Material material;

uniform ParallelLight parallelLight;
uniform PointLight pointLight[4];

uniform float modelLight;

uniform sampler2D depthMap;
uniform samplerCube shadowMap[4];

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gGeoNormal;
uniform sampler2D gDepth;
uniform mat4 lightSpaceMatrix;

uniform vec3 viewPos;
uniform bool useBlinnPhong;
uniform bool useQuadratic;

uniform float far_plane;
uniform bool parallelShadows;
uniform bool pointShadows;

// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

vec3 CalParallelLight(ParallelLight parallelLight, vec3 norm, vec3 viewDir, vec3 parallelLightDir, vec3 texColor, float parallelShadow);
vec3 CalPointLight(PointLight pointLight, vec3 norm, vec3 viewDir, vec3 pointLightDir, vec3 texColor, float pointShadow, vec3 fragPos);

float ShadowCalculation(vec4 FragPosLightSpace, vec3 n);
float PointShadowCalculation(vec3 fragPos, PointLight pointLight, samplerCube shadowMap);

void main()
{
	float depth = texture(gDepth, TexCoords).r;

	if (depth >= 0.9999)
	{
		discard; // clear background color
	}

	vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
	vec3 N = texture(gGeoNormal, TexCoords).rgb;

	vec3 viewDir  = normalize(viewPos - FragPos);
	vec4 FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

	vec3 pointColor = vec3(0.0);

	for (int i = 0; i < 4; ++i)
	{
		vec3 pointLightDir = normalize(pointLight[i].position - FragPos);

		float pointShadow = pointShadows ? PointShadowCalculation(FragPos, pointLight[i], shadowMap[i]) : 0.0;

		pointColor += CalPointLight(pointLight[i], Normal, viewDir, pointLightDir, Diffuse, pointShadow, FragPos) * modelLight;
	}

	vec3 parallelLightDir = normalize(-parallelLight.direction);
	float parallelShadow = parallelShadows ? ShadowCalculation(FragPosLightSpace, N) : 0.0;
	vec3 parallelColor = CalParallelLight(parallelLight, Normal, viewDir, parallelLightDir, Diffuse, parallelShadow);

	vec3 textureColor = pointColor + parallelColor;

    BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

    FragColor = vec4(textureColor, 1.0);
}

// parallelLight
vec3 CalParallelLight(ParallelLight parallelLight, vec3 norm, vec3 viewDir, vec3 parallelLightDir, vec3 texColor, float parallelShadow)
{
	vec3 parallelAmbient = (parallelLight.ambient) * texColor;
	
	//vec3 parallelLightDir = normalize(-parallelLight.direction);
	float parallelDiff = max(dot(norm, parallelLightDir), 0.0);
	vec3 parallelDiffuse = (parallelLight.diffuse) * (parallelDiff * texColor);

	vec3 parallelReflectDir = reflect(-parallelLightDir, norm);
	float parallelSpec = pow(max(dot(viewDir, parallelReflectDir), 0.0), material.shininess / 4.0);
	vec3 parallelSpecular = (parallelLight.specular) * (parallelSpec);

	vec3 parallelHalfVec = normalize(parallelLightDir + viewDir);
	float parallelBPSpec = pow(max(dot(norm, parallelHalfVec), 0.0), material.shininess / 2.0);
	vec3 parallelBPSpecular = (parallelLight.specular) * (parallelBPSpec);                    

	vec3 parallelLightColor = vec3(0.0);
	if (!useBlinnPhong)
		parallelLightColor = (parallelLight.enabled? (parallelAmbient + (1.0 - parallelShadow) * (parallelDiffuse + parallelSpecular)) : vec3(0.0));
	else
		parallelLightColor = (parallelLight.enabled? (parallelAmbient + (1.0 - parallelShadow) * (parallelDiffuse + parallelBPSpecular)) : vec3(0.0));

	return parallelLightColor;
}

float ShadowCalculation(vec4 FragPosLightSpace, vec3 n)
{
	// perform perspective divide
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depthMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 lightDir = normalize(-parallelLight.direction); 
    float bias = max(0.01 * (1.0 - dot(n, lightDir)), 0.001);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

// pointLight	
vec3 CalPointLight(PointLight pointLight, vec3 norm, vec3 viewDir, vec3 pointLightDir, vec3 texColor, float pointShadow, vec3 fragPos)
{
	float distance = length(pointLight.position - fragPos);
	float attenuation;
	if (useQuadratic)
		attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));
	else
		attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance);
	
	vec3 pointAmbient = (attenuation) *  (pointLight.ambient) * texColor;

	//vec3 pointLightDir = normalize(pointLight.position - fs_in.FragPos);
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
		pointLightColor = (pointLight.enabled? (pointAmbient + (1.0 - pointShadow) * (pointDiffuse + pointSpecular)) : vec3(0.0));
	else
		pointLightColor = (pointLight.enabled? (pointAmbient + (1.0 - pointShadow) * (pointDiffuse + pointBPSpecular)) : vec3(0.0));

	return pointLightColor;
}

float PointShadowCalculation(vec3 fragPos, PointLight pointLight, samplerCube shadowMap)
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
