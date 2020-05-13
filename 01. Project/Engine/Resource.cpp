#include "stdafx.h"
#include "Resource.h"


CResource::CResource(RES_TYPE eType) :
	m_iRef(0), 
	m_eType(eType),
	m_iCloneCount(0)
{
}

CResource::CResource(const CResource & res) :
	CEntity(res),
	m_strPath(res.m_strPath),
	m_iRef(0),
	m_eType(res.m_eType),
	m_iCloneCount(0)
{
}


CResource::~CResource()
{
}

void CResource::SaveToScene(FILE * pFile)
{
	// 리소스를 직접 저장하지 않는다.
	// 우리는 리소스의 Key와 Path를 저장하여 필요하면 불러와서 Load하는 개념이다.

	SaveWString(pFile, GetName());

	SaveWString(pFile, GetPath());
}
