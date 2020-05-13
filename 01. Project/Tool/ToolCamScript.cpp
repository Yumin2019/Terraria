#include "stdafx.h"
#include "ToolCamScript.h"

#include <Engine/Camera.h>
 
CToolCamScript::CToolCamScript() :
	m_fSpeed(100.f),
	m_fScaleSpeed(1.f),
	CScript(0)
{
}


CToolCamScript::~CToolCamScript()
{
}

void CToolCamScript::Update()
{

	Vec3 tPos = GetTransform()->GetLocalPos();
	float fScale = GetCamera()->GetScale();
	float fSpeed = m_fSpeed;

	if (KEYPRESS(KEY_TYPE::KEY_LSHIFT))
	{
		fSpeed *= 5.f;
	}

	if (KEYPRESS(KEY_TYPE::KEY_W))
	{
		tPos.y += fSpeed * DT;
	}

	if (KEYPRESS(KEY_TYPE::KEY_S))
	{
		tPos.y -= fSpeed * DT;
	}


	if (KEYPRESS(KEY_TYPE::KEY_A))
	{
		tPos.x -= fSpeed * DT;
	}


	if (KEYPRESS(KEY_TYPE::KEY_D))
	{
		tPos.x += fSpeed * DT;
	}

	if (KEYPRESS(KEY_TYPE::KEY_1))
	{
		fScale -= m_fScaleSpeed * DT;

		if (fScale < 0.01f)
			fScale = 0.01f;
		GetCamera()->SetScale(fScale);
	}
	
	if (KEYPRESS(KEY_TYPE::KEY_2))
	{
		fScale += m_fScaleSpeed * DT;
		GetCamera()->SetScale(fScale);

	}

	GetTransform()->SetLocalPos(tPos);

}
