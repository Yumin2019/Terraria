#pragma once
#include <Engine/Script.h>

// 2단 점프는 고려하지 않음.

class CJumpScript :
	public CScript
{
public:
	CJumpScript();
	virtual ~CJumpScript();
	CLONE(CJumpScript);

private:
	bool	m_bJumping; // 점프하는 중인지 에 대한 여부.
	float	m_fPower; // 사용하는 Power 값.
	float	m_fChange; // Power 변화값.
	float	m_fSecond; // 시간 값.

	class CGravityScript* m_pGravity;

public:
	bool IsJumping() const { return m_bJumping; } // Jump 여부.

	void Jump(float fPower, float fPowerChange) // Jump 시작
	{
		if (m_bJumping /*|| m_fDelayTime < 0.2f*/)
			return;

		m_bJumping = true;
		m_fPower = fPower;
		m_fChange = fPowerChange;
		m_fSecond = 0.f;
	}

	void SetJumping(bool bJump) { m_bJumping = bJump; } // 강제 종료 가능.

public:
	virtual void Update();
	virtual void Awake();

};

