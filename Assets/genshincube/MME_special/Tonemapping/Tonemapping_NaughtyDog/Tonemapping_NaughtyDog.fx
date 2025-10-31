
float4 ClearColor = {1, 1, 1, 1};
float ClearDepth = 1;


////////////////////////////////////////////////////////////////////////////////////////////////

float Script : STANDARDSGLOBAL <
               string ScriptOutput = "color";
string ScriptClass = "scene";
string ScriptOrder = "postprocess";
> = 0.8;


// 当前画面的描画读取
texture ScnMap : RENDERCOLORTARGET <
                 bool AntiAlias = false;
float2 ViewportRatio = {1.0, 1.0};
int MipLevels = 1;
string Format = "A16B16G16R16F";
> ;
sampler ScnSamp = sampler_state
{
    texture = <ScnMap>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

// 遮罩
texture Tonemapping_Mask : OFFSCREENRENDERTARGET <
                     string Description = "the Mask information for Tonemapping_NaughtyDog.x";
float2 ViewPortRatio = {1.0f, 1.0f};
float4 ClearColor = {1, 1, 1, 1};
float ClearDepth = 1.0f;
string Format = "D3DFMT_A8R8G8B8";
int MipLevels = 1;
bool AntiAlias = true;
string DefaultEffect =
    "self = hide;"
    "* = Mask/SJ_Mask_On.fx;";
> ;
sampler MaskMapSmp = sampler_state
{
    texture = <Tonemapping_Mask>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

// 深度缓存
texture DepthBuffer : RENDERDEPTHSTENCILTARGET <
                      float2 ViewPortRatio = {1.0, 1.0};
> ;

// 视角空间的尺寸
float2 ViewportSize : VIEWPORTPIXELSIZE;

// 视角空间偏移
static float2 ViewportOffset = (float2(0.5, 0.5) / ViewportSize);

struct VS_OUTPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
};

float3 TonemapNaughtyDog(float3 x)
{
    float A = -2586.3655;
    float B = 0.6900;
    float C = -767.6706;
    float D = -8.5706;
    float E = 2.8784;
    float F = 107.4683;
    return saturate(((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F);
}

VS_OUTPUT VS_Buffer(float4 Pos
                            : POSITION, float2 Tex
                            : TEXCOORD0)
{
    VS_OUTPUT Out;
    Out.Pos = Pos;
    Out.Tex = Tex + ViewportOffset;
    return Out;
}

float4 PS_Buffer(float2 Tex
                         : TEXCOORD0) : COLOR
{
    float4 ScreenColor = tex2D(ScnSamp, Tex);
    float4 ScreenColorT = pow(ScreenColor + 1e-8, 2.2);
    ScreenColorT.rgb = TonemapNaughtyDog(ScreenColorT.rgb);
    ScreenColorT = pow(ScreenColorT, 1 / 2.2);

    float4 mask = tex2D(MaskMapSmp, Tex);
    ScreenColor = lerp(ScreenColor, ScreenColorT, mask);
    return ScreenColor;
}

////////////////////////////////////////////////////////////////////////////////////////////////

technique PostEffect <
    string Script =
    "RenderColorTarget0=ScnMap;"
    "RenderDepthStencilTarget=DepthBuffer;"
    "ClearSetColor=ClearColor;"
    "ClearSetDepth=ClearDepth;"
    "Clear=Color;"
    "Clear=Depth;"
    "ScriptExternal=Color;"

    "RenderColorTarget0=;"
    "RenderDepthStencilTarget=;"
    "Pass=RimLightBuffer;";
>
{
    pass RimLightBuffer < string Script = "Draw=Buffer;";
    >
    {
        AlphaBlendEnable = FALSE;
        //AlphaTestEnable = FALSE;
        VertexShader = compile vs_3_0 VS_Buffer();
        PixelShader = compile ps_3_0 PS_Buffer();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
