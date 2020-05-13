#pragma once
#include "MonsterScript.h"

class CFlyawayScript;

class CZombieScript :
	public CMonsterScript
{
public:
	CZombieScript();
	virtual ~CZombieScript();

	CLONE(CZombieScript);

private:
	int			m_iCryIdx;
	float		m_fCryTime;
	float		m_fCryLimitTime;


public:
	virtual bool SaveToScene(FILE* pFile);
	virtual bool LoadFromScene(FILE* pFile);

	virtual void Awake();
	virtual void Update();
	virtual void LateUpdate();

	virtual void OnCollisionEnter(CCollider2D* pOther);
	virtual void OnCollision(CCollider2D* pOther) {}
	virtual void OnCollisionExit(CCollider2D* pOther) {}

};

