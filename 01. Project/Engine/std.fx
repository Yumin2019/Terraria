
// HLSL 

#include "value.fx"

/*
	Vec4			m_tDiff; // Diffusion 확산 색상 계수.
	Vec4			m_tSpec; // 반사광 계수.
	Vec4			m_tEmv; // 발광 계수.

	// 0 ~ 3번까지, Shader로 넘길 데이터에 대한 정보이다.
	int					m_IntArray[(UINT)SHADER_PARAM::INT_0 - (UINT)SHADER_PARAM::INT_END];
	float				m_FloatArray[(UINT)SHADER_PARAM::FLOAT_0 - (UINT)SHADER_PARAM::FLOAT_END];
	Vec2				m_Vec2Array[(UINT)SHADER_PARAM::VEC2_0 - (UINT)SHADER_PARAM::VEC2_END];
	Vec4				m_Vec4Array[(UINT)SHADER_PARAM::VEC4_0 - (UINT)SHADER_PARAM::VEC4_END];
	Matrix				m_MatArray[(UINT)SHADER_PARAM::MATRIX_0 - (UINT)SHADER_PARAM::MATRIX_END];
*/


struct VS_COL_INPUT
{
    float3 tPos : POSITION;
    float4 tColor : COLOR;

	// float3 .. float4 는 HLSL 언어에서 제공하는 구조체이다.
};

struct VS_COL_OUTPUT
{
	// 출력은 float4형태로 넘겨야 한다.
    float4 tPos : SV_Position; // SYSTEM_VALUE POS
    float4 tColor : COLOR;
};

// Vertex Color Shader
VS_COL_OUTPUT VS_Color(VS_COL_INPUT tInput)
{
    VS_COL_OUTPUT output = (VS_COL_OUTPUT) 0; // HLSL에서 구조체를 초기화하는 방식이다. 

	//// 투영 좌표계를 반환할 때에는 FLOAT4 4번째 요소에 1.f 값을 넣어준다.
 //   output.tPos = float4(tInput.tPos, 1.f); // HLSL의 문법에서는 다양한 생성자를 기본적으로 지원해주고 있다.
 //   output.tColor = tInput.tColor;


    // 넘어온 정점의 좌표 (Local)에서 w자리에 1.f를 넣어서 움직임을 정의하도록 한다.
    // 이후에 WorldMatrix를 곱하여, WorldSpace의 공간으로 물체를 이동시킨다. (각 좌표에 매칭되는 자리가 1의 값을 가지고-> 단위행렬)
    // 4행에서 움직임을 정의하고 있다. 
    // 여기서 각 자리의 부분에서는 닮음 변환을 정의할 수 있다.
    // 아무튼 이동하는 좌표는 (0 + dX, 0 + dY, 0 + dZ, 1.f)가 될 것이다.
    // World로의 변환 과정에서 회전 / Scaling을 할 수 있다. 
    float4 vWorldPos = mul(float4(tInput.tPos, 1.f), g_matWorld);

    // WorldSpace를 CameraSpace로 표현한다. 행렬의 정보에서는 카메라의 좌표를 원점으로 이동시키는 벡터에 대한 연산을 
    // 움직임으로 4행에 정의하고 있으며, 회전 행렬에 대해서는 하고 있지 않다.
    float4 vViewPos = mul(vWorldPos, g_matView);

    // CameraSpace를 Projection Space로 표현한다. 
    // 나중에 .
    float4 vProjPos = mul(vViewPos, g_matProj);

    output.tColor = tInput.tColor;
    output.tPos = vProjPos;


    return output;

	/*
	정리 : VS_COL_OUTPUT 구조체의 형태로  값을 반환한다. 
	들어오는 값은 시스템 메모리에서 넘어오는 VS_COL_INPUT의 형태이다.
	이렇게 들어온 좌표에 대해서 아무런 처리도 하지 않고(이미 투영좌표계로 잡아두었으니) 그대로 값을 VS_COL_OUTPUT 형태로
	반환해준다.
	*/
}




float4 PS_Color(VS_COL_OUTPUT tOutput) : SV_Target
{
    return tOutput.tColor;
}



struct TEX_INPUT
{
    float3 tPos : POSITION;
    float2 tUVPos : TEXCOORD;
    // 원하는 정보만을 정점에서 뽑아서 사용할 수 있다.
};

