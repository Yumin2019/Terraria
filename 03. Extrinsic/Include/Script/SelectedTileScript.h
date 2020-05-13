#pragma once
#include <Engine/Script.h>

class CSelectedTileScript :
	public CScript
{
public:
	CSelectedTileScript();
	virtual ~CSelectedTileScript();
	CLONE(CSelectedTileScript);

public:
	virtual void Update() {}
};

