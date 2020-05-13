#pragma once
#include <Engine/Script.h>

class CHeartUIScript :
	public CScript
{
public:
	CHeartUIScript();
	virtual ~CHeartUIScript();
	CLONE(CHeartUIScript);

private:
	class CPlayerScript* m_pPlayer;

public:
	virtual void Awake();

	virtual void Update() {}
	virtual void Render();
};

