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
	// �ϴ�, ���� �پ����� �Ǵ��ϱ� ����, ó��.
	bool	m_bFall;

	// ����ġ�� ����ϱ� ���� �߷� ��.
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

