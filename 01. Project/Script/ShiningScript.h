#pragma once
#include <Engine/Script.h>

/*
	Shining Mtrl을 사용하는 오브젝트가 가지고 있는 Script이다.
	Mtrl의 값을 셋팅하여 색상을 지정하고, 그것의 값을 변경하여 일정시간 마다 반짝이는 효과를 준다.
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

