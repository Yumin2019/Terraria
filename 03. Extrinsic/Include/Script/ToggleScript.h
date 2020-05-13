#pragma once
#include <Engine/Script.h>

/*
TexMtrl을 사용하고 있어야 한다. 오브젝트와 충돌시.... 텍스처를 교체한다. 0 ~ 1
*/

class CToggleScript :
	public CScript
{
public:
	CToggleScript();
	virtual ~CToggleScript();
	CLONE(CToggleScript);

public:
	virtual void Update() {}
	virtual void OnCollisionEnter(CCollider2D* pOther);
	virtual void OnCollisionExit(CCollider2D* pOther);

};

