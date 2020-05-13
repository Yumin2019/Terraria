#pragma once
#pragma once

#include <FontEngine/FW1FontWrapper.h>
#include <FontEngine/FW1CompileSettings.h>

#ifdef _DEBUG
#pragma comment(lib, "FontEngine/FW1FontWrapper_Debug.lib")
#else
#pragma comment(lib, "FontEngine/FW1FontWrapper.lib")
#endif

/*
IDX는 ENUM문을 가지고 사용하도록 한다.
*/

#define FONT_RGBA(r, g, b, a) (((((BYTE)a << 24) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)

class CFontMgr
{
	SINGLE(CFontMgr);

private:
	IFW1Factory*			 m_pFW1Factory;
	IFW1FontWrapper*		 m_pDefault;
	vector<IFW1FontWrapper*> m_vecFont;

public:
	bool Init();
	void AddFont(const wchar_t* pFontName);
	void DrawFont(UINT iIdx, wchar_t* pStr, float fPosX, float fPosY, float fSize, UINT iColor);
	void DrawFont(wchar_t* pStr, float fPosX, float fPosY, float fSize, UINT iColor);
};

