#include "stdafx.h"
#include "GravityScript.h"


CGravityScript::CGravityScript() :
	CScript((UINT)SCRIPT_TYPE::GRAVITYSCRIPT),
	m_bFall(true),
	m_fSecond(0.f)
{
}


CGravityScript::~CGravityScript()
{
}

void CGravityScript::Awake()
{
	m_bFall = false;
	m_fSecond = 0.f;
}

void CGravityScript::Update()
{

	if (m_bFall)
	{
		m_fSecond += DT;

		Vec3 tPos = GetTransform()->GetLocalPos();
		float fSpeed = 200.f;

		tPos.y -= DT * fSpeed;

		GetTransform()->SetLocalPos(tPos);
	}



	// 땅과 붙었는지 판단한다.

	if (m_bFall && GetTransform()->IsMoveY() && GetTransform()->GetDirY() == -1.f)
	{
		m_bFall = false;
		m_fSecond = 0.f;
	}

	// 아무 타일하고도 충돌이 아니라면, (공중에 떠있음) true.
	else if (GetTransform()->IsTileColl() == false)
	{
		m_bFall = true;
		GetTransform()->SetMoveY(false, 0.f);
		GetTransform()->SetMoveX(false, 0.f);
	}

}

bool CGravityScript::LoadFromScene(FILE * pFile)
{
	fread(&m_bFall, sizeof(bool), 1, pFile);
	fread(&m_fSecond, 4, 1, pFile);

	return true;
}

bool CGravityScript::SaveToScene(FILE * pFile)
{
	fwrite(&m_bFall, sizeof(bool), 1, pFile);
	fwrite(&m_fSecond, 4, 1, pFile);

	return true;
}

