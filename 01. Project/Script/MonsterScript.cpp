#include "stdafx.h"
#include "MonsterScript.h"
#include "FlyawayScript.h"

#include "PlayerScript.h"
#include "MouseScript.h"
#include "ParticleScript.h"
#include "ItemObjScript.h"
#include "PoolMgr.h"
#include "UtilMgr.h"

#include "MonsterHPScript.h"


#include <Engine/SceneMgr.h>
#include <Engine/Scene.h>
#include <Engine/Layer.h>
#include <Engine/TileMap2D.h>

int CMonsterScript::g_iMonster;

CMonsterScript::CMonsterScript(UINT iScriptType) :
	CDirectionScript(iScriptType),
	m_fSpeed(100.f),
	m_fSecond(0.f),
	m_fFindDist(400.f),
	m_fLimitTime(1.f),
	m_pFly(nullptr),
	m_pPlayer(nullptr),
	m_pMonsterUI(nullptr),
	m_bStop(false),
	m_bFind(false),
	m_bSlime(false)
{
}

CMonsterScript::CMonsterScript() :
	CDirectionScript((UINT)SCRIPT_TYPE::ZOMBIESCRIPT),
	m_fSpeed(100.f),
	m_fSecond(0.f),
	m_fFindDist(0.f),
	m_fLimitTime(1.f),
	m_pGravity(nullptr),
	m_pFly(nullptr),
	m_pPlayer(nullptr),
	m_bStop(false),
	m_bFind(false),
	m_bSlime(false)
{
}


CMonsterScript::~CMonsterScript()
{
}

int CMonsterScript::GetAttack() const
{
	return rand() % (m_iAttackMax - m_iAttackMin + 1) + m_iAttackMin;
}

void CMonsterScript::Attacked(int iDamage)
{
	// ���Ͱ� �±� ������ �������� ������ FlyAway�� �����ش�.
	GetFly()->Clear();

	m_iHP -= iDamage;
	Vec3 vCenter = GetTransform()->GetLocalPos();
	
	Vec2 vStart = Vec2(vCenter);

	vStart.x += 5.f * (rand() % 3 - 2);
	vStart.y += 10.f * (rand() % 3 + 2);

	Vec2 vEnd = vStart + Vec2(15.f * (rand() % 7 - 4), 70.f);

	Vec4 vEndColor;
	Vec4 vStartColor;

	switch (rand() % 6)
	{
	case 0:
		vEndColor.x = 255.f; 
		vEndColor.y = 255.f;
		vEndColor.z = 255.f;
		break;

	case 1: // ��
		vEndColor.x = 255.f;
		vEndColor.y = 0.f;
		vEndColor.z = 0.f;

		
		break;

	case 2:
		// ��
		vEndColor.x = 255.f;
		vEndColor.y = 127.f;
		vEndColor.z = 39.f;
		

		break;
	case 3:

		// ��
		vEndColor.x = 255.f;
		vEndColor.y = 255.f;
		vEndColor.z = 0.f;
	
		break;


	case 4: // ��ũ
		vEndColor.x = 255.f;
		vEndColor.y = 0.f;
		vEndColor.z = 128.f;
		break;

	case 5:
		vEndColor.x = 0.f;
		vEndColor.y = 0.f;
		vEndColor.z = 255.f;
		break;


	}

	vStartColor = vEndColor;
	vStartColor.w = 255.f;
	vEndColor.w = 50.f;


	float fPlayTime = (rand() % 11) * 0.1f + 0.7f; 
	float fStartScale = 25.f + (float)(rand() % 6); // 25 ~ 30

	// ���� �������� ���������.
	CUtilMgr::GetInst()->AddFont(to_wstring(iDamage), fPlayTime, vStart, vEnd,
		fStartScale, fStartScale + 3.f, vStartColor, vEndColor);

	// ���� �������� HP�� �����Ѵ�.
	if (m_pMonsterUI == nullptr)
		assert(false);

	m_pMonsterUI->SetData(GetHP(), GetHPMax());

	if (m_iHP <= 0)
	{
		// Particle�� �������ش�.
		vector<CParticleScript*> vecParticle;
		CPoolMgr::GetInst()->OrderParticle(5, vecParticle);

		Vec3 tCenterPos = Vec3(vCenter.x, vCenter.y, 60.f);

		float fMinRad = 0.f;
		float fMaxRad = XM_2PI; 

		for (size_t i = 0; i < vecParticle.size(); ++i)
		{
			vecParticle[i]->Init(tCenterPos, 10.f, ITEM::END, fMinRad, fMaxRad, 2.2f, 3.f, 0.f);
		}

		if (m_bSlime)
		{
			// �������̶�� Gel�� �����Ѵ�.
			CItemObjScript* pItem = CPoolMgr::GetInst()->OrderObj(LIST_ITEM::GEL);

			pItem->GetTransform()->SetLocalPos(tCenterPos);
			pItem->Start();
			pItem->SetCount(rand() % 3 + 1); // 1 ~ 3�� Drop

			CResMgr::GetInst()->Play(L"Slime_Killed", CHANNEL_TYPE::EFFECT_0, EFFECT_VOLUME);

		}
		else
		{
			CResMgr::GetInst()->Play(L"Zombie_Killed", CHANNEL_TYPE::EFFECT_0, EFFECT_VOLUME);

		}

		// Monster�� ����ϴ� UI�� ��ȯ
		CPoolMgr::GetInst()->ReturnMonsterUI(this);

		--CMonsterScript::g_iMonster;
		DeleteObject(GetObj());
		GetObj()->SetEnable(false);
	}

		if (m_bSlime)
			CResMgr::GetInst()->Play(L"Slime_Hit", CHANNEL_TYPE::EFFECT_0, EFFECT_VOLUME); 
		else
			CResMgr::GetInst()->Play(L"Zombie_Hit", CHANNEL_TYPE::EFFECT_0, EFFECT_VOLUME);
}

