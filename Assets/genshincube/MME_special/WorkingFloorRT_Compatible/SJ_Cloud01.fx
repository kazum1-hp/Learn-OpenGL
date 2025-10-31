#include "mirror.fxsub"

#define COLOR(r, g, b, a)\
	float4(r, g, b, a) / 255;

#define ST(x, y, z, w)\
	float4(x, y, z, w);

#include "bloom.conf"
//受辉光影响程度
static const float BloomIntensity = 1.0;

//----------//

//云RGBA图
#define RGBATex "tex/CloudRGBA_01.png"

//主色亮色_白天
float4 LightColor_Day = COLOR(230, 252, 255, 255)

//主色阴影色_白天
float4 ShadowColor_Day = COLOR(130, 206, 244, 255);

//主色亮色_黄昏
float4 LightColor_Mid = COLOR(251, 165, 113, 255);

//主色阴影色_黄昏
float4 ShadowColor_Mid = COLOR(236, 102, 63, 255);

//主色亮色_夜晚
float4 LightColor_Night = COLOR(62, 139, 194, 255);

//主色阴影色_夜晚
float4 ShadowColor_Night = COLOR(29, 92, 150, 255);

//主色强度
static const float AlbedoIntensity = 0.9;

//颜色插值控制
static const float ColorControl = 1.4;

//云透明度
static const float AlphaControl = 0.95;

//----------//

//边缘光范围
static const float RimRadius = 0.7;

//边缘光颜色_白天
float4 RimColor_Day = COLOR(168, 199, 207, 255);

//边缘光颜色_黄昏
float4 RimColor_Mid = COLOR(255, 122, 45, 255);

//边缘光颜色_夜晚
float4 RimColor_Night = COLOR(55, 165, 180, 255);

//边缘光强度
static const float RimIntensity = 2.0;

//----------//

//云Noise图
#define NoiseTex "tex/Noise_01.png"
static const float4 NoiseTex_ST = ST(5.45, 4.98, 0.0, 0.0);

//云Noise流速
static const float4 NoiseSpeed = ST(1.0, -0.47, 0.0, 0.0);

//云Noise强度
static const float NoiseIntensity = 0.9;

//sdf消散Noise图
#define SDFNoise "tex/NoiseRGB_01.png"
static const float4 SDFNoise_ST = ST(1.0, 1.0, 0.0, 0.0);
static const float4 SDFNoise_Mul = ST(1.0, 0.8, 1.38, 0.04);//xyzw对应行数乘算，速度乘算，程度乘算，过渡程度

//----------//

float3 SunPos : CONTROLOBJECT < string name="(self)"; string item = "Sun_Direction"; >;
float3 SunDir : CONTROLOBJECT < string name="(self)"; string item = "Sun_Direction_2"; >;

float NoiseSDFL : CONTROLOBJECT < string name="(self)"; string item = "Cloud01SDF_L"; >;
float NoiseSDFR : CONTROLOBJECT < string name="(self)"; string item = "Cloud01SDF_R"; >;

float ExposureAdd : CONTROLOBJECT < string name="(self)"; string item = "Exposure+"; >;
float ExposureMinus : CONTROLOBJECT < string name="(self)"; string item = "Exposure-"; >;
static float Exposure = 1.0 + ExposureAdd - ExposureMinus;

texture RGBAMap< string ResourceName = RGBATex; >;
sampler RGBASampler = sampler_state
{
	texture = <RGBAMap>;
	MINFILTER = LINEAR; MAGFILTER = LINEAR; MIPFILTER = LINEAR;
	ADDRESSU = WRAP; ADDRESSV = WRAP;
};

texture NoiseMap< string ResourceName = NoiseTex; >;
sampler NoiseSampler = sampler_state
{
	texture = <NoiseMap>;
	MINFILTER = LINEAR; MAGFILTER = LINEAR; MIPFILTER = LINEAR;
	ADDRESSU = WRAP; ADDRESSV = WRAP;
};

