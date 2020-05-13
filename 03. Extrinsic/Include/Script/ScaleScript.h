#pragma once
#include <Engine/Script.h>

class CScaleScript :
	public CScript
{
public:
	CScaleScript();
	virtual ~CScaleScript();
	CLONE(CScaleScript);

private:
	Vec2	m_vScale; // 원본 스케일.
	Vec2	m_vStartScale; // 시작 Scale
	Vec2	m_tPileScale; // 중첩 Scale값.
	bool	m_bScale; // Scale 진행 여부.
	float	m_fTime;
	float	m_fSetScale; // 배수 값.
	float	m_fCoefficient; // Size, Time 계수 값.

public:
	void ScaleChange(float fSetScale, float fDuration = 1.f);
	


public:
	virtual void Awake();
	virtual void Update();
};

