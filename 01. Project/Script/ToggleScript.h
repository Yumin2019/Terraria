#pragma once
#include <Engine/Script.h>

/*
TexMtrl�� ����ϰ� �־�� �Ѵ�. ������Ʈ�� �浹��.... �ؽ�ó�� ��ü�Ѵ�. 0 ~ 1
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