texture SDFNoiseMap< string ResourceName = SDFNoise; >;
sampler SDFNoiseSampler = sampler_state
{
	texture = <SDFNoiseMap>;
	MINFILTER = LINEAR; MAGFILTER = LINEAR; MIPFILTER = LINEAR;
	ADDRESSU = WRAP; ADDRESSV = WRAP;
};

struct appdata
{
    float4 vertex : POSITION;
    float2 uv : TEXCOORD1;
    float4 vertex_color : TEXCOORD2;
    float3 normal : NORMAL;
};

struct v2f
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
    float3 normal_world : TEXCOORD1;
    float4 vertex_color : TEXCOORD2;
};

float4x4 WorldMatrix : WORLD;
float4x4 WorldViewProjMatrix : WORLDVIEWPROJECTION;

v2f vert_cloud (appdata v)
{
    v2f o;
    float4 pos_mvp = mul(v.vertex, WorldViewProjMatrix);
    float3 pos_world = mul(WorldMatrix, v.vertex).xyz;

    float4 pos_mvp_mirror = mul(v.vertex, WorldMatrix);
    pos_mvp_mirror = TransMirrorPos(pos_mvp_mirror);
    pos_mvp_mirror = mul(pos_mvp_mirror, ViewProjMatrix);
    pos_mvp_mirror.x = -pos_mvp_mirror.x;

    o.pos = pos_mvp_mirror;
    o.uv = v.uv;
    o.normal_world = normalize(pos_world);
    o.vertex_color = v.vertex_color;
    return o;
}

float Time : TIME;

void SRGB2Linear()
{
	LightColor_Day = pow(LightColor_Day + 1e-8, 2.2);
	ShadowColor_Day = pow(ShadowColor_Day + 1e-8, 2.2);
	LightColor_Mid = pow(LightColor_Mid + 1e-8, 2.2);
	ShadowColor_Mid = pow(ShadowColor_Mid + 1e-8, 2.2);
	LightColor_Night = pow(LightColor_Night + 1e-8, 2.2);
	ShadowColor_Night = pow(ShadowColor_Night + 1e-8, 2.2);
	RimColor_Day = pow(RimColor_Day + 1e-8, 2.2);
	RimColor_Mid = pow(RimColor_Mid + 1e-8, 2.2);
	RimColor_Night = pow(RimColor_Night + 1e-8, 2.2);

    SunBloomColorLow = pow(SunBloomColorLow + 1e-8, 2.2);
	SunBloomColorHigh = pow(SunBloomColorHigh + 1e-8, 2.2);

	MoonBloomColor = pow(MoonBloomColor + 1e-8, 2.2);

	return;
}

