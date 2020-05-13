#pragma once
#include <Engine/Script.h>

class CLogoScript :
	public CScript
{
public:
	CLogoScript();
	virtual ~CLogoScript();
	CLONE(CLogoScript);
	
private:
	class CDayScript*	m_pDay;
	Vec3				m_vScale;
	float				m_fAngle;

private:
	void GameStart();

public:
	virtual void Awake();
	virtual void Update() {}
	virtual void LateUpdate();
	virtual void Render();
};

