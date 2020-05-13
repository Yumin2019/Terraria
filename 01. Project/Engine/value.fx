

// struct 
struct Light2DInfo
{
    float4 tLightPos;
    float4 tLightColor;
    float4 tLightDir;
    int iLight2DType;
    float fRange;
    float fAngle;
    int ipadding;

};

struct PanelBuffer
{
    int iUse;
   
    float fRValue;
    float fGValue;
    float fBValue;
};

struct TileBuffer
{
    int iX;
    int iY;
    float fGapRatio;
    int iPadding;
};

// b0 번 레지스터를 사용한다.
cbuffer GLOBAL_MATRIX : register(b0)
{
    row_major matrix g_matWorld;
    row_major matrix g_matView;
    row_major matrix g_matProj;

    row_major matrix g_matWV;
    row_major matrix g_matWVP;
}

cbuffer MATERIAL_PARAMETER : register(b1)
{
    float4 g_tDiff;
    float4 g_tSpec;
    float4 g_tEmv;

    // HLSL에서 제공하는 배열은 16byte로 쪼개지므로, 우리가 보통사용 하는 배열과는 상태가 다르다.
    // 따라서, 변수를 하나씩 선언하는 것으로 한다.

    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;

    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;

    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;

    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;

    // matrix 와 같은 형태이다. float4x4
    row_major float4x4 g_mat_0;
    row_major float4x4 g_mat_1;
    row_major float4x4 g_mat_2;
    row_major float4x4 g_mat_3;

    int tex_0;
    int tex_1;
    int tex_2;
    int tex_3;
    
}

// 레지스터 2번 Ani2D 구조체를 넣는 용도 : 애니메이션의 정보를 받아온다.
cbuffer ANI2D : register(b2)
{
    float2 g_tOffset;     // Offset Value
    float2 g_tLeftTop;   // UV LeftTop
    float2 g_tLength;    // UV Length
    int IsAni2D;
    int padding2;
}

// 2D Light
cbuffer LIGHT2D : register(b3)
{
    Light2DInfo g_tLight2DArr[100];
    int g_iLight2DCount;
    int3 padding3;
}

cbuffer BACKGROUND : register(b4)
{
    float2 g_BLeftTop;
    float2 g_BLength;
}

cbuffer TILE : register(b5)
{
   TileBuffer g_TileBuffer[4000];
}

cbuffer PANEL : register(b6)
{
    PanelBuffer g_PanelArr[50];
}

cbuffer GLOBAL_VALUE : register(b7)
{
    float2 g_vResolution;
    float g_fDeltaTime;
    float g_fAccTime;
    
    float g_iMorning;
    float g_fRatio;
    float2 vPadding;
}


/*

typedef struct _tagAni2D
{
	Vec2 tLeftTop;
	Vec2 tLength; // 16
	int IsAni2D;
	int arrPadding[3]; // 16

*/



// 텍스처를 받을 레지스터 ... 
Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);

// 애니메이션용 texture
Texture2D g_tex_anim2D : register(t17);

// Sampler 
sampler g_sam_0 : register(s0); // Aniso
sampler g_sam_1 : register(s1); // Point

// GetDimensions 함수는 많은 자원을 활용한다. 따라서 Test용도로만 HasTex함수를 이용한다.
int HasTex(in Texture2D tex)
{
    uint iWidth = 0;
    uint iHeight = 0;
    tex.GetDimensions(iWidth, iHeight);

    if (iWidth == 0 || iHeight == 0)
        return 0;

    return 1;
}