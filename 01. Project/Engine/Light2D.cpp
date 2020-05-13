#include "stdafx.h"
#include "Light2D.h"
#include "Transform.h"
#include "RenderMgr.h"

CLight2D::CLight2D() :
	CComponent(COMPONENT_TYPE::LIGHT2D),
	m_tLight2DInfo{}
{
	m_tLight2DInfo.iLightType = (int)LIGHT2D_TYPE::END;
}


CLight2D::~CLight2D()
{
}

void CLight2D::FinalUpdate()
{

	m_tLight2DInfo.tLightPos = GetTransform()->GetWorldPos();

	// RenderMgr¿¡ µî·Ï
	CRenderMgr::GetInst()->RegisterLight2D(m_tLight2DInfo);
	
}

bool CLight2D::SaveToScene(FILE * pFile)
{
	UINT iType = (UINT)GetComponentType();
	fwrite(&iType, 4, 1, pFile);

	fwrite(&m_tLight2DInfo, sizeof(Light2DInfo), 1, pFile);

	return true;
}

bool CLight2D::LoadFromScene(FILE * pFile)
{
	fread(&m_tLight2DInfo, sizeof(Light2DInfo), 1, pFile);

	return true;
}

void CLight2D::UpdateData()
{
}
