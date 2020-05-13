#pragma once
#include <Engine/Script.h>

class CGravityScript :
	public CScript
{
public:
	CGravityScript();
	virtual ~CGravityScript();
	CLONE(CGravityScript);

private:
	// 일단, 땅과 붙었는지 판단하기 위한, 처리.
	bool	m_bFall;

	// 가중치로 사용하기 위한 중력 값.
	float		m_fSecond;

public:
	void SetFall(bool bFall) { m_bFall = bFall; }
	bool IsFall() const { return m_bFall; }
	float GetSecond() const { return m_fSecond; }


public:
	virtual void Awake();
	virtual void Update();
	virtual bool LoadFromScene(FILE* pFile);
	virtual bool SaveToScene(FILE* pFile);

};