struct TEX_OUTPUT
{
    float4 tOutPos : SV_Position;
    float2 tUVPos : TEXCOORD;
};

TEX_OUTPUT VS_Tex(TEX_INPUT tInput)
{
    TEX_OUTPUT output = (TEX_OUTPUT) 0; // HLSL에서 구조체를 초기화하는 방식이다. 
    float4 vWorldPos = mul(float4(tInput.tPos, 1.f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProj);

    output.tOutPos = vProjPos;
    output.tUVPos = tInput.tUVPos;

    return output;
}

float4 PS_Tex(TEX_OUTPUT tInput) : SV_Target
{
    float4 tColor = (float4) 0.f;

    if (g_int_0 == 0)
        tColor = g_tex_0.Sample(g_sam_1, tInput.tUVPos);
  
    else if (g_int_0 == 1)
        tColor = g_tex_1.Sample(g_sam_1, tInput.tUVPos);
    
    else if (g_int_0 == 2)
        tColor = g_tex_2.Sample(g_sam_1, tInput.tUVPos);
    
    else if (g_int_0 == 3)
        tColor = g_tex_3.Sample(g_sam_1, tInput.tUVPos);

    // UVPos를 토대로 보간된 값이 넘어와서, 그림에 해당하는 픽셀 정보를 반환.
    // 넘겨준 그림의 텍스처를 크기에 맞게 변환하여, 해당 위치에 픽셀을 찍어준다.
    if (g_float_0)
        tColor.rgb /= g_float_0;
    
    if (g_int_1) // DayLight여부
    {
        if (g_iMorning)
        {
            if (g_fRatio >= 0.5f)
            {
                tColor.rgb *= (0.2f + 0.8f * (1.f - g_fRatio) * 2.f);
            }
            else
            {
                tColor.rgb *= (0.2f + 0.8f * (g_fRatio * 2.f));
            }
        
        }
        else
        {
            if (g_fRatio >= 0.5f)
            {
                tColor.rgb *= (0.2f + 0.8f * (1.f - g_fRatio) * 0.4f);
            }
            else
            {
                tColor.rgb *= (0.2f + 0.8f * (g_fRatio * 0.4f));
            }
        
        }

    }
       
        return tColor;
}

// Collider 2D Shader

TEX_OUTPUT VS_Collider2D(TEX_INPUT tInput)
{
    TEX_OUTPUT output = (TEX_OUTPUT) 0;

    float4 vWorldPos = mul(float4(tInput.tPos, 1.f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProj);

    output.tOutPos = vProjPos;
    output.tUVPos = tInput.tUVPos;

    return output;

}

float4 PS_Collider2D(TEX_OUTPUT tInput) : SV_Target
{
    if (g_int_0)
        return float4(1.f, 0.2f, 0.f, 0.f);

    return float4(0.f, 1.f, 0.f, 1.f);
}

// ====================
// Standard 2D Shader
// g_tex_0 : Default Texture 
// g_tex_anim2D : Animation Texture
// alpha blend : true
// ====================

struct VTX_INPUT
{
    float3 tPos : POSITION;
    float2 tUV : TEXCOORD;
};

struct VTX_OUTPUT
{
    float4 tPos : SV_POSITION;
    float2 tUV : TEX_COORD;
    float3 tWorldPos : POSITION; // PixelShader에서는 Pixel의 WorldPos를 받길 원한다.
};


VTX_OUTPUT VS_Standard2D(VTX_INPUT tInput)
{
    VTX_OUTPUT output = (VTX_OUTPUT) 0.f;
   
    float4 tPosition = (float4) 0.f;

    if (IsAni2D)
    {
        // 애니메이션을 사용하는 경우에는 월드행렬을 통해서 좌표를 구하고 Offset을 더해서 출력위치를 지정한다.
        // 오프셋을 고려하지 않고 추가한 각 프레임을 임의의 Offset을 통해서 조정하여 출력위치를 정한다는 의미이다.
        // 리소스에서 출력이되는 위치를 변경하는 개념이 아니라, 출력위치를 Shader단에서 교체하는 개념이다.
        // 따라서, 적당히 애니메이션만 추가를 해둔 이후에 Offset을 맞추어 처리할 수 있다.
        tPosition = mul(float4(tInput.tPos, 1.f), g_matWorld);
        tPosition.xy += g_tOffset;
        tPosition = mul(tPosition, g_matView);
        tPosition = mul(tPosition, g_matProj);
    }
    else
    {
        tPosition = mul(float4(tInput.tPos, 1.f), g_matWVP);
    }

    output.tPos = tPosition;
    output.tUV = tInput.tUV;

    float3 tWorldPos = mul(float4(tInput.tPos, 1.f), g_matWorld);
    tWorldPos.z = 0.f;
    output.tWorldPos = tWorldPos;

    //  Standard 2D Vertex 함수에서는 WorldMatrix를 통해 정점의 World좌표를 구해둔다.

    return output;
}

// TitleShader
//
//
//

float4 PS_Title(VTX_OUTPUT tInput) : SV_Target
{
    float4 tColor = (float4) 0.f;

    // x가 AccTime의 영향을 받도록 한다. Right To Left
    tInput.tUV.x += g_fAccTime * 0.04f * g_float_1;
    
    if(tex_0)
      tColor = g_tex_0.Sample(g_sam_1, tInput.tUV);
    else 
        return float4(1.f, 0.f, 1.f, 1.f);

    
    if (g_iMorning)
    {
        if (g_fRatio >= 0.5f)
        {
            tColor.rgb *= (0.2f + 0.8f * (1.f - g_fRatio) * 2.f);
        }
        else
        {
            tColor.rgb *= (0.2f + 0.8f * (g_fRatio * 2.f));
        }
        
    }
    else
    {
        if (g_fRatio >= 0.5f)
        {
            tColor.rgb *= (0.2f + 0.8f * (1.f - g_fRatio) * 0.4f);
        }
        else
        {
            tColor.rgb *= (0.2f + 0.8f * (g_fRatio * 0.4f));
        }
        
    }
    
    if (g_float_0)
        tColor.rgb /= g_float_0;
    
    return tColor;
}


// HeartShader
// g_int_0 : HP
// g_int_1 : Max HP 
// g_tex_0 : Output Texture

float4 PS_Heart(VTX_OUTPUT tInput) : SV_Target
{
    float4 tColor = (float4) 0.f;
    
    if (tex_0 == 0)
        return float4(1.f, 0.f, 1.f, 1.f);
    
    // 5개에 해당하는 Idx를 구한다.
    int iIdx = tInput.tUV.x / 0.2f;
    
    // 5분할은 정해져 있다.
    int iHeartSize = g_int_1 / 5; // 20
    int iHeartCount = g_int_0 / iHeartSize; // HP / HeartSize(개당 수치) -> 몫 만큼이 하트 설치 개수. 70 / 20 - > 3
    
    // 자신의 위치가 출력이 되어야 하는지 판단한다.
    if (iIdx <= iHeartCount)
    {
        tInput.tUV.x = (tInput.tUV.x % 0.2f) * 5.f;
        tColor = g_tex_0.Sample(g_sam_1, tInput.tUV);
        
        if (iIdx == iHeartCount)
        {
            // 이 경우에는 Alpha값의 영향을 받도록 처리한다.
            int iLeftOver = g_int_0 % iHeartSize; // 70 % 20 -> 10 
            float fRatio = (float) iLeftOver / iHeartSize; // 현재 남은 체력 / (개당 수치) -> 비율을 구해낸다 (Alpha값으로 이용)
            
            if (tColor.a)
                tColor.a = fRatio;
        }
        
    }
    else
        clip(-1);
    
    
    return tColor;
}


// MonsterUIShader
// g_int_0 : HP
// g_int_1 : Max HP 
// g_tex_0 : Output Texture

float4 PS_MonsterUI(VTX_OUTPUT tInput) : SV_Target
{
    float4 tColor = (float4) 0.f;
    
    if (tex_0 == 0)
        return float4(1.f, 0.f, 1.f, 1.f);
    
    // 최대체력인 경우는 출력하지 않는다.
    if (g_int_0 == g_int_1)
        clip(-1);
    
    tColor = g_tex_0.Sample(g_sam_0, tInput.tUV);
        
    // Border에 해당하는 부분은 그냥 출력.
    if (tColor.r == 0.f && tColor.g == 0.f && tColor.b == 0.f)
        return tColor;
    
    // 아닌 부분이라면 비율적으로 출력하는 공간인지 구분한다.
    float fRatio = (float) g_int_0 / g_int_1; // hp / max
    
    if (tInput.tUV.x > fRatio)
        clip(-1);
    
    // 나머지는 출력을 하는 곳이다. 초록색 노랑색 빨간색 비율에 따라서 색상을 다르게 준다.
    if(fRatio > 0.666f)
    {
        // Green
        tColor = float4(0.f, 1.f, 0.f, 1.f);
        
    }
    else if(fRatio > 0.333f)
    {
        // Yellow
        tColor = float4(1.f, 1.f, 0.f, 1.f);
        
    }
    else // red 
        tColor = float4(1.f, 0.f, 0.f, 1.f);
    
    return tColor;
}

float4 PS_Standard2D(VTX_OUTPUT tInput) : SV_Target
{
    float4 tColor = (float4) 0.f;

    if (IsAni2D)
    {
        // 레지스터 3번에 들어온 값을 통해서, Animation을 이용하는지 확인한다.

        float2 tUV = g_tLeftTop + (g_tLength * tInput.tUV);
        
        if (g_int_0)
            tUV.x *= -1;
        
        tColor = g_tex_anim2D.Sample(g_sam_1, tUV);
        

    }
    else
    {
        // 기본 텍스처를 사용해준다.
        if (g_int_0)
            tInput.tUV.x *= -1;
        
        tColor = g_tex_0.Sample(g_sam_1, tInput.tUV);

        // 일반 텍스처의 경우에는 보간되어 들어온 UV좌표를 토대로 Mesh에서의 UV좌표를 토대로 찍어준다.
    }

    if(g_float_0)
    {
      
        float3 tLightPow = (float3) 0.f;
        int iLight = 0;
    
        for (int i = 0; i < g_iLight2DCount; ++i)
        {
        // 광원의 수만큼 중첩을 시킨다.
            float fDist = distance(tInput.tWorldPos.xy, g_tLight2DArr[i].tLightPos.xy);
         
            float fRatio = saturate((g_tLight2DArr[i].fRange - fDist) / g_tLight2DArr[i].fRange);

            if (fRatio < 0.7f)
                continue;
            
            if (fDist < g_tLight2DArr[i].fRange)
                ++iLight;
            
        // saturate : 가득한 함수는 0 ~ 1 의 범위로 제한을 거는 함수이다.
        // 0보다 작은 수는 0으로 1보다 큰 수는 1로 고정시킨다.
            
            if (g_tLight2DArr[i].tLightColor.r == 1.f && g_tLight2DArr[i].tLightColor.g == 1.f && g_tLight2DArr[i].tLightColor.b == 1.f)
                fRatio = 1.f;
            
            tLightPow += fRatio * g_tLight2DArr[i].tLightColor.rgb;

        }
    
         // 광원의 영향을 받는 경우 색을 뭍히고, 아닌 경우라면 그냥 출력.
        if (iLight)
        {
            //tColor.rgb *= saturate(1.f - (float) iLight * 0.3f);
            tColor.rgb *= tLightPow;
        }
        else
        {
            if (g_iMorning)
            {
                if (g_fRatio >= 0.5f)
                {
                    tColor.rgb *= (0.2f + 0.8f * (1.f - g_fRatio) * 2.f);
                }
                else
                {
                    tColor.rgb *= (0.2f + 0.8f * (g_fRatio * 2.f));
                }
        
            }
            else
            {
                if (g_fRatio >= 0.5f)
                {
                    tColor.rgb *= (0.2f + 0.8f * (1.f - g_fRatio) * 0.4f);
                }
                else
                {
                    tColor.rgb *= (0.2f + 0.8f * (g_fRatio * 0.4f));
                }
        
            }
        }
        
    }
    
    if (g_float_1)
        tColor.rgb /= g_float_1;
    
     if (g_float_2 && tColor.a != 0.f)
     tColor.a = g_float_2;
    
        return tColor;
}

// =============
// Tile Shader
// g_vec2_0 : TILE X Y SIZE
// g_vec2_1 : TILE IMAGE SIZE (NOT ZERO)
// g_vec2_2 : TILE IMAGE INTERVAL 
// g_vec2_3 : TILEMAP TILE SIZE UV (Static)
// g_tex_0  : TILE TEXTURE

// g_vec4_0 : float2 LTPos, float2 Scale

float4 PS_Tile(VTX_OUTPUT tInput) : SV_Target
{
    float4 tColor = (float4) 0.f;
    
   // TileMap에서 Tile하나의 비율을 가지고 그곳에 있는 픽셀들이 같은 Idx를 참조하도록 한다.
    int iX = tInput.tUV.x / g_vec2_3.x;
    int iY = tInput.tUV.y / g_vec2_3.y;
    
    // 자신의 위치에 해당하는 인덱스는 100 * Y + X이다.
    int idx = iY * 100 + iX;
    
    // 자신이 참조하는 Tile 의 인덱스 위치이다.
    //g_TileBuffer[idx].iX;
    //g_TileBuffer[idx].iY;
    
    /*
    g_TileBuffer[idx].iX
    g_TileBuffer[idx].iY : 해당 위치에서 참조하는 타일의 인덱스이다.
    */
    
    float2 tTileSizeUV = g_vec2_0 / g_vec2_1; // Image에서의 Tile의 Size UV
    float2 tTileGapUV = g_vec2_2 / g_vec2_1; // Image에서의 Tile Gap UV
    
    float2 tLeftTop = (float2) 0.f;
    tLeftTop.x = g_TileBuffer[idx].iX * tTileGapUV.x;
    tLeftTop.y = g_TileBuffer[idx].iY * tTileGapUV.y;
    
    // 각 타일에서의 비율을 구해낸다.
    // 자신이 참조하는 인덱스에 해당하는 LT UV를 얻어서 이것으로 빼버린다.
    
    float2 tUVPer = tInput.tUV - (g_vec2_3 * float2(iX, iY));
    
    // 여기서 나온 값은 가로 1 / 100 세로 1 / 40에서의 
    // UV이다. 따라서 이것을 가로 세로 1 1에서의 비율로 만든다.
    tUVPer.x *= 100.f;
    tUVPer.y *= 40.f;
    
    // tUVPer는 타일의 크기를 1로 보았을때의 Percent 값이다.
    // 위에서 구한 비율을 토대로 하여 각 참조 타일에 연결해준다.
    float2 tUV = tLeftTop + (tTileSizeUV * tUVPer);
    tColor = g_tex_0.Sample(g_sam_1, tUV);
    
    if(tColor.a != 0.f) // 투명이 아니라면, 불투명.
        tColor.a = 1.f;
    
    
 
    
    // 광원에 따른 추가처리
    float3 tLightPow = (float3) 0.f;
    
    // 각 픽셀의 WorldPos를 구한다.
    float2 vWorldPos = g_vec4_0.xy + (tInput.tUV * float2(g_vec4_0.z, -g_vec4_0.w));
    int iLight = 0;
    
    for (int i = 0; i < g_iLight2DCount; ++i)
    {
        // 광원의 수만큼 중첩을 시킨다.
        float fDist = distance(vWorldPos, g_tLight2DArr[i].tLightPos.xy);
        
        float fRatio = saturate((g_tLight2DArr[i].fRange - fDist) / g_tLight2DArr[i].fRange);

        if (fRatio < 0.7f)
            continue;
        
        if (fDist < g_tLight2DArr[i].fRange)
            ++iLight;
        
        // saturate : 가득한 함수는 0 ~ 1 의 범위로 제한을 거는 함수이다.
        // 0보다 작은 수는 0으로 1보다 큰 수는 1로 고정시킨다.
        
        if (g_tLight2DArr[i].tLightColor.r == 1.f && g_tLight2DArr[i].tLightColor.g == 1.f && g_tLight2DArr[i].tLightColor.b == 1.f)
            fRatio = 1.f;

            tLightPow += fRatio * g_tLight2DArr[i].tLightColor.rgb;
    }
    
    
    
    // 광원의 영향을 받는 경우 색을 뭍히고, 아닌 경우라면 그냥 출력.
    if (iLight)
    {
        //tColor.rgb *= saturate(1.f - (float) iLight * 0.2f);
        tColor.rgb *= tLightPow;
    }
    else
    {
        if (g_iMorning)
        {
            if (g_fRatio >= 0.5f)
            {
                tColor.rgb *= (0.2f + 0.8f * (1.f - g_fRatio) * 2.f);
            }
            else
            {
                tColor.rgb *= (0.2f + 0.8f * (g_fRatio * 2.f));
            }
        
        }
        else
        {
            if (g_fRatio >= 0.5f)
            {
                tColor.rgb *= (0.2f + 0.8f * (1.f - g_fRatio) * 0.4f);
            }
            else
            {
                tColor.rgb *= (0.2f + 0.8f * (g_fRatio * 0.4f));
            }
        
        }
    }
 

       // 벽과의 거리에 따른 광원처리.
    if (g_TileBuffer[idx].fGapRatio != 0.f)
    {
        float fRatio = 1.f - (float) g_TileBuffer[idx].fGapRatio;
        tColor.rgb *= fRatio;
    }
    
   
    
    return tColor;
}

// PS_Panel
// g_tex_0 : Output Texture
// g_float_0 : r 가중치
// g_float_1 : g 가중치
// g_float_2 : b 가중치
// g_float_3 : a 값.

float4 PS_Panel(VTX_OUTPUT tInput) : SV_Target
{
    float4 tColor = (float4) 0.f;
    float2 tPanelUV = float2(0.1f, 0.2f);
    
    // 각 위치에 따른 인덱스를 알아낸다.
    int2 tIndex = tInput.tUV / tPanelUV;
    int iIdx = tIndex.y * 10 + tIndex.x;
    
    // 처리하지 않는 경우 투명으로 처리.
    /*
    0 : 패널을 출력하지 않음.
    1 : 출력함.
    2 : 출력하는데, 다른 색상을 지정할 것임. 상수버퍼의 rgb값을 이용.
    
     */

    
    if (g_PanelArr[iIdx].iUse == 0)
        return tColor;
    
    // 각 UV에서 자신의 상대적인 비율을 구한다. (가로 10 세로 5일때의 비율)
    float2 tUV = tInput.tUV % tPanelUV;
    
    // 가로 세로 1의 비율로 변환.
    tUV.x *= 10.f;
    tUV.y *= 5.f;
    
    // 이것을 토대로 샘플링
    tColor = g_tex_0.Sample(g_sam_1, tUV);
    
    if (g_PanelArr[iIdx].iUse == 2)
    {
        if (tColor.r == 0.f && tColor.g == 0.f && tColor.b == 0.f)
            return tColor;
        
        tColor.r = g_PanelArr[iIdx].fRValue;
        tColor.g = g_PanelArr[iIdx].fGValue;
        tColor.b = g_PanelArr[iIdx].fBValue;
    }
    else
    {
        tColor.r *= g_float_0;
        tColor.g *= g_float_1;
        tColor.b *= g_float_2;
    }
    
    if (tColor.a != 0.f)
        tColor.a = g_float_3;
    
    // a 가 0인 곳. 투명을 제외하고는... g_float_3의 영향을 받아서 처리함.
    
    return tColor;
}

// ====================
// Shining Shader
// g_float_0 : r 가중치
// g_float_1 : g 가중치
// g_float_2 : b 가중치
// g_float_3 : rgb / g_float_3



float4 PS_Shining(VTX_OUTPUT tInput) : SV_Target
{
    float4 tColor = (float4) 0.f;

    tColor = g_tex_0.Sample(g_sam_1, tInput.tUV);
    
    tColor.r *= g_float_0;
    tColor.g *= g_float_1;
    tColor.b *= g_float_2;
    tColor.rgb /= g_float_3;
    
    
    return tColor;
}


// =====================
// BackGround Shader (Vertex Shader는 Std2D를 이용)
// g_tex_0 : Texture
// g_float_0 : 부예지는 처리 조정값.
// g_float_1 : 색감 바꾸기 조정값.
// g_float_2 : 색감변화 r

//  g_vec2_0 : 사용 여부 / r 가중치값.
//  g_vec2_1 : 사용 여부 / g 
//  g_vec2_2 : 사용 여부 / b
//  g_vec2_3 :

float4 PS_BackGround(VTX_OUTPUT tInput) : SV_Target
{
    float4 tColor = (float4) 0.f;

    float2 tUV = g_BLeftTop + (g_BLength * tInput.tUV);
    tColor = g_tex_0.Sample(g_sam_1, tUV);

    // uv의 y 값이 클 수록, 더 뿌여지는 처리를 하고 싶다.

    if (tInput.tUV.y <= 0.5f)
        tColor.rgb += (tInput.tUV.y + 0.3f + g_float_0) * 0.6f;
    else
        tColor.rgb += (0.5f + 0.3f + g_float_0) * 0.6f;


    tColor.rgb /= g_float_1;

    if (g_vec2_0.r == 1.f)
        tColor.r *= g_vec2_0.g;

    if (g_vec2_1.r == 1.f)
        tColor.g *= g_vec2_1.g;

    if (g_vec2_2.r == 1.f)
        tColor.b *= g_vec2_2.g;

    tColor.rg *= 1.2f;

    
    if (g_iMorning)
    {
        if (g_fRatio >= 0.5f)
        {
            tColor.rgb *= (0.2f + 0.8f * (1.f - g_fRatio) * 2.f);
        }
        else
        {
            tColor.rgb *= (0.2f + 0.8f * (g_fRatio * 2.f));
        }
        
    }
    else
    {
        if (g_fRatio >= 0.5f)
        {
            tColor.rgb *= (0.2f + 0.8f * (1.f - g_fRatio) * 0.4f);
        }
        else
        {
            tColor.rgb *= (0.2f + 0.8f * (g_fRatio * 0.4f));
        }
        
    }

    return tColor;
}


// ====================
// 2D Shadow Shader
// g_tex_0 : Default Texture
// g_tex_3 : Animation Texture
// alpha blend : true
// ====================

VTX_OUTPUT VS_2DShadow(VTX_INPUT tInput)
{
    VTX_OUTPUT output = (VTX_OUTPUT) 0.f;

    if (tInput.tPos.y == 0.5f)
    {
        // Local좌표로 (투영좌표) 들어온 y값이 0.5라는 것은 RectMesh를 기준으로 ...
        // 위의 두 점이 들어온다.
        
        tInput.tPos.x += 0.8f; // x 좌표를 옮긴다.
        tInput.tPos.y -= 0.5f; // 두 점을 잡아당기고,

        // Local좌표에서 변환이 일어나기 때문에. 살짝 눕혀진 느낌을 표현할 수 있는 것이다. (그림자처럼)
    }

    output.tPos = mul(float4(tInput.tPos, 1.f), g_matWVP);
    output.tUV = tInput.tUV;
    // 원래 하던 것.

    return output;
}

float4 PS_2DShadow(VTX_OUTPUT tInput) : SV_Target
{
    float4 tColor = (float4) 0.f;

    if (IsAni2D)
    {
        float2 tUV = g_tLeftTop + (g_tLength * tInput.tUV);
        tColor = g_tex_anim2D.Sample(g_sam_1, tUV);
    }
    else
    {
        tColor = g_tex_0.Sample(g_sam_1, tInput.tUV);
    }

    // 애니메이션이 있다면, 애니메이션을 UV좌표를 통해서 위치를 구하고, 이것의 Color를 계산.
    // 알파값을 제외하고 대부분을 검은색으로 출력해버린다. (그림자)

    return float4(0.1f, 0.1f, 0.1f, tColor.a);

}


struct VS_PARTICLE_INPUT
{
    float2 vUV : TEXCOORD;
    float3 vPos : POSITION;
};

struct VS_PARTICLE_OUTPUT
{
    float2 vUV : TEXCOORD;
    float4 vPosition : SV_Position; 
};

// ==================
// Particle Shader
// ==================
//
// g_int_0 : 참조 인덱스 X
// g_int_1 : 참조 인덱스 Y
// g_int_2 : Particle Max X
// g_int_3 : Particle Max Y
//
// g_tex_0 : Particle Texture
// ==================

VS_PARTICLE_OUTPUT VS_Particle(VS_PARTICLE_INPUT tInput)
{
    VS_PARTICLE_OUTPUT tOutput = (VS_PARTICLE_OUTPUT) 0.f;
    
    tOutput.vPosition = mul(float4(tInput.vPos, 1.f), g_matWVP);
    tOutput.vUV = tInput.vUV;
    
    return tOutput;
}

float4 PS_Particle(VS_PARTICLE_OUTPUT tInput) : SV_Target
{
    float4 tColor = (float4) 0.f;
    
    if (tex_0)
    {
        float2 vUVSize = float2(1.f / (float) g_int_2, 1.f / (float) g_int_3);
        float2 vUV = vUVSize * float2(g_int_0, g_int_1);
        vUV += vUVSize * tInput.vUV;
        
       tColor = g_tex_0.Sample(g_sam_1, vUV);
       tColor /= 0.7f;
        
        float fAlpha = 1.f - g_float_0;
        
        if (tColor.a != 0.f && fAlpha <= 0.7f)
            tColor.a = fAlpha;
        
        
      
    }
    else
    {
        tColor = float4(1.f, 0.f, 1.f, 1.f);
    }
    
    return tColor;
}

