#include "stdafx.h"
#include "Scene.h"
#include "Layer.h"
#include "Camera.h"

CScene::CScene() :
	m_pLayerArray{},
	m_eSceneType(SCENE_TYPE::NONE)
{
	// �̸� �̸� ���� Layer�� �����صд�.

}


CScene::~CScene()
{
	Safe_Delete_Array(m_pLayerArray);
}


bool CScene::Init()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_pLayerArray[i] = new CLayer;
		m_pLayerArray[i]->SetLayerIdx(i);
	}

	return true;
}

void CScene::Awake()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_pLayerArray[i]->Awake();
	}
}

void CScene::Start()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_pLayerArray[i]->Start();
	}
}

void CScene::Update()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_pLayerArray[i]->Update();
	}
}

void CScene::LateUpdate()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_pLayerArray[i]->LateUpdate();
	}
}

void CScene::FinalUpdate()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_pLayerArray[i]->FinalUpdate();
	}
}

void CScene::Render()
{
	g_tTransform.matView = m_vecCamera[0]->GetViewMatrix();
	g_tTransform.matProj = m_vecCamera[0]->GetProjMatrix();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_pLayerArray[i]->Render();
	}

	m_vecCamera.clear();
}

void CScene::ChangeLayer(int iLayerIdx1, int iLayerIdx2)
{
	if (iLayerIdx1 == iLayerIdx2)
		return;

	else if (iLayerIdx1 < 0 || iLayerIdx2 < 0 || iLayerIdx1 >= MAX_LAYER || iLayerIdx2 >= MAX_LAYER)
		assert(false);

	CLayer* pTemp = m_pLayerArray[iLayerIdx1]; // temp = 1
	m_pLayerArray[iLayerIdx1] = m_pLayerArray[iLayerIdx2]; // 1�� �ڸ��� 2�� ����
	m_pLayerArray[iLayerIdx2] = pTemp; // 2�� �ڸ��� temp�� ����.

	// �� Layer�� �ε����� �ڸ��� �ٲ�鼭, Idx�� �޶�����.
	// �ش� Layer�� ������Ʈ�� �ε����� �ٲ��ش�.

	m_pLayerArray[iLayerIdx1]->SetLayerIdx(iLayerIdx1);
	const vector<CGameObject*>& vecObj1 = m_pLayerArray[iLayerIdx1]->GetLayerObjVec();

	for (size_t i = 0; i < vecObj1.size(); ++i)
		vecObj1[i]->SetLayerIdx(iLayerIdx1);

	m_pLayerArray[iLayerIdx2]->SetLayerIdx(iLayerIdx2);
	const vector<CGameObject*>& vecObj2 = m_pLayerArray[iLayerIdx2]->GetLayerObjVec();

	for (size_t i = 0; i < vecObj2.size(); ++i)
		vecObj2[i]->SetLayerIdx(iLayerIdx2);

}

CLayer* CScene::SetLayerName(const wstring & strName, int iIdx)
{
	if (iIdx < 0 || iIdx >= MAX_LAYER)
		assert(false);

	m_pLayerArray[iIdx]->SetName(strName);

	return m_pLayerArray[iIdx];
}

void CScene::AddGameObject(const wstring & strLayerName, CGameObject * pObj, bool bMoveAll)
{
	CLayer* pLayer = FindLayer(strLayerName);

	if (pLayer == nullptr || pObj == nullptr)
		assert(nullptr);

	pLayer->AddGameObject(pObj, bMoveAll);
}

void CScene::AddGameObject(int iLayerIdx, CGameObject * pObj, bool bMoveAll)
{
	if (iLayerIdx < 0 || iLayerIdx >= MAX_LAYER)
		assert(false);
	else if (pObj == nullptr)
		assert(nullptr);

	m_pLayerArray[iLayerIdx]->AddGameObject(pObj, bMoveAll);
}

CLayer * CScene::FindLayer(const wstring & strName) const
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (m_pLayerArray[i]->GetName() == strName)
			return m_pLayerArray[i];
	}

	return nullptr;
}

CGameObject * CScene::FindParentObj(const wstring & strName) const
{

	for (size_t i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObj = m_pLayerArray[i]->GetParentObjVec();

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			if (vecObj[j]->GetName() == strName)
				return vecObj[j];
		}
	}

	return nullptr;
	
}

CGameObject * CScene::FindObject(const wstring & strName) const
{
	for (size_t i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObj = m_pLayerArray[i]->GetLayerObjVec();

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			if (vecObj[j]->GetName() == strName)
				return vecObj[j];
		}
	}

	return nullptr;
}

