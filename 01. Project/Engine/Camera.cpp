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

	// �����
	m_matView = XMMatrixIdentity();

	Vec3 tPos = GetTransform()->GetLocalPos();

	// ī�޶� �������� �̵���Ų �� ó��. ��� ������Ʈ�� ī�޶� �������� �̵��� ��ŭ��
	// �̵� ���Ѿ� �Ѵ�.
	m_matView._41 = -tPos.x;
	m_matView._42 = -tPos.y;
	m_matView._43 = -tPos.z;

	// ���� ���
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
	// ī�޶�� Final Update���� RenderMgr�� RegisterCamera�Լ��� ȣ���Ͽ�,
	// ���� ����� CameraVector�� ī�޶� �־��ش�.
	
	// �ϳ� �ִ� ī�޶� ������, �� ī�޶� �������� ViewSpace���� ��ȯ��
	// Projection Space�� ��ȯ�� �ϰ� �ִ�.

	// ī�޶��� ��ǥ�� ���� ViewSpace ��ȯ.
	// ȭ���� ������ �þ߰��� ���� ProjectionSpace ��ȯ




	// BackGroundLayer���� ����ϴ� ������ �̰��� Update���� ���ȴ�.

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
	// �ڽ��� ��� ������ �����ϰ�, �̰��� ���� �ش� Layer�鸸 Render�Լ� ȣ��.
	// ī�޶� ��� Layer�� ������ �ִ�.
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


