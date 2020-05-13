#pragma once
#include <Engine/Script.h>

class CArmorPanelScript :
	public CScript
{
public:
	CArmorPanelScript();
	virtual ~CArmorPanelScript();
	CLONE(CArmorPanelScript);

private:
	class CPlayerScript*	m_pPlayer;

public:
	virtual void Awake();
	virtual void Update() {}
	virtual void Render();
};

