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

	// 카메라 초기화.
	CRenderMgr::GetInst()->ClearCamera();

	m_pScene->FinalUpdate();

	// Collision
	CCollisionMgr::GetInst()->Update();



	// 게임을 진행을 하다가, 특정 이벤트가 발생하면,
	// 이것을 EventMgr의 EventVec에 넣어두었다가,
	// FinalUpdate에서 일괄적으로 처리한다.

	// OBJECT_DELETE 이벤트의 경우에는 이 시점에서 Active = false인 상태가
	// 되고, 다음 EventMgr::Update시점에서 사라진다.
	// 한 프레임이 늦게 삭제가 되는 만큼, 여기서 Active값을 확인하고,
	// vector에서 제외시키는 처리가 필요할 것이다.

	// OBJECT_CREATE 이벤트의 경우에는 요청을 하고, EventMgr::Update에서
	// Layer에 오브젝트가 추가가 되는데, 이 이전에 기존에 있던 오브젝트의
	// Update가 끝난 상태이다. 따라서, 실질적인 Update는 
	// 오브젝트를 추가한 프레임에서는 그 이전에 있던 오브젝트만
	// 처리를 해주고, 다음 프레임에서 추가한 것까지 업데이트를 하게 된다.

	// Con : 특정 시점에서 Event를 실행하게 된다.


}

void CSceneMgr::UpdateTool()
{
	// Sound
	CResMgr::GetInst()->SoundUpdate();

	CRenderMgr::GetInst()->ClearCamera();
	m_pScene->FinalUpdate();

	// Play모드가 아니라면, Update 와 LateUpdate를 하지 않는다.
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
		// play Mode의 경우에는 반드시 메인 카메라를 이용한다.
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
				// 걸린 Object를 vec에 넣는다.
				vecObj.push_back(vecLayerObj[j]);
			}
		}
	}

	// 오름차순 정렬.
	sort(vecObj.begin(), vecObj.end(), GameObjectCom);

	return tPickPos;
}


