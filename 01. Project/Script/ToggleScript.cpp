#include "stdafx.h"
#include "ToggleScript.h"


CToggleScript::CToggleScript() :
	CScript((UINT)SCRIPT_TYPE::TOGGLESCRIPT)
{
}


CToggleScript::~CToggleScript()
{
}

void CToggleScript::OnCollisionEnter(CCollider2D* pOther)
{
	int idx = 1;
	GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &idx);
}

void CToggleScript::OnCollisionExit(CCollider2D* pOther)
{
	int idx = 0;
	GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &idx);
}
