#include "stdafx.h"
#include "UtilMgr.h"
#include <Engine/FontMgr.h>
#include <Engine/TimeMgr.h>
#include <Engine/Transform.h>
#include <Engine/Device.h>

#include "MainCameraScript.h"
#include "DayScript.h"

CUtilMgr::CUtilMgr() :
	m_arrItemFont{}
{
}


CUtilMgr::~CUtilMgr()
{
}

void CUtilMgr::SetItemFont(ITEM eItem, const wstring & strOutput, 
	float fPlayTime, const Vec2 & vStartPos, const Vec2 & vEndPos,
	float fStartScale, float fEndScale, 
	const Vec4 & vStartColor, const Vec4 & vEndColor)
{
	FontInfo tFont = FontInfo(strOutput, fPlayTime,
		vStartPos, vEndPos, fStartScale,
		fEndScale, vStartColor, vEndColor); // Color : 0 ~ 255 RGB

	m_arrItemFont[(UINT)eItem].tInfo = tFont;
	m_arrItemFont[(UINT)eItem].bEnable = true;
}

void CUtilMgr::AddItem(ITEM eItem, int iCount, const Vec2& vPos)
{
	wstring strFont = ITEM_NAME[(UINT)eItem];
	Vec2 vCenter = vPos;

	// ���� �����ۿ� ���� Font�� �ִ��� ������ �Ǵ��Ѵ�.
	if (IsItemFont(eItem))
	{
		// ���� �����ۿ� ���� Font�� �ִٸ�, Font�� ������ �ٽ� �����ϵ��� �Ѵ�.
		m_arrItemFont[(UINT)eItem].iCount += iCount;
	}
	else
	{
		if (m_iItemFont == 0)
		{
			m_vFirstPos = vPos;
			m_vFirstPos.y += 50.f;
		}

		// ���� �����ۿ� ���� Font�� ���ٸ�, Font�� ������ ó������ �����Ѵ�.
		m_arrItemFont[(UINT)eItem].iCount = iCount;
		m_arrItemFont[(UINT)eItem].iFontIdx = m_iItemFont;
		++m_iItemFont;
	}

	if(m_arrItemFont[(UINT)eItem].iCount > 1)
		strFont += L" (" + to_wstring(m_arrItemFont[(UINT)eItem].iCount) + L")"; // �̸� (����)
	// 1���� ��쿡�� �̸���.

	vCenter.y = m_vFirstPos.y + m_arrItemFont[(UINT)eItem].iFontIdx * 25.f;

	m_arrItemFont[(UINT)eItem].tInfo.strOutput = strFont;
	m_arrItemFont[(UINT)eItem].tInfo.fTime = 0.f;		// ó�� ���� or �̾ ���� ��� ����
	m_arrItemFont[(UINT)eItem].tInfo.vStartPos = vCenter;		// ��ġ ����
	m_arrItemFont[(UINT)eItem].tInfo.vEndPos = vCenter;
	m_arrItemFont[(UINT)eItem].bEnable = true;


}

void CUtilMgr::AddFont(const wstring & strOutput, float fPlayTime,
	const Vec2 & vStartPos, const Vec2 & vEndPos,
	float fStartScale, float fEndScale,
	const Vec4 & vStartColor, const Vec4 & vEndColor)
{
	FontInfo tFont = FontInfo(strOutput, fPlayTime,
		vStartPos, vEndPos, fStartScale,
		fEndScale, vStartColor, vEndColor); // Color : 0 ~ 255 RGB
	m_FontList.push_back(tFont);
}

bool CUtilMgr::Init()
{
	m_FontList.clear(); 


	Vec4 vColor = Vec4(255.f, 255.f, 255.f, 255.f);
	m_iItemFont = 0;

	// �� ItemFont���� �ʿ��� ������ �̸� �����Ѵ�
	for (UINT i = 0; i < (UINT)ITEM::NUM_END; ++i)
	{
		m_arrItemFont[i].bEnable = false;
		m_arrItemFont[i].iCount = 0;
		m_arrItemFont[i].iFontIdx = 0;
		m_arrItemFont[i].tInfo.fPlayTime = 1.f;
	}

	// ó�� �����ϴ� ��Ȳ���� �ʿ��� ó���� ������. 
	CDayScript::g_eScene = SCENE_TYPE::TITLE;

	return true;
}

void CUtilMgr::Render()
{
	float fTime = DT;
	Vec2 vCamLT = Vec2(CMainCameraScript::g_vMainCamPos);
	static Resolution tRes = CDevice::GetInst()->GetResolution();
	static wchar_t strOutput[100] = {};

	vCamLT.x -= tRes.fWidth * 0.5f;
	vCamLT.y += tRes.fHeight * 0.5f;

	auto iter = m_FontList.begin();
	auto iterEnd = m_FontList.end();

	for (; iter != iterEnd; )
	{
		iter->fTime += fTime;
		if (iter->fTime >= iter->fPlayTime)
		{
			iter = m_FontList.erase(iter);
			iterEnd = m_FontList.end();
			continue;
		}

		float fRatio = iter->fTime / iter->fPlayTime;
		float fCurScale = (iter->fEndScale - iter->fStartScale) * fRatio + iter->fStartScale;
		Vec4  vCurColor = (iter->vEndColor - iter->vStartColor) * fRatio + iter->vStartColor;
		Vec2  vCurPos = (iter->vEndPos - iter->vStartPos) * fRatio + iter->vStartPos;

		// (World) -> (WndPos)
		vCurPos -= vCamLT;
		vCurPos.y *= -1.f;

		lstrcpyW(strOutput, iter->strOutput.c_str());
		CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strOutput, vCurPos.x, vCurPos.y, 
			fCurScale, FONT_RGBA(vCurColor.x, vCurColor.y, vCurColor.z, vCurColor.w));
		++iter;
	}


	// ItemCount ó���� ����Ѵ�.
	for (UINT i = 0; i < (UINT)ITEM::NUM_END; ++i)
	{
		if (!m_arrItemFont[i].bEnable)
			continue;

		// Scale, Color, Pos �� ����.

		m_arrItemFont[i].tInfo.fTime += fTime;
		if (m_arrItemFont[i].tInfo.fTime >= m_arrItemFont[i].tInfo.fPlayTime)
		{
			m_arrItemFont[i].iCount = 0;
			m_arrItemFont[i].bEnable = false;
			--m_iItemFont;

			for (UINT j = 0; j < (UINT)ITEM::NUM_END; ++j)
			{
				if (!m_arrItemFont[j].bEnable)
					continue;

				if (m_arrItemFont[i].iFontIdx < m_arrItemFont[j].iFontIdx)
				{
					m_arrItemFont[j].tInfo.vStartPos.y -= 25.f;
					--m_arrItemFont[j].iFontIdx;
				}
		
			}

			continue;
		}

		Vec2  vCurPos = m_arrItemFont[i].tInfo.vStartPos;

		// (World) -> (WndPos)
		vCurPos -= vCamLT;
		vCurPos.y *= -1.f;

		lstrcpyW(strOutput, m_arrItemFont[i].tInfo.strOutput.c_str());
		CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strOutput, vCurPos.x, vCurPos.y,
			22.f, UINT_MAX);
	}
}


