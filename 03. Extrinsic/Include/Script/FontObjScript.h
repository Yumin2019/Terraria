#pragma once
#include <Engine/Script.h>

class CFontObjScript :
	public CScript
{
public:
	CFontObjScript();
	virtual ~CFontObjScript();
	CLONE_DISABLE_SCRIPT(CFontObjScript);

	// Only One Object
	// Font ����� ���ؼ� ���� ��ġ�� ����.

public:
	virtual void Update() {}
	virtual void Render();
};

