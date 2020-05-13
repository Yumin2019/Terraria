#pragma once

#include <Engine/Script.h>

class CToolCamScript :
	public CScript
{
public:
	CToolCamScript();
	virtual ~CToolCamScript();
	CLONE(CToolCamScript);

private:
	float	m_fSpeed;
	float	m_fScaleSpeed;

public:
	virtual void Update();
};

