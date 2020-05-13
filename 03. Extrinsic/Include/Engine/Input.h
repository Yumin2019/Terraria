#pragma once
#include "Component.h"
class CInput :
	public CComponent
{
public:
	CInput();
	virtual ~CInput();
	CLONE(CInput);

public:
	virtual void Update();
};

