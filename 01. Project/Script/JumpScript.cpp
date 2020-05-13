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
		//bool	m_bJumping; // �����ϴ� ������ �� ���� ����.
		//float	m_fPower; // ����ϴ� Power ��.
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

		// Jump ���� ���� : �ٴڿ� �پ����. || ���� �ε���.
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

