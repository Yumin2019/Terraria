#pragma once

typedef struct _tagFontInfo
{
	wstring strOutput;

	// PlayTime에 따라서 속도가 조절이 된다.
	float	fPlayTime;
	float	fTime;

	// WorldPos
	Vec2	vStartPos;
	Vec2	vEndPos;

	// Font Scale
	float	fStartScale;
	float	fEndScale;

	// Color
	Vec4	vStartColor;
	Vec4	vEndColor;

	_tagFontInfo() :
		fPlayTime(0.f),
		fTime(0.f),
		fStartScale(0.f),
		fEndScale(0.f)
	{

	}

	_tagFontInfo(const wstring& strOutput, float fPlayTime, const Vec2& vStart, const Vec2& vEnd,
		float fStartScale, float fEndScale, const Vec4& vStartColor, const Vec4& vEndColor)
		: strOutput(strOutput),
		fPlayTime(fPlayTime),
		vStartPos(vStart),
		vEndPos(vEnd),
		fTime(0.f),
		fStartScale(fStartScale),
		fEndScale(fEndScale),
		vStartColor(vStartColor),
		vEndColor(vEndColor)
	{}

}FontInfo, *PFontInfo;

typedef struct _tagItemFont
{
	FontInfo  tInfo;
	bool	  bEnable;
	int		  iCount; // ItemCount

	int		  iFontIdx;
	

	_tagItemFont() :
		bEnable(false),
		iCount(0),
		tInfo()
	{}

}ItemFont, *PItemFont;

class CUtilMgr
{
	SINGLE(CUtilMgr);

private:
	list<FontInfo> m_FontList;
	ItemFont	   m_arrItemFont[(UINT)ITEM::NUM_END];
	int			   m_iItemFont; //ItemFont의 개수.
	Vec2		   m_vFirstPos; // 첫째 아이템의 Pos

public:
	bool Init();
	void Render(); // Font처리.

	// 해당 아이템에 대한 Font가 존재하는지 여부
	bool IsItemFont(ITEM eItem) { return m_arrItemFont[(UINT)eItem].bEnable; }

	// 해당 아이템 Font를 새로운 정보로 셋팅.
	void SetItemFont(ITEM eItem, const wstring& strOutput, float fPlayTime, const Vec2& vStartPos, const Vec2& vEndPos,
		float fStartScale, float fEndScale, const Vec4& vStartColor, const Vec4& vEndColor);

	ItemFont& GetItemFont(ITEM eItem) { return m_arrItemFont[(UINT)eItem]; }

	// ItemObjScript에서 호출하기 위한 용도의 함수.
	// Font의 정보는 내부적으로 처리하고 지금 몇 개의 아이템이 들어왔는지 판단한다.
	void AddItem(ITEM eItem, int iCount, const Vec2& vPos);

	// List에 ItemFont를 추가.
	void AddFont(const wstring& strOutput, float fPlayTime, const Vec2& vStartPos, const Vec2& vEndPos,
		float fStartScale, float fEndScale, const Vec4& vStartColor, const Vec4& vEndColor);
};

