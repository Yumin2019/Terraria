#include "stdafx.h"
#include "MonsterHPScript.h"
#include "MonsterScript.h"

CMonsterHPScript::CMonsterHPScript() :
	CScript((UINT)SCRIPT_TYPE::MONSTERHPSCRIPT),
	m_pMonster(nullptr)
{
}


CMonsterHPScript::~CMonsterHPScript()
{
}

void CMonsterHPScript::SetData(int iHP, int iHPMax)
{
	GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &iHP);
	GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_1, &iHPMax);
}

void CMonsterHPScript::LateUpdate()
{
	if (GetMonster() == nullptr)
		return;


	Vec3 vPos = GetMonster()->GetTransform()->GetLocalPos();
	GetTransform()->SetLocalPos(vPos.x, vPos.y - 30.f);
}

