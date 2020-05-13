#pragma once
#include <Engine/Script.h>

class CMonsterHPScript :
	public CScript
{
public:
	CMonsterHPScript();
	virtual ~CMonsterHPScript();
	CLONE(CMonsterHPScript);

private:
	class CMonsterScript*	m_pMonster;

public:
	CMonsterScript* GetMonster() const { return m_pMonster; }
	void SetMonster(CMonsterScript* pMonster) { m_pMonster = pMonster; }
	void SetData(int iHP, int iHPMax);

	virtual void Update() {}
	virtual void LateUpdate();
};

