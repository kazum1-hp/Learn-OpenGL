#define COLOR(r, g, b, a)\
	float4(r, g, b, a) / 255;

#define ST(x, y, z, w)\
	float4(x, y, z, w);

#include "bloom.conf"
//受辉光影响程度
static const float BloomIntensity = 1.0;

//----------//

//太阳大小
static const float SunSize = 0.03;

//太阳模糊
static const float SunBlur = 0.5;

//太阳颜色
float4 SunColor = COLOR(255, 255, 255, 255);

//太阳亮度
static const float SunStrenth = 1.25;

//----------//

//月亮贴图
#define MoonCubeTex "tex/Moon_Map.dds"

//月亮大小
static const float MoonSize = 0.042;

//月亮模糊
static const float MoonBlur = 0.35;

//月亮颜色
float4 MoonColor = COLOR(190, 255, 255, 255);

//月亮亮度
static const float MoonStrenth = 1.3;

//----------//

//白天顶部颜色
float4 DayTopColor = COLOR(20, 95, 165, 255);

//白天中部颜色
float4 DayMidColor = COLOR(20, 105, 170, 255);

//白天底部颜色
float4 DayBottomColor = COLOR(118, 189, 214, 255);

//夜晚顶部颜色
float4 NightTopColor = COLOR(0, 27, 60, 255);

//夜晚中部颜色
float4 NightMidColor = COLOR(9, 50, 79, 255);

//夜晚底部颜色
float4 NightBottomColor = COLOR(33, 82, 150, 255);

//白天地平线宽度
static const float DayHorWidth = 0.425;

//白天地平线亮度
static const float DayHorStrenth = 1.0;

//白天地平线颜色
float4 DayHorColor = COLOR(55, 148, 200, 255);

//夜晚地平线宽度
static const float NightHorWidth = 0.425;

//夜晚地平线亮度
static const float NightHorStrenth = 1.0;

//夜晚地平线颜色
float4 NightHorColor = COLOR(25, 58, 98, 255);

//----------//

//淡云彩贴图
#define SoftCloudTex "tex/SoftCloud_Map.png"
static const float4 SoftCloudTex_ST = ST(0.35, 0.8, 0.0, 0.2);

//淡云彩流速
static const float SoftCloudSpeed = 0.2;

//淡云彩颜色
float4 SoftCloudColor = COLOR(155, 206, 255, 255);

//----------//

//星空贴图
#define StarTex "tex/Star_Mask.png"
static const float4 StarTex_ST = ST(1.5, 1.5, 0.0, 1.0);

//星空noise贴图
#define StarNoiseTex "tex/Noise_01.png"
static const float4 StarNoiseTex_ST = ST(7.0, 7.0, 0.0, 0.0);

//星空闪动速度
static const float4 StarNoiseSpeed = ST(0.4, 0.3, 0.0, 0.0);

//星空颜色贴图
#define StarColorTex "tex/NoiseRGB_01.png"
static const float4 StarColorTex_ST = ST(0.8, 0.7, -0.14, 0.33);

//星星颜色
float4 StarColor = COLOR(255, 255, 255, 255);

//星星强度
static const float StarIntensity = 0.6;

//银河贴图
#define GalaxyTex "tex/MilkyWay_Mask.png"
static const float4 GalaxyTex_ST = ST(0.33, 1.77, 0.48, 0.1);

//银河noise贴图
#define GalaxyNoiseTex "tex/Noise_02.png"
static const float4 GalaxyNoiseTex_ST = ST(0.7, 1.23, 0.0, 0.18);

//银河闪动速度
static const float4 GalaxyNoiseSpeed = ST(0.4, 0.0, 0.0, 0.0);

//银河内颜色
float4 GalaxyColorIn = COLOR(29, 4, 6, 255);

//银河外颜色1
float4 GalaxyColorOut1 = COLOR(0, 84, 128, 255);

//银河外颜色2
float4 GalaxyColorOut2 = COLOR(0, 154, 148, 255);

//银河亮度
static const float GalaxyStrenth = 3.0;

//----------//

float3 SunPos : CONTROLOBJECT < string name="(self)"; string item = "Sun_Direction"; >;
float3 SunDir : CONTROLOBJECT < string name="(self)"; string item = "Sun_Direction_2"; >;
float4x4 MoonSpaceMatrix : CONTROLOBJECT < string name="(self)"; string item = "Sun_Direction"; >;
float ExposureAdd : CONTROLOBJECT < string name="(self)"; string item = "Exposure+"; >;
float ExposureMinus : CONTROLOBJECT < string name="(self)"; string item = "Exposure-"; >;
static float Exposure = 1.0 + ExposureAdd - ExposureMinus;

