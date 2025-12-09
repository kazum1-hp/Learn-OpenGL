#version 330 core

in VS_OUT{
	vec2 TexCoord;
	vec3 Normal;
	vec3 FragPos;
	vec4 FragPosLightSpace;
	vec3 Tangent;
	vec3 Bitangent;
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

uniform sampler2D depthMap;
uniform samplerCube shadowMap;

uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D normal;
uniform sampler2D height;
uniform bool hasNormalMap;
uniform bool hasHeightMap;
uniform float height_scale;

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
vec3 CalPointLight(PointLight pointLight, vec3 norm, vec3 viewDir, vec3 pointLightDir, vec3 texColor, float pointShadow);

float ShadowCalculation(vec4 FragPosLightSpace, vec3 n);
float PointShadowCalculation(vec3 fragPos);

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);

void main()
{
	vec3 N = normalize(fs_in.Normal);
	vec3 T = normalize(fs_in.Tangent);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(T, N);
	mat3 TBN = mat3(T, B, N);

	vec3 norm = N;
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 parallelLightDir = normalize(-parallelLight.direction);
	vec3 pointLightDir = normalize(pointLight.position - fs_in.FragPos);
	vec2 texCoords = fs_in.TexCoord;

	if (hasHeightMap)
	{
		viewDir = normalize(transpose(TBN) * viewDir);
		texCoords = ParallaxMapping(fs_in.TexCoord, viewDir);
	
		 // discards a fragment when sampling outside default texture region (fixes border artifacts)
		if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
			discard;
	}

	if (hasNormalMap)
	{
		vec3 normalTex = texture(normal, texCoords).xyz;
		normalTex = normalTex * 2.0 - 1.0;
		// 如果 Y 通道反了：
		//normalTex.y = -normalTex.y;
		norm = normalize(TBN * normalTex);
	}
	
	vec4 texColor = texture(diffuse, texCoords);
	float alpha = texColor.a;

	if (alpha < 0.1)
		discard;

	//vec4 specularColor = texture(specular, fs_in.TexCoord);
	//vec3 texColor = vec3(diffuseColor ).rgb;

	float parallelShadow = parallelShadows ? ShadowCalculation(fs_in.FragPosLightSpace, N) : 0.0;
	float pointShadow = pointShadows ? PointShadowCalculation(fs_in.FragPos) : 0.0;

	vec3 textureColor = (CalParallelLight(parallelLight, norm, viewDir, parallelLightDir, texColor.rgb, parallelShadow) 
						+ CalPointLight(pointLight, norm, viewDir, pointLightDir, texColor.rgb, pointShadow)) * modelLight;

	FragColor = vec4(textureColor, alpha);
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
vec3 CalPointLight(PointLight pointLight, vec3 norm, vec3 viewDir, vec3 pointLightDir, vec3 texColor, float pointShadow)
{
	float distance = length(pointLight.position - fs_in.FragPos);
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

float PointShadowCalculation(vec3 fragPos)
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

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
	// number of depth layers
    const float minLayers = 10;
    const float maxLayers = 20;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * height_scale; 
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(height, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(height, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
    // -- parallax occlusion mapping interpolation from here on
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(height, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}