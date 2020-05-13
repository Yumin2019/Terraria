#include "stdafx.h"
#include "DirectionScript.h"


CDirectionScript::CDirectionScript(UINT iScriptType) :
	CScript(iScriptType),
	m_iDir(1)
{
	
}

CDirectionScript::CDirectionScript() :
	CScript(0),
	m_iDir(1)
{
}


CDirectionScript::~CDirectionScript()
{
}


void CDirectionScript::LateUpdate()
{
	if (GetMeshRender())
	{
		int iData = 0;

		if (m_iDir == 1)
			iData = 1;

		GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &iData);

	}
}

bool CDirectionScript::SaveToScene(FILE * pFile)
{
	fwrite(&m_iDir, 4, 1, pFile);

	return true;
}

bool CDirectionScript::LoadFromScene(FILE * pFile)
{
	fread(&m_iDir, 4, 1, pFile);

	return true;
}
