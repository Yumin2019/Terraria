#pragma once
#include <Engine/Script.h>

class CParticleScript :
	public CScript
{
public:
	CParticleScript();
	virtual ~CParticleScript();
	CLONE(CParticleScript);

private:
	class CFlyawayScript*		m_pFly;
	bool m_bStart;
	float m_fSecond;
	float m_fLimitTime;

public:
	bool Init(const Vec3& tCenterPos, float fRadius, ITEM eItem, float fMinRadian, float fMaxRadian,
		float fLifeTime, float fSpeed, float fChangeSpeed);
	virtual void Awake();
	virtual void Update();
};

