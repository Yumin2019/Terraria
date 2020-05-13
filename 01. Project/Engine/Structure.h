#pragma once

typedef struct _tagGlobalValue
{
	Vec2 g_vResolution;
	float g_fDeltaTime;
	float g_fAccTime;

	float g_iMorning;
	float g_fRatio;
	Vec2 vPadding;

}GlobalValue, *PGlovalValue;

typedef struct _tagResolution
{
	float fWidth;
	float fHeight;

	_tagResolution() :
		fWidth(0.f),
		fHeight(0.f)
	{

	}

	_tagResolution(int x, int y) 
	{
		fWidth = (float)x;
		fHeight = (float)y;
	}

}Resolution, *PResolution;

// Vertex Structure 
typedef struct _tagVertex
{
	Vec3 tPos;
	Vec4 tColor;
	Vec2 tUVPos;

}Vertex, *PVertex;

// 상수 버퍼 메모리
typedef struct _tagTransform
{
	Matrix matWorld; // To WorldSpace
	Matrix matView; // To ViewSpace
	Matrix matProj; // To ProjectionSpae

	Matrix matWV;
	Matrix matWVP;

}Transform, *PTransform;

typedef struct _tagMtrlParam
{
	Vec4			m_tDiff; // Diffusion 확산 색상 계수.
	Vec4			m_tSpec; // 반사광 계수.
	Vec4			m_tEmv; // 발광 계수.

	// 0 ~ 3번까지, Shader로 넘길 데이터에 대한 정보이다.
	int					m_IntArray[(UINT)SHADER_PARAM::INT_END - (UINT)SHADER_PARAM::INT_0];
	float				m_FloatArray[(UINT)SHADER_PARAM::FLOAT_END - (UINT)SHADER_PARAM::FLOAT_0];
	Vec2				m_Vec2Array[(UINT)SHADER_PARAM::VEC2_END - (UINT)SHADER_PARAM::VEC2_0];
	Vec4				m_Vec4Array[(UINT)SHADER_PARAM::VEC4_END - (UINT)SHADER_PARAM::VEC4_0];
	Matrix				m_MatArray[(UINT)SHADER_PARAM::MATRIX_END - (UINT)SHADER_PARAM::MATRIX_0];

	int					m_iTexArr[(UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0];

}MtrlParam, *PMtrlParam;

// Debug 메세지를 출력할 용도의 구조체
typedef struct _tagDebug
{
	DBG_TYPE eType;
	wstring strDBG;

	_tagDebug(DBG_TYPE eType, const wstring& strDBG) :
		eType(eType),
		strDBG(strDBG)
	{}

}Debug, *PDebug;

typedef struct _tagAni2D
{
	Vec2 tOffset;
	Vec2 tLeftTop;
	Vec2 tLength; // 16
	int IsAni2D;
	int iPadding;

}Ani2D, *PAni2D;

typedef struct _tagPanelBuffer
{
	int iUse;

	float fRValue;
	float fGValue;
	float fBValue;

}PanelBuffer, *PPanelBuffer;

typedef struct _tagBackGround
{
	Vec2 tLeftTop;
	Vec2 tLength;
	
}BackGround, *PBackGround;

typedef struct _tagTileBuffer
{
	int		iX; // 이미지에서의 참조 Tile Idx
	int		iY;
	float	fGapRatio; // 벽과의 차이가 얼마나 나는가 ?에 따른 감소비율
	int		iPadding;

}TileBuffer, *PTileBuffer;

typedef struct _tagTileInfo
{
	// 표현하고 싶은 정보.
	// 캘수 있는 (ex : water) bool (즉 마우스 충돌 여부)
	// 지나갈 수 있는 (ex : wall) bool (오브젝트 충돌 여부)
	// 아이템이 지정된. flag

	bool	m_bDigable;
	bool	m_bBlock;
	bool	m_bNatural;
	ITEM	m_eItem;

	_tagTileInfo() :
		m_bDigable(true),
		m_bBlock(true),
		m_eItem(ITEM::NONE),
		m_bNatural(true)
	{}

	_tagTileInfo(bool bDigable, bool bBlock, ITEM eItem, bool bNatural = false) :
		m_bDigable(bDigable),
		m_bBlock(bBlock),
		m_eItem(eItem),
		m_bNatural(bNatural)
	{}

}TileInfo, *PTileInfo;

typedef struct _tagLight2DInfo
{
	Vec4 tLightPos;
	Vec4 tLightColor;
	Vec4 tLightDir;
	int iLightType;
	float fRange;
	float fAngle;
	int ipadding;
}Light2DInfo, *PLight2DInfo;

