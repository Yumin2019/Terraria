#include "stdafx.h"
#include "Camera.h"
#include "Device.h"

#include "KeyMgr.h"
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "RenderMgr.h"

#include "Transform.h"
#include "Scene.h"
#include "Layer.h"


CCamera::CCamera() :
	CComponent(COMPONENT_TYPE::CAMERA),
	m_fNear(1.f),
	m_fFar(1000.f),
	m_fFOV(XM_PI / 4.f),
	m_fScale(1.f),
	m_eType(PROJ_TYPE::ORTHGRAPHIC),
	m_eVertical(MOVE_DIR_VERTICAL::MD_NONE),
	m_eHorizon(MOVE_DIR_HORIZON::MD_NONE),
	m_iLayerCheck(0)
{
}


CCamera::~CCamera()
{
}

void CCamera::FinalUpdate()
{

	// 뷰행렬
	m_matView = XMMatrixIdentity();

	Vec3 tPos = GetTransform()->GetLocalPos();

	// 카메라를 원점으로 이동시킨 것 처럼. 모든 오브젝트를 카메라가 원점으로 이동한 만큼을
	// 이동 시켜야 한다.
	m_matView._41 = -tPos.x;
	m_matView._42 = -tPos.y;
	m_matView._43 = -tPos.z;

	// 투영 행렬
	m_matProj = XMMatrixIdentity();
	Resolution tResolution = GET_SINGLE(CDevice)->GetResolution();

	if (m_eType == PROJ_TYPE::PERSPECTIVE)
	{
		m_matProj = XMMatrixPerspectiveFovLH(m_fFOV, tResolution.fWidth / tResolution.fHeight, m_fNear, m_fFar);
	}
	else
	{
		m_matProj = XMMatrixOrthographicLH(tResolution.fWidth * m_fScale, tResolution.fHeight * m_fScale, m_fNear, m_fFar);
	}

	GET_SINGLE(CRenderMgr)->RegisterCamera(this);
	// 카메라는 Final Update에서 RenderMgr의 RegisterCamera함수를 호출하여,
	// 현재 장면의 CameraVector에 카메라를 넣어준다.
	
	// 하나 있는 카메라를 가지고, 이 카메라를 기준으로 ViewSpace로의 변환과
	// Projection Space의 변환을 하고 있다.

	// 카메라의 좌표에 따른 ViewSpace 변환.
	// 화면의 비율과 시야각에 따른 ProjectionSpace 변환




	// BackGroundLayer에서 사용하는 정보로 이것은 Update에서 사용된다.

	// Script
	m_eVertical = MOVE_DIR_VERTICAL::MD_NONE;
	m_eHorizon = MOVE_DIR_HORIZON::MD_NONE;
	// Script End
}

bool CCamera::SaveToScene(FILE * pFile)
{
	UINT iType = (UINT)GetComponentType();
	fwrite(&iType, 4, 1, pFile);

	fwrite(&m_fNear, 4, 1, pFile);
	fwrite(&m_fFar, 4, 1, pFile);

	fwrite(&m_fFOV, 4, 1, pFile);
	fwrite(&m_fScale, 4, 1, pFile);

	fwrite(&m_eType, sizeof(PROJ_TYPE), 1, pFile);
	fwrite(&m_iLayerCheck, 4, 1, pFile);
	
	return true;
}

bool CCamera::LoadFromScene(FILE * pFile)
{
	fread(&m_fNear, 4, 1, pFile);
	fread(&m_fFar, 4, 1, pFile);

	fread(&m_fFOV, 4, 1, pFile);
	fread(&m_fScale, 4, 1, pFile);

	fread(&m_eType, sizeof(PROJ_TYPE), 1, pFile);
	fread(&m_iLayerCheck, 4, 1, pFile);

	return true;
}

void CCamera::Render()
{
	// 자신의 행렬 정보를 셋팅하고, 이것을 토대로 해당 Layer들만 Render함수 호출.
	// 카메라가 찍는 Layer가 정해져 있다.
	g_tTransform.matView = GetViewMatrix();
	g_tTransform.matProj = GetProjMatrix();

	CScene* pCurScene = CSceneMgr::GetInst()->GetScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (m_iLayerCheck & (1 << i))
		{
			pCurScene->GetLayer(i)->Render();
		}
	}
}


