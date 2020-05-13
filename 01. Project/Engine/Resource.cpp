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
	// ���ҽ��� ���� �������� �ʴ´�.
	// �츮�� ���ҽ��� Key�� Path�� �����Ͽ� �ʿ��ϸ� �ҷ��ͼ� Load�ϴ� �����̴�.

	SaveWString(pFile, GetName());

	SaveWString(pFile, GetPath());
}
