
// HLSL 

#include "value.fx"

/*
	Vec4			m_tDiff; // Diffusion Ȯ�� ���� ���.
	Vec4			m_tSpec; // �ݻ籤 ���.
	Vec4			m_tEmv; // �߱� ���.

	// 0 ~ 3������, Shader�� �ѱ� �����Ϳ� ���� �����̴�.
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

	// float3 .. float4 �� HLSL ���� �����ϴ� ����ü�̴�.
};

struct VS_COL_OUTPUT
{
	// ����� float4���·� �Ѱܾ� �Ѵ�.
    float4 tPos : SV_Position; // SYSTEM_VALUE POS
    float4 tColor : COLOR;
};

// Vertex Color Shader
VS_COL_OUTPUT VS_Color(VS_COL_INPUT tInput)
{
    VS_COL_OUTPUT output = (VS_COL_OUTPUT) 0; // HLSL���� ����ü�� �ʱ�ȭ�ϴ� ����̴�. 

	//// ���� ��ǥ�踦 ��ȯ�� ������ FLOAT4 4��° ��ҿ� 1.f ���� �־��ش�.
 //   output.tPos = float4(tInput.tPos, 1.f); // HLSL�� ���������� �پ��� �����ڸ� �⺻������ �������ְ� �ִ�.
 //   output.tColor = tInput.tColor;


    // �Ѿ�� ������ ��ǥ (Local)���� w�ڸ��� 1.f�� �־ �������� �����ϵ��� �Ѵ�.
    // ���Ŀ� WorldMatrix�� ���Ͽ�, WorldSpace�� �������� ��ü�� �̵���Ų��. (�� ��ǥ�� ��Ī�Ǵ� �ڸ��� 1�� ���� ������-> �������)
    // 4�࿡�� �������� �����ϰ� �ִ�. 
    // ���⼭ �� �ڸ��� �κп����� ���� ��ȯ�� ������ �� �ִ�.
    // �ƹ�ư �̵��ϴ� ��ǥ�� (0 + dX, 0 + dY, 0 + dZ, 1.f)�� �� ���̴�.
    // World���� ��ȯ �������� ȸ�� / Scaling�� �� �� �ִ�. 
    float4 vWorldPos = mul(float4(tInput.tPos, 1.f), g_matWorld);

    // WorldSpace�� CameraSpace�� ǥ���Ѵ�. ����� ���������� ī�޶��� ��ǥ�� �������� �̵���Ű�� ���Ϳ� ���� ������ 
    // ���������� 4�࿡ �����ϰ� ������, ȸ�� ��Ŀ� ���ؼ��� �ϰ� ���� �ʴ�.
    float4 vViewPos = mul(vWorldPos, g_matView);

    // CameraSpace�� Projection Space�� ǥ���Ѵ�. 
    // ���߿� .
    float4 vProjPos = mul(vViewPos, g_matProj);

    output.tColor = tInput.tColor;
    output.tPos = vProjPos;


    return output;

	/*
	���� : VS_COL_OUTPUT ����ü�� ���·�  ���� ��ȯ�Ѵ�. 
	������ ���� �ý��� �޸𸮿��� �Ѿ���� VS_COL_INPUT�� �����̴�.
	�̷��� ���� ��ǥ�� ���ؼ� �ƹ��� ó���� ���� �ʰ�(�̹� ������ǥ��� ��Ƶξ�����) �״�� ���� VS_COL_OUTPUT ���·�
	��ȯ���ش�.
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
    // ���ϴ� �������� �������� �̾Ƽ� ����� �� �ִ�.
};

struct TEX_OUTPUT
{
    float4 tOutPos : SV_Position;
    float2 tUVPos : TEXCOORD;
};

TEX_OUTPUT VS_Tex(TEX_INPUT tInput)
{
    TEX_OUTPUT output = (TEX_OUTPUT) 0; // HLSL���� ����ü�� �ʱ�ȭ�ϴ� ����̴�. 
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

    // UVPos�� ���� ������ ���� �Ѿ�ͼ�, �׸��� �ش��ϴ� �ȼ� ������ ��ȯ.
    // �Ѱ��� �׸��� �ؽ�ó�� ũ�⿡ �°� ��ȯ�Ͽ�, �ش� ��ġ�� �ȼ��� ����ش�.
    if (g_float_0)
        tColor.rgb /= g_float_0;
    
    if (g_int_1) // DayLight����
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
    float3 tWorldPos : POSITION; // PixelShader������ Pixel�� WorldPos�� �ޱ� ���Ѵ�.
};


VTX_OUTPUT VS_Standard2D(VTX_INPUT tInput)
{
    VTX_OUTPUT output = (VTX_OUTPUT) 0.f;
   
    float4 tPosition = (float4) 0.f;

    if (IsAni2D)
    {
        // �ִϸ��̼��� ����ϴ� ��쿡�� ��������� ���ؼ� ��ǥ�� ���ϰ� Offset�� ���ؼ� �����ġ�� �����Ѵ�.
        // �������� ������� �ʰ� �߰��� �� �������� ������ Offset�� ���ؼ� �����Ͽ� �����ġ�� ���Ѵٴ� �ǹ��̴�.
        // ���ҽ����� ����̵Ǵ� ��ġ�� �����ϴ� ������ �ƴ϶�, �����ġ�� Shader�ܿ��� ��ü�ϴ� �����̴�.
        // ����, ������ �ִϸ��̼Ǹ� �߰��� �ص� ���Ŀ� Offset�� ���߾� ó���� �� �ִ�.
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

    //  Standard 2D Vertex �Լ������� WorldMatrix�� ���� ������ World��ǥ�� ���صд�.

    return output;
}

// TitleShader
//
//
//

float4 PS_Title(VTX_OUTPUT tInput) : SV_Target
{
    float4 tColor = (float4) 0.f;

    // x�� AccTime�� ������ �޵��� �Ѵ�. Right To Left
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
    
    // 5���� �ش��ϴ� Idx�� ���Ѵ�.
    int iIdx = tInput.tUV.x / 0.2f;
    
    // 5������ ������ �ִ�.
    int iHeartSize = g_int_1 / 5; // 20
    int iHeartCount = g_int_0 / iHeartSize; // HP / HeartSize(���� ��ġ) -> �� ��ŭ�� ��Ʈ ��ġ ����. 70 / 20 - > 3
    
    // �ڽ��� ��ġ�� ����� �Ǿ�� �ϴ��� �Ǵ��Ѵ�.
    if (iIdx <= iHeartCount)
    {
        tInput.tUV.x = (tInput.tUV.x % 0.2f) * 5.f;
        tColor = g_tex_0.Sample(g_sam_1, tInput.tUV);
        
        if (iIdx == iHeartCount)
        {
            // �� ��쿡�� Alpha���� ������ �޵��� ó���Ѵ�.
            int iLeftOver = g_int_0 % iHeartSize; // 70 % 20 -> 10 
            float fRatio = (float) iLeftOver / iHeartSize; // ���� ���� ü�� / (���� ��ġ) -> ������ ���س��� (Alpha������ �̿�)
            
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
    
    // �ִ�ü���� ���� ������� �ʴ´�.
    if (g_int_0 == g_int_1)
        clip(-1);
    
    tColor = g_tex_0.Sample(g_sam_0, tInput.tUV);
        
    // Border�� �ش��ϴ� �κ��� �׳� ���.
    if (tColor.r == 0.f && tColor.g == 0.f && tColor.b == 0.f)
        return tColor;
    
    // �ƴ� �κ��̶�� ���������� ����ϴ� �������� �����Ѵ�.
    float fRatio = (float) g_int_0 / g_int_1; // hp / max
    
    if (tInput.tUV.x > fRatio)
        clip(-1);
    
    // �������� ����� �ϴ� ���̴�. �ʷϻ� ����� ������ ������ ���� ������ �ٸ��� �ش�.
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
        // �������� 3���� ���� ���� ���ؼ�, Animation�� �̿��ϴ��� Ȯ���Ѵ�.

        float2 tUV = g_tLeftTop + (g_tLength * tInput.tUV);
        
        if (g_int_0)
            tUV.x *= -1;
        
        tColor = g_tex_anim2D.Sample(g_sam_1, tUV);
        

    }
    else
    {
        // �⺻ �ؽ�ó�� ������ش�.
        if (g_int_0)
            tInput.tUV.x *= -1;
        
        tColor = g_tex_0.Sample(g_sam_1, tInput.tUV);

        // �Ϲ� �ؽ�ó�� ��쿡�� �����Ǿ� ���� UV��ǥ�� ���� Mesh������ UV��ǥ�� ���� ����ش�.
    }

    if(g_float_0)
    {
      
        float3 tLightPow = (float3) 0.f;
        int iLight = 0;
    
        for (int i = 0; i < g_iLight2DCount; ++i)
        {
        // ������ ����ŭ ��ø�� ��Ų��.
            float fDist = distance(tInput.tWorldPos.xy, g_tLight2DArr[i].tLightPos.xy);
         
            float fRatio = saturate((g_tLight2DArr[i].fRange - fDist) / g_tLight2DArr[i].fRange);

            if (fRatio < 0.7f)
                continue;
            
            if (fDist < g_tLight2DArr[i].fRange)
                ++iLight;
            
        // saturate : ������ �Լ��� 0 ~ 1 �� ������ ������ �Ŵ� �Լ��̴�.
        // 0���� ���� ���� 0���� 1���� ū ���� 1�� ������Ų��.
            
            if (g_tLight2DArr[i].tLightColor.r == 1.f && g_tLight2DArr[i].tLightColor.g == 1.f && g_tLight2DArr[i].tLightColor.b == 1.f)
                fRatio = 1.f;
            
            tLightPow += fRatio * g_tLight2DArr[i].tLightColor.rgb;

        }
    
         // ������ ������ �޴� ��� ���� ������, �ƴ� ����� �׳� ���.
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
    
   // TileMap���� Tile�ϳ��� ������ ������ �װ��� �ִ� �ȼ����� ���� Idx�� �����ϵ��� �Ѵ�.
    int iX = tInput.tUV.x / g_vec2_3.x;
    int iY = tInput.tUV.y / g_vec2_3.y;
    
    // �ڽ��� ��ġ�� �ش��ϴ� �ε����� 100 * Y + X�̴�.
    int idx = iY * 100 + iX;
    
    // �ڽ��� �����ϴ� Tile �� �ε��� ��ġ�̴�.
    //g_TileBuffer[idx].iX;
    //g_TileBuffer[idx].iY;
    
    /*
    g_TileBuffer[idx].iX
    g_TileBuffer[idx].iY : �ش� ��ġ���� �����ϴ� Ÿ���� �ε����̴�.
    */
    
    float2 tTileSizeUV = g_vec2_0 / g_vec2_1; // Image������ Tile�� Size UV
    float2 tTileGapUV = g_vec2_2 / g_vec2_1; // Image������ Tile Gap UV
    
    float2 tLeftTop = (float2) 0.f;
    tLeftTop.x = g_TileBuffer[idx].iX * tTileGapUV.x;
    tLeftTop.y = g_TileBuffer[idx].iY * tTileGapUV.y;
    
    // �� Ÿ�Ͽ����� ������ ���س���.
    // �ڽ��� �����ϴ� �ε����� �ش��ϴ� LT UV�� �� �̰����� ��������.
    
    float2 tUVPer = tInput.tUV - (g_vec2_3 * float2(iX, iY));
    
    // ���⼭ ���� ���� ���� 1 / 100 ���� 1 / 40������ 
    // UV�̴�. ���� �̰��� ���� ���� 1 1������ ������ �����.
    tUVPer.x *= 100.f;
    tUVPer.y *= 40.f;
    
    // tUVPer�� Ÿ���� ũ�⸦ 1�� ���������� Percent ���̴�.
    // ������ ���� ������ ���� �Ͽ� �� ���� Ÿ�Ͽ� �������ش�.
    float2 tUV = tLeftTop + (tTileSizeUV * tUVPer);
    tColor = g_tex_0.Sample(g_sam_1, tUV);
    
    if(tColor.a != 0.f) // ������ �ƴ϶��, ������.
        tColor.a = 1.f;
    
    
 
    
    // ������ ���� �߰�ó��
    float3 tLightPow = (float3) 0.f;
    
    // �� �ȼ��� WorldPos�� ���Ѵ�.
    float2 vWorldPos = g_vec4_0.xy + (tInput.tUV * float2(g_vec4_0.z, -g_vec4_0.w));
    int iLight = 0;
    
    for (int i = 0; i < g_iLight2DCount; ++i)
    {
        // ������ ����ŭ ��ø�� ��Ų��.
        float fDist = distance(vWorldPos, g_tLight2DArr[i].tLightPos.xy);
        
        float fRatio = saturate((g_tLight2DArr[i].fRange - fDist) / g_tLight2DArr[i].fRange);

        if (fRatio < 0.7f)
            continue;
        
        if (fDist < g_tLight2DArr[i].fRange)
            ++iLight;
        
        // saturate : ������ �Լ��� 0 ~ 1 �� ������ ������ �Ŵ� �Լ��̴�.
        // 0���� ���� ���� 0���� 1���� ū ���� 1�� ������Ų��.
        
        if (g_tLight2DArr[i].tLightColor.r == 1.f && g_tLight2DArr[i].tLightColor.g == 1.f && g_tLight2DArr[i].tLightColor.b == 1.f)
            fRatio = 1.f;

            tLightPow += fRatio * g_tLight2DArr[i].tLightColor.rgb;
    }
    
    
    
    // ������ ������ �޴� ��� ���� ������, �ƴ� ����� �׳� ���.
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
 

       // ������ �Ÿ��� ���� ����ó��.
    if (g_TileBuffer[idx].fGapRatio != 0.f)
    {
        float fRatio = 1.f - (float) g_TileBuffer[idx].fGapRatio;
        tColor.rgb *= fRatio;
    }
    
   
    
    return tColor;
}

