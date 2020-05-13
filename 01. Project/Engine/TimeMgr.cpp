#include "stdafx.h"
#include "TimeMgr.h"
#include "FontMgr.h"

CTimeMgr::CTimeMgr() 
	: m_llOldCount{}
	, m_llFrequency{}
	, m_fDeltaTime (0.f)
	, m_fAccTime(0.f)
	, m_iFPS(0)
	, m_iFPSCount(0)
	, m_bFreeze(false)
{
}

CTimeMgr::~CTimeMgr()
{
}

bool CTimeMgr::Init()
{
	QueryPerformanceFrequency(&m_llFrequency);
	QueryPerformanceCounter(&m_llOldCount);

	return true;
}

void CTimeMgr::Update()
{
	QueryPerformanceCounter(&m_llCurCount);
	
	m_fDeltaTime = (float)(m_llCurCount.QuadPart - m_llOldCount.QuadPart) / (float)m_llFrequency.QuadPart;
	m_llOldCount = m_llCurCount;

	m_fAccTime += m_fDeltaTime;	

	++m_iFPSCount;

	if (m_fDeltaTime >= 0.1f)
	{
		m_fDeltaTime = 0.f;
	}


	g_tValue.g_fDeltaTime = m_fDeltaTime;
	g_tValue.g_fAccTime += m_fDeltaTime;
}

void CTimeMgr::Render()
{
	if (m_fAccTime >= 1.f)
	{
		// 1.2s -> 120 ¹ø.
		// 1s -> 100    È½¼ö / ÃÊ.
		// 1200 / 12 - > 100

		m_iFPS = m_iFPSCount / m_fAccTime;
		m_fAccTime -= 1.f;
		m_iFPSCount = 0;
	}

	wchar_t szFPS[256] = {};

	wsprintf(szFPS, L"FPS : %d", (UINT)m_iFPS);
	CFontMgr::GetInst()->DrawFont(szFPS, 10.f, 100.f, 16.f, FONT_RGBA(255, 0, 0, 255));
}
