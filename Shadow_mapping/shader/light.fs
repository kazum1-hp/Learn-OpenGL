#version 330 core

in VS_OUT{
	vec2 TexCoord;
	vec3 Normal;
	vec3 FragPos;
	vec4 FragPosLightSpace;
} fs_in;

out vec4 FragColor;

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
uniform PointLight pointLight;

uniform float modelLight;
uniform sampler2D textures;
uniform sampler2D shadowMap;
uniform vec3 viewPos;
uniform bool useBlinnPhong;
uniform bool useQuadratic;

vec3 CalParallelLight(ParallelLight parallelLight, vec3 norm, vec3 viewDir, vec3 texColor);
vec3 CalPointLight(PointLight pointLight, vec3 norm, vec3 viewDir, vec3 texColor);

float ShadowCalculation(vec4 FragPosLightSpace)
{
	// perform perspective divide
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(-parallelLight.direction); 
    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{
	vec3 norm = normalize(fs_in.Normal);
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);

	vec4 texColor = texture(textures, fs_in.TexCoord);
	float alpha = texColor.a;

	if (alpha < 0.1)
		discard;

	vec3 textureColor = CalParallelLight(parallelLight, norm, viewDir, texColor.rgb) * modelLight;

	FragColor = vec4(textureColor, alpha);
}

// parallelLight

vec3 CalParallelLight(ParallelLight parallelLight, vec3 norm, vec3 viewDir, vec3 texColor)
{
	vec3 parallelAmbient = (parallelLight.ambient) * texColor;
	
	vec3 parallelLightDir = normalize(-parallelLight.direction);
	float parallelDiff = max(dot(norm, parallelLightDir), 0.0);
	vec3 parallelDiffuse = (parallelLight.diffuse) * (parallelDiff * texColor);

	vec3 parallelReflectDir = reflect(-parallelLightDir, norm);
	float parallelSpec = pow(max(dot(viewDir, parallelReflectDir), 0.0), material.shininess / 4.0);
	vec3 parallelSpecular = (parallelLight.specular) * (parallelSpec);

	vec3 parallelHalfVec = normalize(parallelLightDir + viewDir);
	float parallelBPSpec = pow(max(dot(norm, parallelHalfVec), 0.0), material.shininess / 2.0);
	vec3 parallelBPSpecular = (parallelLight.specular) * (parallelBPSpec);

	float shadow = ShadowCalculation(fs_in.FragPosLightSpace);                      

	vec3 parallelLightColor = vec3(0.0);
	if (!useBlinnPhong)
		parallelLightColor = (parallelLight.enabled? (parallelAmbient + (1.0 - shadow) * (parallelDiffuse + parallelSpecular)) : vec3(0.0));
	else
		parallelLightColor = (parallelLight.enabled? (parallelAmbient + (1.0 - shadow) * (parallelDiffuse + parallelBPSpecular)) : vec3(0.0));

	return parallelLightColor;
}

// pointLight	
vec3 CalPointLight(PointLight pointLight, vec3 norm, vec3 viewDir, vec3 texColor)
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

	float shadow = ShadowCalculation(fs_in.FragPosLightSpace);

	vec3 pointLightColor = vec3(0.0);
	if (!useBlinnPhong)
		pointLightColor = (pointLight.enabled? (pointAmbient + (1.0 - shadow) * (pointDiffuse + pointSpecular)) : vec3(0.0));
	else
		pointLightColor = (pointLight.enabled? (pointAmbient + (1.0 - shadow) * (pointDiffuse + pointBPSpecular)) : vec3(0.0));

	return pointLightColor;
}