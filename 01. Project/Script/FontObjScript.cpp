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
	// Object�� ��� ����.
	// Tool������ Mgr�� ���ؼ� ȣ���� ����������, Release������ 
	CUtilMgr::GetInst()->Render();
}
