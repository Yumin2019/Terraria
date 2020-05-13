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
	// Font 출력을 위해서 나중 위치에 존재.

public:
	virtual void Update() {}
	virtual void Render();
};