textureCUBE MoonCubeMap < string ResourceName = MoonCubeTex; >;
sampler MoonCubeSampler = sampler_state
{
	texture = <MoonCubeMap>;
	MINFILTER = LINEAR; MAGFILTER = LINEAR; MIPFILTER = LINEAR;
	ADDRESSU = WRAP; ADDRESSV = WRAP;
};

texture SoftCloudMap < string ResourceName = SoftCloudTex; >;
sampler SoftCloudSampler = sampler_state
{
	texture = <SoftCloudMap>;
	MINFILTER = LINEAR; MAGFILTER = LINEAR; MIPFILTER = LINEAR;
	ADDRESSU = WRAP; ADDRESSV = WRAP;
};

texture StarMap < string ResourceName = StarTex; >;
sampler StarSampler = sampler_state
{
	texture = <StarMap>;
	MINFILTER = LINEAR; MAGFILTER = LINEAR; MIPFILTER = LINEAR;
	ADDRESSU = WRAP; ADDRESSV = WRAP;
};

texture StarNoiseMap < string ResourceName = StarNoiseTex; >;
sampler StarNoiseSampler = sampler_state
{
	texture = <StarNoiseMap>;
	MINFILTER = LINEAR; MAGFILTER = LINEAR; MIPFILTER = LINEAR;
	ADDRESSU = WRAP; ADDRESSV = WRAP;
};

texture StarColorMap < string ResourceName = StarColorTex; >;
sampler StarColorSampler = sampler_state
{
	texture = <StarColorMap>;
	MINFILTER = LINEAR; MAGFILTER = LINEAR; MIPFILTER = LINEAR;
	ADDRESSU = WRAP; ADDRESSV = WRAP;
};

texture GalaxyMap < string ResourceName = GalaxyTex; >;
sampler GalaxySampler = sampler_state
{
	texture = <GalaxyMap>;
	MINFILTER = LINEAR; MAGFILTER = LINEAR; MIPFILTER = LINEAR;
	ADDRESSU = CLAMP; ADDRESSV = CLAMP;
};

texture GalaxyNoiseMap < string ResourceName = GalaxyNoiseTex; >;
sampler GalaxyNoiseSampler = sampler_state
{
	texture = <GalaxyNoiseMap>;
	MINFILTER = LINEAR; MAGFILTER = LINEAR; MIPFILTER = LINEAR;
	ADDRESSU = WRAP; ADDRESSV = WRAP;
};

//From Inigo Quilez, https://www.iquilezles.org/www/articles/intersectors/intersectors.htm
float sphIntersect(float3 rayDir, float3 spherePos, float radius)
{
    float3 oc = -spherePos;
    float b = dot(oc, rayDir);
    float c = dot(oc, oc) - radius * radius;
    float h = b * b - c;
    if(h < 0.0) return -1.0;
    h = sqrt(h);
    return -b - h;
}

//三相映射 https://blog.csdn.net/ezcome11/article/details/117051470
float4 TriPlanar (float4 posWS, float3 normal, float smooth, sampler2D textures, float4 STnum)
{
	float3 normalWS = normalize(normal);
	float3 weight = pow(abs(normalWS), smooth);
	float3 uvWeight = weight / (weight.x + weight.y + weight.z);

	float4 col0 = tex2D(textures, posWS.xy * STnum.xy + STnum.zw) * uvWeight.z;
    float4 col1 = tex2D(textures, posWS.xz * STnum.xy + STnum.zw) * uvWeight.y;
    float4 col2 = tex2D(textures, posWS.zy * STnum.xy + STnum.zw) * uvWeight.x;
    
    return col0 + col1 + col2;
}

struct appdata
{
    float4 vertex : POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
};

struct v2f
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
    float3 pos_world : TEXCOORD1;
};

float4x4 WorldMatrix : WORLD;
float4x4 WorldViewProjMatrix : WORLDVIEWPROJECTION;

v2f vert_skybox (appdata v)
{
    v2f o;
    float4 pos_mvp = mul(v.vertex, WorldViewProjMatrix);
    float3 pos_world = mul(WorldMatrix, v.vertex).xyz;

    o.pos = pos_mvp;
    o.uv = v.uv;
    o.pos_world = pos_world;
    return o;
}

//校准月亮朝向 https://kelvinvanhoorn.com/2022/03/17/skybox-tutorial-part-1
static const float3x3 CorrectionMatrix = float3x3(0, -0.2588190451, -0.9659258263,
    0.08715574275, 0.9622501869, -0.2578341605,
    0.9961946981, -0.08418598283, 0.02255756611);

float Time : TIME;

