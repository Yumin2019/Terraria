#pragma once
#include <Engine/Script.h>

// 2�� ������ ������� ����.

class CJumpScript :
	public CScript
{
public:
	CJumpScript();
	virtual ~CJumpScript();
	CLONE(CJumpScript);

private:
	bool	m_bJumping; // �����ϴ� ������ �� ���� ����.
	float	m_fPower; // ����ϴ� Power ��.
	float	m_fChange; // Power ��ȭ��.
	float	m_fSecond; // �ð� ��.

	class CGravityScript* m_pGravity;

public:
	bool IsJumping() const { return m_bJumping; } // Jump ����.

	void Jump(float fPower, float fPowerChange) // Jump ����
	{
		if (m_bJumping /*|| m_fDelayTime < 0.2f*/)
			return;

		m_bJumping = true;
		m_fPower = fPower;
		m_fChange = fPowerChange;
		m_fSecond = 0.f;
	}

	void SetJumping(bool bJump) { m_bJumping = bJump; } // ���� ���� ����.

public:
	virtual void Update();
	virtual void Awake();

};

