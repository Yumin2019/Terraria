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
	Vec2	m_vScale; // ���� ������.
	Vec2	m_vStartScale; // ���� Scale
	Vec2	m_tPileScale; // ��ø Scale��.
	bool	m_bScale; // Scale ���� ����.
	float	m_fTime;
	float	m_fSetScale; // ��� ��.
	float	m_fCoefficient; // Size, Time ��� ��.

public:
	void ScaleChange(float fSetScale, float fDuration = 1.f);
	


public:
	virtual void Awake();
	virtual void Update();
};

