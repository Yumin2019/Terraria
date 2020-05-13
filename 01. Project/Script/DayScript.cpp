#include "stdafx.h"
#include "DayScript.h"
#include <Engine/TimeMgr.h>
#include <Engine/Core.h>
#include <Engine/ResMgr.h>

#include "PrototypeMgr.h"
#include "PoolMgr.h"
#include "MainCameraScript.h"
#include "MonsterScript.h"

SCENE_TYPE CDayScript::g_eScene = SCENE_TYPE::TITLE;

CDayScript::CDayScript() :
	CScript((UINT)SCRIPT_TYPE::DAYSCRIPT),
	m_fStartAngle(180.f),
	m_fEndAngle(0.f),
	m_fSecond(0.f),
	m_fHalfTime(150.f), // day : 5 min
	m_bMorning(true),
	m_fMonsterTime(0.f),
	m_fMonsterLimitTime(5.f)
{
	m_fEndAngle = XMConvertToRadians(m_fEndAngle);
	m_fStartAngle = XMConvertToRadians(m_fStartAngle);
	m_fCurAngle = m_fStartAngle;

	m_fDist = (CCore::GetInst()->GetRes().fHeight - 20.f);
}


CDayScript::~CDayScript()
{
}

void CDayScript::Awake()
{
	m_fMonsterLimitTime = 20.f; // 20
	m_fMonsterTime = 0.f;

	m_vCenterPos = GetTransform()->GetLocalPos();

	int iTexture = 0;
	GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &iTexture);

	g_tValue.g_iMorning = 1;
	g_tValue.g_fRatio = 0.2f;
	CMonsterScript::g_iMonster = 0;
	CResMgr::GetInst()->Stop(CHANNEL_TYPE::BGM);
	CResMgr::GetInst()->Stop(CHANNEL_TYPE::EFFECT_0);
	CResMgr::GetInst()->Stop(CHANNEL_TYPE::EFFECT_1);


	if (g_eScene == SCENE_TYPE::TITLE)
	{
		m_fHalfTime = 10.f;
		CResMgr::GetInst()->Play(L"TitleBgm", CHANNEL_TYPE::BGM, 0.3f);
	}
	else
	{
		m_fHalfTime = 150.f;

		CResMgr::GetInst()->Play(L"MorningBgm", CHANNEL_TYPE::BGM, 0.07f);

	}



	m_fSecond = m_fHalfTime * 0.2f;

}

void CDayScript::Update()
{
	// 몬스터 생성
	if ((g_eScene == SCENE_TYPE::INGAME) && CMonsterScript::g_iMonster < 5) // Max
	{
		m_fMonsterTime += DT;

		if (m_fMonsterTime >= m_fMonsterLimitTime)
		{
			m_fMonsterTime -= m_fMonsterLimitTime;
			CGameObject* pMonster = nullptr;
			static Resolution tRes = CCore::GetInst()->GetRes();

			CMonsterScript* pMonsterScript = nullptr;

			if (m_bMorning) // Slime
			{
				pMonster = CPrototypeMgr::GetInst()->CreateCloneObj(L"RabbitSlime", L"Monster");
				pMonsterScript = (CMonsterScript*)pMonster->FindScript((UINT)SCRIPT_TYPE::SLIMESCRIPT);
			}
			else // Zombie
			{
				pMonster = CPrototypeMgr::GetInst()->CreateCloneObj(L"Zombie" + to_wstring(rand() % 7), L"Monster");
				pMonsterScript = (CMonsterScript*)pMonster->FindScript((UINT)SCRIPT_TYPE::ZOMBIESCRIPT);
			}

			// Monster에게 UI를 셋팅한다.
			CPoolMgr::GetInst()->OrderMonsterUI(pMonsterScript);

		

			Vec3 vPos = CMainCameraScript::g_vMainCamPos;

			if (rand() % 2)
			{
				// 오른쪽에 생성
				vPos.x += tRes.fWidth / 2.f + 40.f * (rand() % 10);

				// 넘어가는 경우 (보일 가능성을 생각해서)에는 반대 편에 생성해버린다.
				if (vPos.x >= CMainCameraScript::g_vMapEnd.x)
					vPos.x = CMainCameraScript::g_vMapStart.x + 100.f;
			}
			else
			{
				// 왼쪽에 생성
				vPos.x -= tRes.fWidth / 2.f + 40.f * (rand() % 10);

				if (vPos.x <= CMainCameraScript::g_vMapStart.x)
					vPos.x = CMainCameraScript::g_vMapEnd.x - 100.f;
			}

			vPos.z = 110.f;
			pMonster->GetTransform()->SetLocalPos(vPos);
			++CMonsterScript::g_iMonster;

			
		}
	}


	m_fSecond += DT;
	if (m_fSecond >= m_fHalfTime)
	{
		m_fSecond -= m_fHalfTime;
		m_bMorning = !m_bMorning;
		m_fCurAngle = m_fStartAngle;

		if (g_eScene != SCENE_TYPE::TITLE)
		{
			CResMgr::GetInst()->Stop(CHANNEL_TYPE::BGM);
		
			if (m_bMorning == false)
				CResMgr::GetInst()->Play(L"NightBgm", CHANNEL_TYPE::BGM, 0.07f);
			else
				CResMgr::GetInst()->Play(L"MorningBgm", CHANNEL_TYPE::BGM, 0.07f);

		}
		
		
		int iTexture = 0;

		if (m_bMorning == false)
		{
			iTexture = 1;
			g_tValue.g_iMorning = 0;
		}
		else
		{
			g_tValue.g_iMorning = 1;
		}

		// Texture Shader를 사용한다고 하자. 0번 Sun / 1번 Moon
		GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &iTexture);
	}

	float fRatio = m_fSecond / m_fHalfTime;
	m_fCurAngle = (m_fEndAngle - m_fStartAngle) * fRatio + m_fStartAngle;
	g_tValue.g_fRatio = fRatio;


	Vec3 vPos = m_vCenterPos;
	vPos.x += cosf(m_fCurAngle) * m_fDist;
	vPos.y += sinf(m_fCurAngle) * m_fDist;
	GetTransform()->SetLocalPos(vPos);
	GetTransform()->SetLocalRot(Vec3(0.f, 0.f, m_fCurAngle));

}
