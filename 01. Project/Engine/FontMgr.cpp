#include "stdafx.h"
#include "FontMgr.h"

#include "Device.h"

CFontMgr::CFontMgr()
{
}


CFontMgr::~CFontMgr()
{
	// ���������� Release�� ������.
	SAFE_RELEASE(m_pFW1Factory);

	SAFE_RELEASE(m_pDefault);

	for (UINT i = 0; i < m_vecFont.size(); ++i)
	{
		SAFE_RELEASE(m_vecFont[i]);
	}
}

bool CFontMgr::Init()
{
	// Factory �� ���� ó���� Engine���� ���.
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_pFW1Factory)))
	{
		assert(FALSE);
	}

	// Engine���� ����� �⺻ ��Ʈ�� �����صд�.
	if (FAILED(m_pFW1Factory->CreateFontWrapper(DEVICE, L"Arial", &m_pDefault)))
	{
		assert(FALSE);
	}

	return true;
}

void CFontMgr::AddFont(const wchar_t * pFontName)
{
	// �̸� �÷��� ��Ʈ���� �Ѵ�.  (Windows/Fonts/~)
	IFW1FontWrapper* pFont = nullptr;

	if (FAILED(m_pFW1Factory->CreateFontWrapper(DEVICE, pFontName, &pFont)))
	{
		assert(FALSE);
	}

	m_vecFont.push_back(pFont);
}

void CFontMgr::DrawFont(UINT iIdx, wchar_t * pStr, float fPosX, float fPosY, float fSize, UINT iColor)
{
	 m_vecFont[iIdx]->DrawString(
		CONTEXT,
		pStr, // String
		fSize,// Font size
		fPosX,// X position
		fPosY,// Y position
		iColor,// Text color, 0xAaBbGgRr
		0      // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);

}

void CFontMgr::DrawFont(wchar_t * pStr, float fPosX, float fPosY, float fSize, UINT iColor)
{
	m_pDefault->DrawString(
		CONTEXT,
		pStr, // String
		fSize,// Font size
		fPosX,// X position
		fPosY,// Y position
		iColor,// Text color, 0xAaBbGgRr
		0      // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}
