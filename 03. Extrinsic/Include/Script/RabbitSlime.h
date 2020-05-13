#pragma once
#include "MonsterScript.h"

class CRabbitSlime :
	public CMonsterScript
{
public:
	CRabbitSlime();
	virtual ~CRabbitSlime();

	CLONE(CRabbitSlime);

public:
	virtual bool SaveToScene(FILE* pFile);
	virtual bool LoadFromScene(FILE* pFile);

	virtual void Update();
	virtual void LateUpdate();

	virtual void OnCollisionEnter(CCollider2D* pOther);
	virtual void OnCollision(CCollider2D* pOther) {}
	virtual void OnCollisionExit(CCollider2D* pOther) {}
};

