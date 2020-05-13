#include "stdafx.h"
#include "SceneMgr.h"


#include "Layer.h"

// Mgr
#include "EventMgr.h"
#include "CollisionMgr.h"
#include "ResMgr.h"
#include "RenderMgr.h"
#include "Core.h"
#include "Device.h"


#include "Camera.h"
#include "Transform.h"
#include "Scene.h"



DEFINITION_SINGLE(CSceneMgr);

CSceneMgr::CSceneMgr() :
	m_pScene(nullptr),
	m_pNextScene(nullptr)
{
}


CSceneMgr::~CSceneMgr()
{
	SAFE_DELETE(m_pScene);
	SAFE_DELETE(m_pNextScene);
}

bool CSceneMgr::Init()
{
	m_pScene = new CScene;
	m_pScene->SetName(L"Test Scene");

	m_pScene->Init();

	return true;
}

void CSceneMgr::Update()
{
	// Sound
	CResMgr::GetInst()->SoundUpdate();


	// Update
	m_pScene->Update();
	m_pScene->LateUpdate();

	// ī�޶� �ʱ�ȭ.
	CRenderMgr::GetInst()->ClearCamera();

	m_pScene->FinalUpdate();

	// Collision
	CCollisionMgr::GetInst()->Update();



	// ������ ������ �ϴٰ�, Ư�� �̺�Ʈ�� �߻��ϸ�,
	// �̰��� EventMgr�� EventVec�� �־�ξ��ٰ�,
	// FinalUpdate���� �ϰ������� ó���Ѵ�.

	// OBJECT_DELETE �̺�Ʈ�� ��쿡�� �� �������� Active = false�� ���°�
	// �ǰ�, ���� EventMgr::Update�������� �������.
	// �� �������� �ʰ� ������ �Ǵ� ��ŭ, ���⼭ Active���� Ȯ���ϰ�,
	// vector���� ���ܽ�Ű�� ó���� �ʿ��� ���̴�.

	// OBJECT_CREATE �̺�Ʈ�� ��쿡�� ��û�� �ϰ�, EventMgr::Update����
	// Layer�� ������Ʈ�� �߰��� �Ǵµ�, �� ������ ������ �ִ� ������Ʈ��
	// Update�� ���� �����̴�. ����, �������� Update�� 
	// ������Ʈ�� �߰��� �����ӿ����� �� ������ �ִ� ������Ʈ��
	// ó���� ���ְ�, ���� �����ӿ��� �߰��� �ͱ��� ������Ʈ�� �ϰ� �ȴ�.

	// Con : Ư�� �������� Event�� �����ϰ� �ȴ�.


}

void CSceneMgr::UpdateTool()
{
	// Sound
	CResMgr::GetInst()->SoundUpdate();

	CRenderMgr::GetInst()->ClearCamera();
	m_pScene->FinalUpdate();

	// Play��尡 �ƴ϶��, Update �� LateUpdate�� ���� �ʴ´�.
}

void CSceneMgr::ChangeScene(CScene * pNextScene)
{
	SAFE_DELETE(m_pScene);
	m_pScene = pNextScene;
}

void CSceneMgr::FindObjByTag(const wstring & strName, vector<CGameObject*>& vecObj)
{
	for (size_t i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecLayerObj = m_pScene->GetLayer(i)->GetParentObjVec();

		for (size_t j = 0; j < vecLayerObj.size(); ++j)
		{
			if (vecLayerObj[j]->GetName() == strName)
				vecObj.push_back(vecLayerObj[j]);
		}
	}

}

bool GameObjectCom(const CGameObject* pLeft, const CGameObject* pRight)
{
	return (pLeft->GetTransform()->GetWorldPos().z < pRight->GetTransform()->GetWorldPos().z);
}

Vec3 CSceneMgr::FindObjByPoint(POINT tPoint, vector<CGameObject*>& vecObj, CCamera * pToolCamera)
{
	CCamera* pCamera = pToolCamera;
	if (CCore::GetInst()->GetSceneMode() == SCENE_MODE::PLAY)
	{
		// play Mode�� ��쿡�� �ݵ�� ���� ī�޶� �̿��Ѵ�.
		pCamera = CRenderMgr::GetInst()->GetCamera(0);
	}

	Resolution tRes = CDevice::GetInst()->GetResolution();
	// Wnd Pos -> Game Pos
	Vec3	tPickPos = Vec3((float)tPoint.x - (tRes.fWidth / 2.f), (tRes.fHeight) / 2.f - (float)tPoint.y, 0.f);
	tPickPos *= pCamera->GetScale();
	tPickPos += pCamera->GetTransform()->GetWorldPos();

	for (size_t i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecLayerObj = m_pScene->GetLayer(i)->GetLayerObjVec();

		for (size_t j = 0; j < vecLayerObj.size(); ++j)
		{
			if (vecLayerObj[j]->GetTransform()->IsCasting(tPickPos))
			{
				// �ɸ� Object�� vec�� �ִ´�.
				vecObj.push_back(vecLayerObj[j]);
			}
		}
	}

	// �������� ����.
	sort(vecObj.begin(), vecObj.end(), GameObjectCom);

	return tPickPos;
}


