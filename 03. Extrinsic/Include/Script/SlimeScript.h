#pragma once
#include "MonsterScript.h"
class CSlimeScript :
	public CMonsterScript
{
public:
	CSlimeScript();
	virtual ~CSlimeScript();

	CLONE(CSlimeScript);

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

