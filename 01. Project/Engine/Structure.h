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

// ��� ���� �޸�
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
	Vec4			m_tDiff; // Diffusion Ȯ�� ���� ���.
	Vec4			m_tSpec; // �ݻ籤 ���.
	Vec4			m_tEmv; // �߱� ���.

	// 0 ~ 3������, Shader�� �ѱ� �����Ϳ� ���� �����̴�.
	int					m_IntArray[(UINT)SHADER_PARAM::INT_END - (UINT)SHADER_PARAM::INT_0];
	float				m_FloatArray[(UINT)SHADER_PARAM::FLOAT_END - (UINT)SHADER_PARAM::FLOAT_0];
	Vec2				m_Vec2Array[(UINT)SHADER_PARAM::VEC2_END - (UINT)SHADER_PARAM::VEC2_0];
	Vec4				m_Vec4Array[(UINT)SHADER_PARAM::VEC4_END - (UINT)SHADER_PARAM::VEC4_0];
	Matrix				m_MatArray[(UINT)SHADER_PARAM::MATRIX_END - (UINT)SHADER_PARAM::MATRIX_0];

	int					m_iTexArr[(UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0];

}MtrlParam, *PMtrlParam;

// Debug �޼����� ����� �뵵�� ����ü
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
	int		iX; // �̹��������� ���� Tile Idx
	int		iY;
	float	fGapRatio; // ������ ���̰� �󸶳� ���°� ?�� ���� ���Һ���
	int		iPadding;

}TileBuffer, *PTileBuffer;

typedef struct _tagTileInfo
{
	// ǥ���ϰ� ���� ����.
	// Ķ�� �ִ� (ex : water) bool (�� ���콺 �浹 ����)
	// ������ �� �ִ� (ex : wall) bool (������Ʈ �浹 ����)
	// �������� ������. flag

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