float4 frag_cloud (v2f i) : COLOR0
{
	SRGB2Linear();

    float3 normal_world = normalize(i.normal_world);
    float3 SunDirection = normalize(SunPos - SunDir);
    float3 MoonDirection = normalize(SunDir - SunPos);
    float SunDistiance = distance(normal_world, SunDirection);
    float MoonDistiance = distance(normal_world, MoonDirection);

    //白天夜晚插值
    float DayNightStep = smoothstep(-0.3, 0.2, SunDirection.y);
    //黄昏区域
    float SunStep = smoothstep(-0.1, 0.35, SunDirection.y);
    float DuskMask = pow(saturate(dot(SunDirection, normal_world)), pow(5, SunStep * 0.4 + 0.6)) * (1.0 - SunStep);
    //辉光
    SunStep = smoothstep(0.0, 0.33, SunDirection.y);
    float SunBloomMask = pow(saturate(dot(SunDirection, normal_world)), pow(SunBloomPow, SunStep * 0.4 + 0.6));
    float MoonBloomMask = (1 - smoothstep(0, 1, MoonDistiance * 1.0)) * smoothstep(0.4, 1, length(MoonDirection.xyz));
    MoonBloomMask = pow(MoonBloomMask, MoonBloomPow);
    float3 SunBloomFinalColor = lerp(SunBloomColorLow.rgb, SunBloomColorHigh.rgb, SunStep);
    SunBloomFinalColor = SunBloomFinalColor * SunBloomMask * SunBloomStrenth * (1.0 - DuskMask * 0.8);
    float3 MoonBloomFinalColor = MoonBloomColor.rgb * MoonBloomMask * MoonBloomStrenth;

    //Noise
    float4 noise = tex2D(NoiseSampler, (i.uv * NoiseTex_ST.xy + NoiseTex_ST.zw) + Time * 0.02 * NoiseSpeed.xy);
    noise = pow(noise + 1e-8, 2.2);
    noise = noise * 2.0 - 1.0;

    float2 SDFNoiseUV = float2(Time * 0.01 * i.vertex_color.g * SDFNoise_Mul.y, i.vertex_color.r * SDFNoise_Mul.r);   //r为v坐标，g速度，b限制
    float SDFNoiseR = tex2D(SDFNoiseSampler, SDFNoiseUV * SDFNoise_ST.xy + SDFNoise_ST.zw).r;
    SDFNoiseR = smoothstep(0.4, 0.9, SDFNoiseR);
    SDFNoiseR = SDFNoiseR * i.vertex_color.b * SDFNoise_Mul.z;

    //主色
    float4 RGBA = tex2D(RGBASampler, i.uv + noise.xy * NoiseIntensity * 0.01);
    RGBA = pow(RGBA + 1e-8, 2.2);
    RGBA *= smoothstep(saturate(SDFNoiseR), saturate(SDFNoiseR + SDFNoise_Mul.w), RGBA.b);
    float3 AlbedoColor_Day = lerp(ShadowColor_Day, LightColor_Day, RGBA.r * ColorControl) * AlbedoIntensity;
    float3 AlbedoColor_Mid = lerp(ShadowColor_Mid, LightColor_Mid, RGBA.r * ColorControl) * AlbedoIntensity;
    float3 AlbedoColor_Night = lerp(ShadowColor_Night, LightColor_Night, RGBA.r * ColorControl) * AlbedoIntensity;
    float3 AlbedoColor = lerp(AlbedoColor_Night, AlbedoColor_Day, DayNightStep);
    AlbedoColor = lerp(AlbedoColor, AlbedoColor_Mid, DuskMask);

    //边缘光
    float RimStep = smoothstep(-0.5, 0.3, SunDirection.y);
    float NdotL_Sun = (dot(SunDirection, normal_world));
    float NdotL_Moon = (dot(MoonDirection, normal_world));
    float RimArea_Day = smoothstep(RimRadius, 1, NdotL_Sun);
    float RimArea_Night = smoothstep(RimRadius, 1, NdotL_Moon);
    float RimArea = lerp(RimArea_Night, RimArea_Day, RimStep);
    float3 RimColor = lerp(RimColor_Night, RimColor_Day, DayNightStep);
    RimColor = lerp(RimColor, RimColor_Mid, DuskMask);
    RimColor = RimArea * RGBA.g * RimColor * RimIntensity;

    float3 FinalColor = AlbedoColor + RimColor + (SunBloomFinalColor + MoonBloomFinalColor) * BloomIntensity;
    FinalColor *= Exposure;
	FinalColor = pow(FinalColor, 1 / 2.2);
    RGBA = pow(RGBA, 1 / 2.2);

	return float4(FinalColor, RGBA.a * AlphaControl);
}


technique MainTec < string MMDPass = "object"; >
{
    pass DrawObject
	{
        ZWriteEnable = false;
        VertexShader = compile vs_3_0 vert_cloud();
        PixelShader  = compile ps_3_0 frag_cloud();
    }
}

technique MainTec_ss < string MMDPass = "object_ss"; >
{
    pass DrawObject
	{
        ZWriteEnable = false;
        VertexShader = compile vs_3_0 vert_cloud();
        PixelShader  = compile ps_3_0 frag_cloud();
    }
}

technique ShadowTec < string MMDPass = "shadow"; > { }

technique EdgeDepthTec < string MMDPass = "edge"; > { }