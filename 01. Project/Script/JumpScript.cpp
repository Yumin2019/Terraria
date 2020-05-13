#include "stdafx.h"
#include "JumpScript.h"
#include "GravityScript.h"

CJumpScript::CJumpScript() :
	CScript((UINT)SCRIPT_TYPE::JUMPSCRIPT),
	m_fPower(0.f),
	m_fSecond(0.f),
	m_fChange(0.f),
	m_pGravity(nullptr)
{
}


CJumpScript::~CJumpScript()
{
}

void CJumpScript::Update()
{
	if (IsJumping())
	{
		//bool	m_bJumping; // 점프하는 중인지 에 대한 여부.
		//float	m_fPower; // 사용하는 Power 값.
		//float	m_fSecond; /

		m_fSecond += DT;

		if (m_fSecond >= 0.1f)
		{
			m_fSecond -= 0.1f;
			m_fPower -= m_fChange;
		}

		Vec3 tPos = GetTransform()->GetLocalPos();
		tPos.y += m_fPower * DT;

		GetTransform()->SetLocalPos(tPos);

		// Jump 종료 조건 : 바닥에 붙어버림. || 벽과 부딪힘.
		if (!m_pGravity->IsFall() || GetTransform()->IsMoveY())
		{
			m_bJumping = false;
		}
	}
}

void CJumpScript::Awake()
{
	const vector<CScript*> vecScript = GetObj()->GetScriptVec();

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		if (vecScript[i]->GetScriptType() == (UINT)SCRIPT_TYPE::GRAVITYSCRIPT)
		{
			m_pGravity = (CGravityScript*)vecScript[i];
			break;
		}
	}

	if (m_pGravity == nullptr)
	{
		assert(false);
	}
}

