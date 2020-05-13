#include "stdafx.h"
#include "ZombieScript.h"
#include "FlyawayScript.h"

#include "PlayerScript.h"
#include "JumpScript.h"
#include "GravityScript.h"

#include <Engine/SceneMgr.h>
#include <Engine/Scene.h>
#include <Engine/Layer.h>

CZombieScript::CZombieScript() :
	CMonsterScript((UINT)SCRIPT_TYPE::ZOMBIESCRIPT),
	m_fCryTime(5.f),
	m_fCryLimitTime(7.f)
{
}


CZombieScript::~CZombieScript()
{
}


bool CZombieScript::SaveToScene(FILE * pFile)
{
	CMonsterScript::SaveToScene(pFile);

	return true;
}

bool CZombieScript::LoadFromScene(FILE * pFile)
{
	CMonsterScript::LoadFromScene(pFile);

	

	return true;
}

void CZombieScript::Awake()
{
	CMonsterScript::Awake();

	m_bSlime = false;

	int iMinHP = 30;
	int iMaxHP = 55;

	m_iHP = rand() % (iMaxHP - iMinHP + 1) + iMinHP; // 체력을 랜덤으로 주자.
	m_iHPMax = m_iHP;
	m_iAttackMin = 12;
	m_iAttackMax = 16;

	m_iCryIdx = rand() % 5;

}

void CZombieScript::Update()
{
	CMonsterScript::Update();

	if (GetFly()->IsFlyaway())
		return;

	Vec3 tLocalPos = GetTransform()->GetLocalPos();
	float fTime = DT;

	if (GetFind() == false)
	{
		m_fSecond += fTime;

		if (m_fSecond >= m_fLimitTime)
		{
			DirChange();
			m_fSecond -= m_fLimitTime;
		}

		if(!GetTransform()->IsMoveX())
			tLocalPos.x += GetDir() * m_fSpeed * fTime;
	}
	else
	{
		LookAtPlayer();

		if (!GetTransform()->IsMoveX())
		{
			tLocalPos.x += GetDir() * m_fSpeed * fTime;
			m_fSecond += fTime;

			if (m_fSecond >= m_fLimitTime)
			{
				m_fSecond -= m_fLimitTime;

				static Vec3 vJumpVec = Vec3(1.f, 1.f, 0.f);
				static Vec3 vSpeedVec = Vec3(m_fSpeed, 370.f, 0.f);
				static Vec3 vChangeVec = Vec3(0.f, 30.f, 0.f);

				vJumpVec.x = GetDir();

				GetFly()->AddFly(vJumpVec, vSpeedVec, vChangeVec, 1.f);
			}
		}

		// 울음 소리처리.
		m_fCryTime += fTime;

		if (m_fCryTime >= m_fCryLimitTime)
		{
			m_fCryTime -= m_fCryLimitTime;
			CResMgr::GetInst()->Play(L"Zombie_" + to_wstring(m_iCryIdx), CHANNEL_TYPE::EFFECT_0, EFFECT_VOLUME);
		}
	}

	GetTransform()->SetLocalPos(tLocalPos);

}

void CZombieScript::LateUpdate()
{
	CMonsterScript::LateUpdate();

}


void CZombieScript::OnCollisionEnter(CCollider2D * pOther)
{
	wstring strOtherObj = pOther->GetObj()->GetName();

	if (strOtherObj == L"Mouse" || strOtherObj == L"SelectedTile")
		return;


	// Tile은 들어오지 않음. 즉, Item Or Player

	// 어느 쪽에서 맞았는지 확인하자.
	Vec3 tDir = Vec3(-1.f, 1.f, 0.f);
	Vec3 tSpeed = Vec3(150.f, 300.f, 0.f);
	Vec3 tPower = Vec3(10.f, 150.f, 0.f);
	float fTime = 0.3f;

	bool bPlayer = (pOther->GetObj() == GetPlayer()->GetObj());

	if (pOther->GetTransform()->GetLocalPos().x > GetTransform()->GetLocalPos().x)
	{
		// 상대가 오른쪽에 있다.
		if (bPlayer)
		{
			if (!GetPlayer()->IsAlpha())
			{
				// 플레이어가 오른쪽으로 날라간다.
				tDir = Vec3(1.f, 1.f, 0.f);
				tSpeed.x = 300.f;
				GetPlayer()->Flyaway(tDir, tSpeed, tPower, fTime);
				GetPlayer()->Attacked(GetAttack());
			}
			

		}
		else
		{
			Attacked(GetPlayer()->Attack());

			// to left
			GetFly()->AddFly(tDir, tSpeed, tPower, fTime);
		}
		
	}
	else
	{
		// 상대가 왼쪽에 있다.

		if (bPlayer)
		{
			if (!GetPlayer()->IsAlpha())
			{
				// 플레이어가 왼쪽으로 날라간다.
				tSpeed.x = 300.f;
				GetPlayer()->Flyaway(tDir, tSpeed, tPower, fTime);
				GetPlayer()->Attacked(GetAttack());
			}
		}
		else
		{
			Attacked(GetPlayer()->Attack());

			tDir = Vec3(1.f, 1.f, 0.f);
			// 아이템이 왼쪽에 있다 : 오른쪽으로 날아가.
			GetFly()->AddFly(tDir, tSpeed, tPower, fTime);

		}
		
	}

	// 맞고 생각할 시간이 필요해.
	/*m_bStop = true;*/

}