// PS_Panel
// g_tex_0 : Output Texture
// g_float_0 : r ����ġ
// g_float_1 : g ����ġ
// g_float_2 : b ����ġ
// g_float_3 : a ��.

float4 PS_Panel(VTX_OUTPUT tInput) : SV_Target
{
    float4 tColor = (float4) 0.f;
    float2 tPanelUV = float2(0.1f, 0.2f);
    
    // �� ��ġ�� ���� �ε����� �˾Ƴ���.
    int2 tIndex = tInput.tUV / tPanelUV;
    int iIdx = tIndex.y * 10 + tIndex.x;
    
    // ó������ �ʴ� ��� �������� ó��.
    /*
    0 : �г��� ������� ����.
    1 : �����.
    2 : ����ϴµ�, �ٸ� ������ ������ ����. ��������� rgb���� �̿�.
    
     */

    
    if (g_PanelArr[iIdx].iUse == 0)
        return tColor;
    
    // �� UV���� �ڽ��� ������� ������ ���Ѵ�. (���� 10 ���� 5�϶��� ����)
    float2 tUV = tInput.tUV % tPanelUV;
    
    // ���� ���� 1�� ������ ��ȯ.
    tUV.x *= 10.f;
    tUV.y *= 5.f;
    
    // �̰��� ���� ���ø�
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
    
    // a �� 0�� ��. ������ �����ϰ��... g_float_3�� ������ �޾Ƽ� ó����.
    
    return tColor;
}

