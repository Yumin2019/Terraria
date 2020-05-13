#include "stdafx.h"
#include "ArmorPanelScript.h"
#include "PlayerScript.h"


CArmorPanelScript::CArmorPanelScript() :
	CScript((UINT)SCRIPT_TYPE::ARMORPANELSCRIPT),
	m_pPlayer(nullptr)
{
}


CArmorPanelScript::~CArmorPanelScript()
{
}

void CArmorPanelScript::Awake()
{
	vector<CGameObject*> vecFind;
	CSceneMgr::GetInst()->FindObjByTag(L"Player", vecFind);

	if (vecFind.empty())
		assert(false);

	m_pPlayer = (CPlayerScript*)vecFind[0]->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT);

	if (m_pPlayer == nullptr)
		assert(false);
}

void CArmorPanelScript::Render()
{
	static wchar_t strLength[10] = {};

	// 열린 상태라면, 방어구 수치를 출력하는 것으로 한다.

	if (m_pPlayer != nullptr)
	{
		wsprintf(strLength, L"%d", m_pPlayer->GetArmor());
		CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, 539.f, 238.f, 25.f, FONT_RGBA(255, 255, 255, 255));
	}
}
