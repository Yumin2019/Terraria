#pragma once
#include <Engine/Script.h>


class CMouseUIScript :
	public CScript
{
public:
	CMouseUIScript();
	virtual ~CMouseUIScript();
	CLONE(CMouseUIScript);

private:
	wstring		m_strCollision;

public:
	void SetCollisionStr(const wstring& strCollisionStr) { m_strCollision = strCollisionStr; }
	const wstring& GetCollisionStr() const { return m_strCollision; }

public:
	virtual void Update() {}


};