float CMonsterScript::GetPlayerDist() const
{
	float fDist = Vec3::Distance(m_pPlayer->GetTransform()->GetWorldPos(), GetTransform()->GetWorldPos());

	return fDist;
}

float CMonsterScript::LookAtPlayer()
{
	float fX = m_pPlayer->GetTransform()->GetWorldPos().x - GetTransform()->GetWorldPos().x;

	// �Ÿ��� 10.f ���϶�� ������.
	if (fabsf(fX) <= 10.f)
		return fX;

	if (fX > 0.f)
	{
		// �����ʿ� �ִ�.
		SetDir(1);
	}
	else
	{
		// ���ʿ� �ִ�.
		SetDir(-1);
	}

	return fX;
}

void CMonsterScript::Awake()
{
	const vector<CScript*>& vecScript = GetObj()->GetScriptVec();

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		switch ((SCRIPT_TYPE)vecScript[i]->GetScriptType())
		{

		case SCRIPT_TYPE::GRAVITYSCRIPT:
			m_pGravity = (CGravityScript*)vecScript[i];
			break;


		case SCRIPT_TYPE::FLYAWAYSCRIPT:
			m_pFly = (CFlyawayScript*)vecScript[i];

			break;
		 }
		
		
	}

	if (m_pFly == nullptr || m_pGravity == nullptr)
		assert(false);


	CScene* pScene = CSceneMgr::GetInst()->GetScene();
	CLayer* pLayer = pScene->FindLayer(L"Default");
	const vector<CGameObject*>& vecObj = pLayer->GetParentObjVec();

	for (size_t i = 0; i < vecObj.size(); ++i)
	{
		if (vecObj[i]->GetName() == L"Player")
		{
			m_pPlayer = (CPlayerScript*)vecObj[i]->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT);
			break;
		}
	}


	if (m_pPlayer == nullptr)
		assert(false);


	float fLight = 1.f;
	GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fLight);
}

void CMonsterScript::Update()
{
	if (!m_bFind && GetPlayerDist() <= m_fFindDist)
	{
		m_bFind = true;
	}
}

bool CMonsterScript::SaveToScene(FILE * pFile)
{
	fwrite(&m_fSpeed, 4, 1, pFile);
	fwrite(&m_fSecond, 4, 1, pFile);
	fwrite(&m_fLimitTime, 4, 1, pFile);
	CDirectionScript::SaveToScene(pFile);

	return true;
}

bool CMonsterScript::LoadFromScene(FILE * pFile)
{
	fread(&m_fSpeed, 4, 1, pFile);
	fread(&m_fSecond, 4, 1, pFile);
	fread(&m_fLimitTime, 4, 1, pFile);
	CDirectionScript::LoadFromScene(pFile);

	return true;
}

void CMonsterScript::LateUpdate()
{
	CDirectionScript::LateUpdate();
}

void CMonsterScript::TileCollision()
{
	if (m_pMonsterUI)
	{
		m_pMonsterUI->LateUpdate();
		m_pMonsterUI->GetTransform()->FinalUpdate();
	}
}