// ====================
// Shining Shader
// g_float_0 : r ����ġ
// g_float_1 : g ����ġ
// g_float_2 : b ����ġ
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
// BackGround Shader (Vertex Shader�� Std2D�� �̿�)
// g_tex_0 : Texture
// g_float_0 : �ο����� ó�� ������.
// g_float_1 : ���� �ٲٱ� ������.
// g_float_2 : ������ȭ r

//  g_vec2_0 : ��� ���� / r ����ġ��.
//  g_vec2_1 : ��� ���� / g 
//  g_vec2_2 : ��� ���� / b
//  g_vec2_3 :

float4 PS_BackGround(VTX_OUTPUT tInput) : SV_Target
{
    float4 tColor = (float4) 0.f;

    float2 tUV = g_BLeftTop + (g_BLength * tInput.tUV);
    tColor = g_tex_0.Sample(g_sam_1, tUV);

    // uv�� y ���� Ŭ ����, �� �ѿ����� ó���� �ϰ� �ʹ�.

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
        // Local��ǥ�� (������ǥ) ���� y���� 0.5��� ���� RectMesh�� �������� ...
        // ���� �� ���� ���´�.
        
        tInput.tPos.x += 0.8f; // x ��ǥ�� �ű��.
        tInput.tPos.y -= 0.5f; // �� ���� ��ƴ���,

        // Local��ǥ���� ��ȯ�� �Ͼ�� ������. ��¦ ������ ������ ǥ���� �� �ִ� ���̴�. (�׸���ó��)
    }

    output.tPos = mul(float4(tInput.tPos, 1.f), g_matWVP);
    output.tUV = tInput.tUV;
    // ���� �ϴ� ��.

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

    // �ִϸ��̼��� �ִٸ�, �ִϸ��̼��� UV��ǥ�� ���ؼ� ��ġ�� ���ϰ�, �̰��� Color�� ���.
    // ���İ��� �����ϰ� ��κ��� ���������� ����ع�����. (�׸���)

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
// g_int_0 : ���� �ε��� X
// g_int_1 : ���� �ε��� Y
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

