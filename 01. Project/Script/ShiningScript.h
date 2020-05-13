#pragma once
#include <Engine/Script.h>

/*
	Shining Mtrl�� ����ϴ� ������Ʈ�� ������ �ִ� Script�̴�.
	Mtrl�� ���� �����Ͽ� ������ �����ϰ�, �װ��� ���� �����Ͽ� �����ð� ���� ��¦�̴� ȿ���� �ش�.
*/

class CShiningScript :
	public CScript
{
public:
	CShiningScript();
	virtual ~CShiningScript();
	CLONE(CShiningScript);

private:
	float	m_fSecond;
	float	m_fData;

public:
	virtual void Update();

};

