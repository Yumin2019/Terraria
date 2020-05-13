#pragma once
#include <Engine/Script.h>


class CDayScript :
	public CScript
{
public:
	CDayScript();
	virtual ~CDayScript();
	CLONE(CDayScript);

public:
	static SCENE_TYPE g_eScene;

private:
	Vec3			m_vCenterPos; // �߽��� �� ��ġ.
	float			m_fDist; // �߽� ��ġ���� ������ �Ÿ�
	float			m_fStartAngle;
	float			m_fEndAngle;
	float			m_fCurAngle; 

	float			m_fSecond;
	float			m_fHalfTime; // ��, �� �ð�.
	bool			m_bMorning;

	// Monster Creation
	float			m_fMonsterTime;
	float			m_fMonsterLimitTime;

public:
	bool IsMorning() const { return m_bMorning; }

public:
	virtual void Awake();
	virtual void Update();
};

