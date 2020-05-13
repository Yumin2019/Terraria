#include "stdafx.h"
#include "HeartUIScript.h"
#include "PlayerScript.h"

#include <Engine/FontMgr.h>
#include <Engine/Device.h>

CHeartUIScript::CHeartUIScript() :
	CScript((UINT)SCRIPT_TYPE::HEARTUISCRIPT),
	m_pPlayer(nullptr)
{
}


CHeartUIScript::~CHeartUIScript()
{
}

void CHeartUIScript::Awake()
{
	vector<CGameObject*> vecFind;
	CSceneMgr::GetInst()->FindObjByTag(L"Player", vecFind);

	if (vecFind.empty())
		assert(false);

	m_pPlayer = (CPlayerScript*)vecFind[0]->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT);

	if (m_pPlayer == nullptr)
		assert(false);

	Resolution tRes = CDevice::GetInst()->GetResolution();
	Vec3 vScale = GetTransform()->GetLocalScale();
	GetTransform()->SetLocalPos(tRes.fWidth / 2.f - vScale.x / 2.f - 92.f,
		tRes.fHeight / 2.f - 20.f - vScale.y / 2.f - 12.f);
}

void CHeartUIScript::Render()
{


	static wchar_t strOutput[20] = {};
	float fWidth = CDevice::GetInst()->GetResolution().fWidth;

	// FontMgr를 이용해서 플레이어의 현재 체력을 출력한다.
	if (m_pPlayer != nullptr)
	{
		int iHP = m_pPlayer->GetHP();
		int iHPMax = m_pPlayer->GetHPMax();

		GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &iHP);

		wsprintf(strOutput, L"Life %d/%d", iHP, iHPMax);
		CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strOutput,
			fWidth - 200.f, 10.f, 20.f, FONT_RGBA(255, 255, 255, 255)); 

	}
}