void SRGB2Linear()
{
	SunColor = pow(SunColor + 1e-8, 2.2);
	SunBloomColorLow = pow(SunBloomColorLow + 1e-8, 2.2);
	SunBloomColorHigh = pow(SunBloomColorHigh + 1e-8, 2.2);

	MoonColor = pow(MoonColor + 1e-8, 2.2);
	MoonBloomColor = pow(MoonBloomColor + 1e-8, 2.2);

	DayTopColor = pow(DayTopColor + 1e-8, 2.2);
	DayMidColor = pow(DayMidColor + 1e-8, 2.2);
	DayBottomColor = pow(DayBottomColor + 1e-8, 2.2);
	NightTopColor = pow(NightTopColor + 1e-8, 2.2);
	NightMidColor = pow(NightMidColor + 1e-8, 2.2);
	NightBottomColor = pow(NightBottomColor + 1e-8, 2.2);

	DayHorColor = pow(DayHorColor + 1e-8, 2.2);
	NightHorColor = pow(NightHorColor + 1e-8, 2.2);

	SoftCloudColor = pow(SoftCloudColor + 1e-8, 2.2);

	StarColor = pow(StarColor + 1e-8, 2.2);
	GalaxyColorIn = pow(GalaxyColorIn + 1e-8, 2.2);
	GalaxyColorOut1 = pow(GalaxyColorOut1 + 1e-8, 2.2);
	GalaxyColorOut2 = pow(GalaxyColorOut2 + 1e-8, 2.2);
	
	return;
}

