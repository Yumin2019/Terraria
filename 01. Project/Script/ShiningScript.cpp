#include "stdafx.h"
#include "ShiningScript.h"


CShiningScript::CShiningScript() :
	CScript((UINT)SCRIPT_TYPE::SHININGSCRIPT),
	m_fSecond(0.f),
	m_fData(1.f)
{
}


CShiningScript::~CShiningScript()
{
}

void CShiningScript::Update()
{
	m_fSecond += DT;

	if (m_fSecond >= 1.f)
	{
		m_fSecond -= 1.f;

		if (m_fData == 1.f)
			m_fData = 0.5f;
		else
			m_fData = 1.f;

		// Shining Mtrl에 대한 처리를 한다.
		GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_3, &m_fData);
	}
}
