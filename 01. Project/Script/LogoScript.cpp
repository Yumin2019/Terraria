#include "stdafx.h"
#include "LogoScript.h"
#include "DayScript.h"
#include <Engine/FontMgr.h>
#include <Engine/Core.h>
#include <Engine/Scene.h>
#include <Engine/Layer.h>

#include "PoolMgr.h"

CLogoScript::CLogoScript() :
	CScript((UINT)SCRIPT_TYPE::LOGOSCRIPT)
{
}


CLogoScript::~CLogoScript()
{
}


void CLogoScript::Awake()
{
	vector<CGameObject*> vecFind;
	CSceneMgr::GetInst()->FindObjByTag(L"TitleSun", vecFind);

	if (vecFind.empty())
		assert(false);

	m_pDay = (CDayScript*)vecFind[0]->FindScript((UINT)SCRIPT_TYPE::DAYSCRIPT);

	if (m_pDay == nullptr)
		assert(false);

	m_vScale = GetTransform()->GetLocalScale();
	m_fAngle = 15.f;

	m_fAngle = XMConvertToRadians(m_fAngle);
}

void CLogoScript::LateUpdate()
{

	int iIdx = 0;
	Vec3 vScale = m_vScale;
	float fAngle = m_fAngle;

	// 0�� Morning 1�� Night
	if (!m_pDay->IsMorning())
	{
		iIdx = 1;
		vScale *= (0.7f + 0.3f * g_tValue.g_fRatio);

		if (g_tValue.g_fRatio >= 0.5f)
			fAngle *= -(1.f - g_tValue.g_fRatio);
		else
			fAngle *= -g_tValue.g_fRatio;

	}
	else
	{
		vScale *= (0.7f + 0.3f * (1.f - g_tValue.g_fRatio));

		if (g_tValue.g_fRatio >= 0.5f)
			fAngle *=  (1.f - g_tValue.g_fRatio);
		else
			fAngle *= g_tValue.g_fRatio;


	}

	// Texture ���ÿ� ���� ó��.
	GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &iIdx);

	// �ڽ��� ũ�⸦ ���� DayObj�� ������ ���� �ش�.
	GetTransform()->SetLocalScale(vScale);
	
	// Angle�� �����Ѵ�. 
	GetTransform()->SetLocalRot(Vec3(0.f, 0.f, fAngle));


	
	
	if (KEYDOWN(KEY_TYPE::KEY_ENTER))
	{
		// ���� Scene�� ���� ó���� �Ѵ�.
		GameStart();
	}
}

void CLogoScript::GameStart()
{
	CScene* pScene = CSceneMgr::GetInst()->GetScene();

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pScene->GetLayer(i);
		const vector<CGameObject*>& vecObj = pLayer->GetLayerObjVec();

		for (int j = 0; j < vecObj.size(); ++j)
			vecObj[j]->SetEnable(true);
	}

	CLayer* pLayer = pScene->FindLayer(L"Title");
	const vector<CGameObject*>& vecObj = pLayer->GetLayerObjVec();

	for (int i = 0; i < vecObj.size(); ++i)
	{
		vecObj[i]->SetEnable(false);
		CScript::DeleteObject(vecObj[i]);
	}

	// PoolMgr�� �ʿ��� �������� ����ϸ� �ȴ�. (�̹� �ѹ� Awake, Start�Լ��� ȣ���ϱ⵵ �߰�)
	CPoolMgr::GetInst()->SetEnable(false);

	// DayObj���� Awake���� �б⸦ �ֱ����� flag
	CDayScript::g_eScene = SCENE_TYPE::INGAME;

	pScene->Awake();
	pScene->Start();

}


void CLogoScript::Render()
{
	static Resolution tRes = CCore::GetInst()->GetRes();

	static wchar_t strFont[50] = {};
	lstrcpy(strFont, L"Press Enter to Start");

	// Font���.
	CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strFont, tRes.fWidth / 2.f - 313.f, 
		tRes.fHeight / 2.f, 80.f, UINT_MAX);

	lstrcpy(strFont, L"Terraria Imitation");
	CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strFont, 10.f, tRes.fHeight - 30.f, 25.f, UINT_MAX);

	lstrcpy(strFont, L"Yumin 2020");
	CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strFont, tRes.fWidth - 124.f, tRes.fHeight - 30.f, 25.f, UINT_MAX);

}
