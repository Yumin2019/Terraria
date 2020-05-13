#include "stdafx.h"
#include "ScaleScript.h"


CScaleScript::CScaleScript() :
	CScript((UINT)SCRIPT_TYPE::SCALESCRIPT),
	m_bScale(false),
	m_fTime(0.f),
	m_fSetScale(1.f),
	m_fCoefficient(1.f)
{
}


CScaleScript::~CScaleScript()
{
}

void CScaleScript::ScaleChange(float fSetScale, float fDuration)
{
	// ������ ���̶��, �ش� �۾��� ������ ���� ������ ó���� �Ѵ�.
	if (m_bScale)
	{
		Vec2 vScale = m_vStartScale * m_fSetScale;

		// �Ǽ� ���� ������ ���ؼ�.. �� ���̰� ������ �������� ����.
		if(fabsf(m_vScale.x - vScale.x) <= 2.f)
			GetTransform()->SetLocalScale(m_vScale);
		else
			GetTransform()->SetLocalScale(vScale);
	}

	m_vStartScale = Vec2(GetTransform()->GetLocalScale());  // ������ ���� Scale�� ����.
	m_tPileScale = Vec2(0.f, 0.f);
	m_bScale = true;
	m_fTime = 0.f;
	m_fCoefficient = 1.f / fDuration;
	m_fSetScale = fSetScale;

}

void CScaleScript::Awake()
{
	m_vScale = Vec2(GetTransform()->GetLocalScale());
}

void CScaleScript::Update()
{
	if (m_bScale)
	{
		float fChange = (m_fSetScale - 1.f) * DT * m_fCoefficient;
		// 2����, 1.
		// 0.5����, -0.5
		// �ð��� ���� �����.

		Vec2 tChangeScale = fChange * m_vStartScale;
		m_tPileScale += tChangeScale;
		GetTransform()->SetLocalScale(m_vStartScale + m_tPileScale);

		m_fTime += DT * m_fCoefficient;

		if (m_fTime >= 1.f)
		{
			m_bScale = false;

			Vec2 vScale = Vec2(GetTransform()->GetLocalScale());

			// �Ǽ� ���� ������ ���ؼ�.. �� ���̰� ������ �������� ����.
			if (fabsf(m_vScale.x - vScale.x) <= 2.f)
				GetTransform()->SetLocalScale(m_vScale);
			else
				GetTransform()->SetLocalScale(vScale);
		}
	}
}
