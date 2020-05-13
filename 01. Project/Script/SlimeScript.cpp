#include "stdafx.h"
#include "SlimeScript.h"
#include "PlayerScript.h"
#include "FlyawayScript.h"


CSlimeScript::CSlimeScript() :
	CMonsterScript((UINT)SCRIPT_TYPE::SLIMESCRIPT)
{
}


CSlimeScript::~CSlimeScript()
{
}

bool CSlimeScript::SaveToScene(FILE * pFile)
{
	CMonsterScript::SaveToScene(pFile);

	return true;
}

bool CSlimeScript::LoadFromScene(FILE * pFile)
{
	CMonsterScript::LoadFromScene(pFile);

	return true;
}

void CSlimeScript::Awake()
{
	CMonsterScript::Awake();

	m_bSlime = true;

	int iMinHP = 14;
	int iMaxHP = 25;

	m_iHP = rand () % (iMaxHP - iMinHP + 1) + iMinHP; // ü���� �������� ����.
	m_iHPMax = m_iHP;
	m_iAttackMin = 9;
	m_iAttackMax = 14;
}

void CSlimeScript::Update()
{
	CMonsterScript::Update();

	if (GetFly()->IsFlyaway())
		return;

	else if (GetFind() == false)
		return;

	m_fSecond += DT;

	if (m_fSecond >= m_fLimitTime)
	{
		m_fSecond -= m_fLimitTime;
		static Vec3 vJumpVec = Vec3(1.f, 1.f, 0.f);
		static Vec3 vSpeedVec = Vec3(300.f, 350.f, 0.f);
		static Vec3 vChangeVec = Vec3(20.f, 40.f, 0.f);

		float fDist = fabsf(LookAtPlayer());
		vJumpVec.x = GetDir();

		if (fDist > 250.f)
			vSpeedVec.x = 350.f;
		else
			vSpeedVec.x = fDist + 100.f;

		GetFly()->AddFly(vJumpVec, vSpeedVec, vChangeVec, 1.f);
	}

}

void CSlimeScript::LateUpdate()
{
	CMonsterScript::LateUpdate();
}

void CSlimeScript::OnCollisionEnter(CCollider2D * pOther)
{

	wstring strOtherObj = pOther->GetObj()->GetName();

	if (strOtherObj == L"Mouse" || strOtherObj == L"SelectedTile")
		return;


	// Tile�� ������ ����. ��, Item Or Player

	// ��� �ʿ��� �¾Ҵ��� Ȯ������.
	Vec3 tDir = Vec3(-1.f, 1.f, 0.f);
	Vec3 tSpeed = Vec3(150.f, 300.f, 0.f);
	Vec3 tPower = Vec3(10.f, 150.f, 0.f);
	float fTime = 0.3f;

	bool bPlayer = (pOther->GetObj() == GetPlayer()->GetObj());

	if (pOther->GetTransform()->GetLocalPos().x > GetTransform()->GetLocalPos().x)
	{
		// ��밡 �����ʿ� �ִ�.
		if (bPlayer)
		{
			if (!GetPlayer()->IsAlpha())
			{
				// �÷��̾ ���������� ���󰣴�.
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
		// ��밡 ���ʿ� �ִ�.

		if (bPlayer)
		{
			if (!GetPlayer()->IsAlpha())
			{
				// �÷��̾ �������� ���󰣴�.
				tSpeed.x = 300.f;
				GetPlayer()->Flyaway(tDir, tSpeed, tPower, fTime);
				GetPlayer()->Attacked(GetAttack());
			}
			

		}
		else
		{
			Attacked(GetPlayer()->Attack());

			tDir = Vec3(1.f, 1.f, 0.f);
			// �������� ���ʿ� �ִ� : ���������� ���ư�.
			GetFly()->AddFly(tDir, tSpeed, tPower, fTime);

		}

	}
}
