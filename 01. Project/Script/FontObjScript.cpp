#include "stdafx.h"
#include "FontObjScript.h"
#include "UtilMgr.h"


CFontObjScript::CFontObjScript() : 
	CScript((UINT)SCRIPT_TYPE::FONTOBJSCRIPT)
{
}


CFontObjScript::~CFontObjScript()
{
}

void CFontObjScript::Render()
{
	// Object가 대신 수행.
	// Tool에서는 Mgr를 통해서 호출이 가능하지만, Release에서는 
	CUtilMgr::GetInst()->Render();
}
