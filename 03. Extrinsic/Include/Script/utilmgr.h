#pragma once

typedef struct _tagFontInfo
{
	wstring strOutput;

	// PlayTime�� ���� �ӵ��� ������ �ȴ�.
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
	int			   m_iItemFont; //ItemFont�� ����.
	Vec2		   m_vFirstPos; // ù° �������� Pos

public:
	bool Init();
	void Render(); // Fontó��.

	// �ش� �����ۿ� ���� Font�� �����ϴ��� ����
	bool IsItemFont(ITEM eItem) { return m_arrItemFont[(UINT)eItem].bEnable; }

	// �ش� ������ Font�� ���ο� ������ ����.
	void SetItemFont(ITEM eItem, const wstring& strOutput, float fPlayTime, const Vec2& vStartPos, const Vec2& vEndPos,
		float fStartScale, float fEndScale, const Vec4& vStartColor, const Vec4& vEndColor);

	ItemFont& GetItemFont(ITEM eItem) { return m_arrItemFont[(UINT)eItem]; }

	// ItemObjScript���� ȣ���ϱ� ���� �뵵�� �Լ�.
	// Font�� ������ ���������� ó���ϰ� ���� �� ���� �������� ���Դ��� �Ǵ��Ѵ�.
	void AddItem(ITEM eItem, int iCount, const Vec2& vPos);

	// List�� ItemFont�� �߰�.
	void AddFont(const wstring& strOutput, float fPlayTime, const Vec2& vStartPos, const Vec2& vEndPos,
		float fStartScale, float fEndScale, const Vec4& vStartColor, const Vec4& vEndColor);
};