float4 frag_skybox (v2f i) : COLOR0
{
	SRGB2Linear();
	float3 pos_world = normalize(i.pos_world);
    float3 SunDirection = normalize(SunPos - SunDir);
    float3 MoonDirection = normalize(SunDir - SunPos);
    
	//求太阳范围位置
    float SunDistiance = distance(pos_world, SunDirection);
	float SunMask = 1 - SunDistiance / SunSize;
	SunMask = smoothstep(0.0, SunBlur, SunMask);

    //求月亮
    float MoonIntersect = sphIntersect(pos_world, MoonDirection, MoonSize);
    float MoonDistiance = distance(pos_world, MoonDirection);
    float MoonMask = 1 - MoonDistiance / MoonSize;
    MoonMask = smoothstep(0.0, MoonBlur, MoonMask);

    float3 MoonNormal = normalize(MoonDirection - pos_world * MoonIntersect);
    MoonNormal = mul(MoonSpaceMatrix, float4(MoonNormal,0)).xyz;
    MoonNormal = mul(CorrectionMatrix, MoonNormal);
    float MoonTex = texCUBE(MoonCubeSampler, MoonNormal).r;
    MoonTex = pow(MoonTex + 1e-8, 2.2);
    float3 MoonFinalColor = MoonTex * MoonStrenth * MoonColor.rgb;

    //天空基础颜色
    float DayNightStep = smoothstep(-0.20, 0.20, SunDirection.y);
    float4 GradientDay = lerp(DayBottomColor, DayMidColor, saturate(pos_world.y)) * step(0, -pos_world.y);
    GradientDay += lerp(DayMidColor, DayTopColor, saturate(pos_world.y)) * step(0, pos_world.y);
    float4 GradientNight = lerp(NightBottomColor, NightMidColor, saturate(pos_world.y)) * step(0, -pos_world.y);
    GradientNight += lerp(NightMidColor, NightTopColor, saturate(pos_world.y)) * step(0, pos_world.y);
    float4 SkyGradient = lerp(GradientNight, GradientDay, DayNightStep);

    //天空地平线颜色
    float HorWidth = lerp(NightHorWidth, DayHorWidth, DayNightStep);
    float HorStrenth = lerp(NightHorStrenth, DayHorStrenth, DayNightStep);
    float HorMask = smoothstep(-HorWidth, 0, pos_world.y) * smoothstep(-HorWidth, 0, -pos_world.y);
    float4 HorGradient = lerp(NightHorColor, DayHorColor, DayNightStep);
    HorGradient.rgb *= HorStrenth;
	SkyGradient = lerp(SkyGradient, HorGradient, HorMask);

    //soft cloud
    float2 SoftCloudUV = pos_world.xy * SoftCloudTex_ST.xy + SoftCloudTex_ST.zw;
    SoftCloudUV.x = pos_world.z < 0 ? 1 - SoftCloudUV.x : SoftCloudUV.x;
    SoftCloudUV.x += Time * SoftCloudSpeed * 0.01;
    SoftCloudUV.y = smoothstep(0.3, 0.8, SoftCloudUV.y);
    float SoftCloudFinalTex = tex2D(SoftCloudSampler, SoftCloudUV).a;
    SoftCloudFinalTex = smoothstep(0.05, 0.8, SoftCloudFinalTex) * abs(pos_world.z);
    float4 SoftCloudFinalColor = SoftCloudFinalTex * SoftCloudColor;

    //星空与银河
    float StarTexFinal = TriPlanar(float4(pos_world, 0), pos_world, 5.0, StarSampler, StarTex_ST);
    float StarNoiseTexFinal = tex2D(StarNoiseSampler, (pos_world.xz + Time * 0.01 * StarNoiseSpeed.xy) * StarNoiseTex_ST.xy + StarNoiseTex_ST.zw).r;
    StarNoiseTexFinal = pow(StarNoiseTexFinal + 1e-8, 2.2);
	float3 StarColorTexFinal = tex2D(StarColorSampler, (pos_world.xz + Time * 0.005 * GalaxyNoiseSpeed.xy) * StarColorTex_ST.xy + StarColorTex_ST.zw).rgb;
    StarColorTexFinal = pow(StarColorTexFinal + 1e-8, 2.2);
	float GalaxyNoise = tex2D(GalaxyNoiseSampler, (pos_world.xz - Time * 0.01 * GalaxyNoiseSpeed.xy) * GalaxyNoiseTex_ST.xy + GalaxyNoiseTex_ST.zw).r;
    GalaxyNoise = pow(GalaxyNoise + 1e-8, 2.2);
	float3 GalaxyTexFinal = tex2D(GalaxySampler, (pos_world.xz + (GalaxyNoise - 0.5) * 0.05) * GalaxyTex_ST.xy + GalaxyTex_ST.zw).rgb;
    GalaxyTexFinal  = pow(GalaxyTexFinal  + 1e-8, 2.2);
	float4 GalaxyColorOut = GalaxyColorOut1 * GalaxyTexFinal.r * pow(GalaxyNoise, 0.9) + GalaxyColorOut2 * GalaxyTexFinal.r * pow(GalaxyNoise, 3.5);
    float4 GalaxyColorInner = GalaxyColorIn * GalaxyTexFinal.g * GalaxyNoise;
    float4 GalaxyColor = lerp(GalaxyColorOut, GalaxyColorInner, GalaxyTexFinal.g * (1.0 - GalaxyNoise));
    GalaxyColor = GalaxyColor * GalaxyStrenth * (1.0 - DayNightStep);
    float StarPos = smoothstep(0.0, 0.3, StarTexFinal);
    float StarBright = smoothstep(0.0, 0.7, StarNoiseTexFinal);
    float3 StarFinalColor = StarPos * StarBright * StarColor * StarIntensity * StarColorTexFinal;
    StarFinalColor += StarFinalColor * GalaxyTexFinal.r * 6.0;
    float StarMask = lerp((1 - smoothstep(-0.4, 0.1, -pos_world.y)), 0, DayNightStep);
    StarFinalColor = StarFinalColor * StarMask;

    //太阳 月亮辉光
    float SunStep = smoothstep(0.0, 0.33, SunDirection.y);
    float SunBloomMask = pow(saturate(dot(SunDirection, pos_world)), pow(SunBloomPow, SunStep * 0.4 + 0.6));
    float MoonBloomMask = (1 - smoothstep(0, 1, MoonDistiance * 1.0)) * smoothstep(0.4, 1, length(MoonDirection.xyz));
    MoonBloomMask = pow(MoonBloomMask, MoonBloomPow);
    float3 SunBloomFinalColor = lerp(SunBloomColorLow.rgb, SunBloomColorHigh.rgb, SunStep);
    SunBloomFinalColor = lerp(SunBloomFinalColor, SkyGradient.rgb, SunDirection.y * 0.33) * SunBloomMask * SunBloomStrenth;
    float3 MoonBloomFinalColor = (SkyGradient.rgb + MoonBloomColor.rgb) * MoonBloomMask * MoonBloomStrenth;

    //混合
    float3 SkyColor = SkyGradient.rgb;
    SkyColor += StarFinalColor.rgb + GalaxyColor.rgb + SoftCloudFinalColor.rgb;
    SkyColor = lerp(SkyColor, SunColor.rgb * SunStrenth, SunMask);
    SkyColor = lerp(SkyColor, MoonFinalColor, MoonMask);
    SkyColor += (SunBloomFinalColor + MoonBloomFinalColor) * BloomIntensity;

    SkyColor *= Exposure;

	SkyColor = pow(SkyColor, 1 / 2.2);

	return float4(SkyColor, 1.0);
}


technique MainTec < string MMDPass = "object"; >
{
    pass DrawObject
	{
        VertexShader = compile vs_3_0 vert_skybox();
        PixelShader  = compile ps_3_0 frag_skybox();
    }
}

technique MainTec_ss < string MMDPass = "object_ss"; >
{
    pass DrawObject
	{
        VertexShader = compile vs_3_0 vert_skybox();
        PixelShader  = compile ps_3_0 frag_skybox();
    }
}

technique ShadowTec < string MMDPass = "shadow"; > { }

technique EdgeDepthTec < string MMDPass = "edge"; > { }