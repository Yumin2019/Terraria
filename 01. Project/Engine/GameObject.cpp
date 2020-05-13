#include "stdafx.h"
#include "GameObject.h"
#include "Component.h"

#include "MeshRender.h"
#include "Collider2D.h"
#include "Script.h"
#include "Layer.h"
#include "SceneMgr.h"
#include "Scene.h"

CGameObject::CGameObject() :
	m_pComArray{},
	m_pParent(nullptr),
	m_iLayerIdx(-1),
	m_bEnable(true),
	m_bActive(true),
	m_bChanged(false)
{
}

CGameObject::CGameObject(const CGameObject & game) :
	CEntity(game), // Name����
	m_pComArray{}, //  Component �迭 �ʱ�ȭ.
	m_iLayerIdx(-1), // �츮�� ���ϴ� ������ ��ü�� ��������, Layer���� ������ ���� �ƴϴ�.
	m_pParent(nullptr),
	m_bEnable(game.m_bEnable),
	m_bActive(true)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (game.m_pComArray[i] != nullptr)
		{
			m_pComArray[i] = game.m_pComArray[i]->Clone();
			m_pComArray[i]->SetGameObject(this);
		}
	}

	// �θ� �ڽ��� ���迡�� �ڽ��� ��ü�� �����Ѵ�.
	// �׷���, �� ��Ӱ��迡 ����� ��ü�� ���ԵǴ� ���� �´°� ?
	// ���ʿ� Parent��ü�� �� ����� ��ü�� �˰� ������ ? NO -> nullptr�� ó��
	// ��ü�ϳ��� �����ߴµ�, �ڽı��� ���� NO -> �ƹ� ó���� �� �Ѵ�.

	// Script�� �����Ѵ�.
	m_vecScript.reserve(game.m_vecScript.size());

	for (size_t i = 0; i < game.m_vecScript.size(); ++i)
	{
		CScript* pScript = (CScript*)game.m_vecScript[i]->Clone();
		pScript->SetGameObject(this);
		m_vecScript.push_back(pScript);
	}
}


CGameObject::~CGameObject()
{
	// �� ���(Component*)�� ���� �����Ҵ��� �������ش�.
	Safe_Delete_Array(m_pComArray);

	// �ڽ��� ��ü�� �Ҵ����� �Ѵ�. Layer-> Parent Delete -> Child Delete ������ ó���� �ȴ�.
	Safe_Delete_VecList(m_vecChild);

	// Script�� �����Ѵ�.
	Safe_Delete_VecList(m_vecScript);
}

void CGameObject::ActiveDisable()
{
	m_bActive = false;

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->ActiveDisable();
	}
}

void CGameObject::AddComponent(CComponent * pComponent)
{

	// �־��� Component�� ���. �����ڸ� ���ؼ� �̹� Type�� ������ �ִ� �����̴�.
	COMPONENT_TYPE eType = pComponent->GetComponentType();
	

	if (eType == COMPONENT_TYPE::SCRIPT)
	{
		m_vecScript.push_back((CScript*)pComponent);
	}
	else
	{
		// ������ �߰� �Ǿ��� ���ɼ�.
		SAFE_DELETE(m_pComArray[(UINT)eType]);

		m_pComArray[(UINT)eType] = pComponent;
		// ���� Type�� ���� �ڸ��� pComponent�� �־��ش�.

	}
	
	pComponent->SetGameObject(this);
	// Component�� �ڽ��� �Ҽ� Object�� �˰� �ֵ��� �Ѵ�.
}

void CGameObject::DeleteComponent(COMPONENT_TYPE eType)
{
	// �����Ϸ��� ������, �ش� component�� ���ٸ� assert
	if (GetComponent(eType) == nullptr)
		assert(false);

	SAFE_DELETE(m_pComArray[(UINT)eType]);
}

void CGameObject::DeleteScript(UINT iScriptType)
{
	vector<CScript*>::iterator iter;
	vector<CScript*>::iterator iterEnd = m_vecScript.end();

	for (iter = m_vecScript.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetScriptType() == iScriptType)
		{
			SAFE_DELETE((*iter));
			m_vecScript.erase(iter);
			break;
		}
	}

}

CScript * CGameObject::FindScript(const wstring & strScriptName)
{
	vector<CScript*>::iterator iter;
	vector<CScript*>::iterator iterEnd = m_vecScript.end();

	for (iter = m_vecScript.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == strScriptName)
			return (*iter);
	}

	return nullptr;
}

CScript * CGameObject::FindScript(UINT iScriptType)
{
	vector<CScript*>::iterator iter;
	vector<CScript*>::iterator iterEnd = m_vecScript.end();

	for (iter = m_vecScript.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetScriptType() == iScriptType)
			return *iter;
	}

	return nullptr;
}

void CGameObject::SetDisable(bool bFlag)
{
	if (m_bEnable)
	{
		// Ȱ��ȭ -> ��Ȱ��ȭ
		if (!bFlag)
		{
			Event tEvent;
			tEvent.eType = EVENT_TYPE::DISABLE_GAMEOBJECT;
			tEvent.wParam = (DWORD_PTR)this;

			CEventMgr::GetInst()->AddEvent(tEvent);
		}
	}
	else
	{
		// ��Ȱ��ȭ -> Ȱ��ȭ
		if (bFlag)
		{
			Event tEvent;
			tEvent.eType = EVENT_TYPE::ENABLE_GAMEOBJECT;
			tEvent.wParam = (DWORD_PTR)this;

			CEventMgr::GetInst()->AddEvent(tEvent);
		}
	}
}

void CGameObject::AddChild(CGameObject * pChild)
{
	// �־��� Child�� �ڽ��� �ڽ��� �� �� ����.
	// nullptr�� �ƴϴ�.
	if (pChild == nullptr || pChild == this)
		assert(false);

	// ���� ������ ������Ʈ�� �ڽ����� ���� ���� ���� �� ����. A->B->C->A ��� �Ѵٸ�, 
	// A�� �θ��� C�� ���ϰ�, C�� ���� ������ A�� ���ϰ� �ȴ�. (���� ó���ϱ� ���Ѵ�)
	// �̷� ��� ��İ���� �켱������ �ذ��� �� ���� �������� �߻��Ѵ�.
	else if (IsAncestor(pChild))
		assert(false);

	// ���� ������Ʈ�� ������ �������� �ڽ��̿��� ��쿡�� �� ���踦 ���ְ� 
	// �� GameObject�� �ڽ��� �Ǿ�� �Ѵ�.
	pChild->ClearParent(this);

	// �ڽ��� �־��� ��, �ڽ��� ��� Layer�� �Ҽӿ��� �� ���� ���� �����,
	// �ڽ��� �θ��� Layer�� �־��ִ� ó���� �Ѵ�.
	if (pChild->GetLayerIdx() == -1)
	{
		pChild->m_iLayerIdx = m_iLayerIdx;
	}
}

bool CGameObject::IsAncestor(CGameObject * pObj) const
{
	// ���� �������� �ö󰡸鼭, pObj�� �ִ��� Ȯ���Ѵ�.

	CGameObject* pParent = m_pParent;

	while (pParent != nullptr)
	{
		if (pObj == pParent)
			return true;

		pParent = pParent->m_pParent;
	}

	return false;
}

void CGameObject::ClearParent(CGameObject * pNextParent)
{
	// pNextParent �� �������� �� GameObject�� �θ� �Ǵ� ��ü�� ������ �ȴ�.
	// �⺻ ���ڷ� nullptr�� ����ϴµ�, �� ��쿡�� �ܼ��� ���� ���踦 ���ִ� �ǹ̰� �ǰ�,
	// ������Ʈ�� �ִ� ��쿡�� ������ ���踦 ����Ͽ�(���� ��쵵 �ִ�) ���ο� �θ� ���� ó���� ���ش�.

	if (m_pParent == nullptr)
	{
		// �θ� ���� ���.

		// ������ ���� ������, ���� �θ�� ������ ���� ����.
		if (pNextParent == nullptr)
			return;

		else
		{
			// ���� �θ� �����Ѵ�.
			pNextParent->m_vecChild.push_back(this);
			m_pParent = pNextParent;

		
		}

		// �ֻ��� �θ� ������Ʈ
		// ���� �θ� �����Ǵ� ��� (Layer ParentList���� ���� ������ �ȴ�.)
		if (m_iLayerIdx != -1)
		{

			// �� ������Ʈ�� �θ� ���ٰ�, �θ� ���� ���̴�. ����, �� ������Ʈ�� �ڽ��� Layer����
			// ParentVec�� �����Ѵ�. �� ��쿡�� �ڽ��� Layer�� ParentVec���� �������� �Ѵ�. (�θ� �������ϱ�)
			CLayer* pLayer = CSceneMgr::GetInst()->GetScene()->GetLayer(m_iLayerIdx);
			pLayer->CheckParentObj();
		}


		// ���࿡ ���� Layer�� �������� �ʴ´ٸ�... Ư�� Layer����
		// ParentVec�� ���������� �ʾҴٴ� ���̴�. Layer�� �־�� �θ� Vec�� �����غ� �� �ִµ�, Layer�� ������
		// �θ� vector���� �������� �ʾƵ� �ǰ�(����� ����), �ڽ����� �߰��Ǵ� ��츸 �����ϸ� �ȴ�. 
	}
	else
	{
		// �θ� �ִ� ��� 

		// �θ� �ִ� �����, ClearParent �ϱ� �ϴ� �θ��ʿ��� �ڽ��� ������ ���ֹ�����.
		vector<CGameObject*>::iterator iter;
		vector<CGameObject*>::iterator iterEnd = m_pParent->m_vecChild.end();

		for (iter = m_pParent->m_vecChild.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter) == this)
			{
				m_pParent->m_vecChild.erase(iter);
				break;
			}
		}

		if (pNextParent == nullptr)
		{
			// ���� �θ� �������� ���� �����, �ܼ��� �θ� �ڽ��� ���踦 ���شٴ� �ǹ��̴�.
			// �̰��� �ش� ������Ʈ�� �ֻ��� ��ü�� �ȴٴ� ���� �ǹ��ϱ⵵ �Ѵ�.
			m_pParent = nullptr;

			if (m_bActive)
			{
				// �ڽ��� �Ҽ� Layer���� �ֻ��� ��ü�� �־��ش�.
				CLayer* pLayer = CSceneMgr::GetInst()->GetScene()->GetLayer(m_iLayerIdx);
				pLayer->AddGameObject(this); // �θ� ������, Parent�� ����.

				// �θ��� Layer�� �ö󰡴� ��쿡�� �θ��� ��ķ� �ö󰡴µ�, ���ڱ� �θ��� ������ ���� ���� �ʰ� �ȴ�.
				
			}

			// ���� ���踦 �̷�� �ִ� ������Ʈ ���̿���, �ڽ��� ���� 
			// ������Ʈ�� �ƴ� ��쿡�� Layer�� ParentVec�� �̵���Ų��.

			// ��ü�� ������� ������ ClearParent�Լ��� ȣ���ϰ� �ȴ�.
			// (�ױ� ���� ���� ���踦 �����ؾ� �ϴϱ�)
			// ���⼭ �ش� ��ü�� ���� ���°� �Ǵµ�, 
			// �� ��쿡 �� ������Ʈ�� ���� ���̹Ƿ� ParentVec
			// ���� �ű�� ó���� ���� �ʴ´�.
		
		}

		else
		{
			// �θ𿡼� �ڽ��� ������ �����, �ڽ��� �� �θ� ��´�.
			m_pParent = pNextParent;
			pNextParent->m_vecChild.push_back(this);

			// ���⼭ AddChild�� ȣ���ؼ� ������ �� �� ��������� �𸣰ڴµ� �� ������.
		}
	}
}

void CGameObject::Awake()
{


	if (!m_bEnable)
		return;

	// �θ� ��Ȱ��ȭ �Ǿ� �ִ�. -> �ڽĶ��� Awake�� ��ȸ�� ���� ����.
	// �θ� Ȱ��ȭ -> �ڽ��� Ȱ��ȭ ���ο� ����, ó���ϰ� �ȴ�.

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_pComArray[i] != nullptr && m_pComArray[i]->IsEnable())
			m_pComArray[i]->Awake();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		if(m_vecChild[i]->IsEnable())
			m_vecChild[i]->Awake();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		if(m_vecScript[i]->IsEnable())
			m_vecScript[i]->Awake();
	}
}

void CGameObject::Start()
{


	if (!m_bEnable)
		return;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_pComArray[i] != nullptr && m_pComArray[i]->IsEnable())
			m_pComArray[i]->Start();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsEnable())
			m_vecChild[i]->Start();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		if (m_vecScript[i]->IsEnable())
			m_vecScript[i]->Start();
	}
}

void CGameObject::Update()
{

	if (!m_bEnable)
		return;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_pComArray[i] != nullptr && m_pComArray[i]->IsEnable())
			m_pComArray[i]->Update();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsEnable())
			m_vecChild[i]->Update();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		if (m_vecScript[i]->IsEnable())
			m_vecScript[i]->Update();
	}

}

void CGameObject::LateUpdate()
{

	if (!m_bEnable)
		return;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_pComArray[i] != nullptr && m_pComArray[i]->IsEnable())
			m_pComArray[i]->LateUpdate();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsEnable())
			m_vecChild[i]->LateUpdate();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		if (m_vecScript[i]->IsEnable())
			m_vecScript[i]->LateUpdate();
	}
}

void CGameObject::FinalUpdate()
{
	m_bChanged = false;

	if (!m_bEnable)
		return;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_pComArray[i] != nullptr && m_pComArray[i]->IsEnable())
			m_pComArray[i]->FinalUpdate();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsEnable())
			m_vecChild[i]->FinalUpdate();
	}


}

void CGameObject::RegisterToLayer()
{
	// Layer�� ������Ʈ�� ����ش�.
	if (m_iLayerIdx == -1)
		assert(false);

	CLayer* pLayer = CSceneMgr::GetInst()->GetScene()->GetLayer(m_iLayerIdx);
	pLayer->RegisterObj(this);

	for (size_t i = 0; i < m_vecChild.size(); ++i)
		m_vecChild[i]->RegisterToLayer();
}



void CGameObject::Render()
{

	if (!m_bEnable)
		return;

	// Render�� ������ Object�� MeshRender�� Component�� ������ ������Ʈ�̴�.
	// MeshRender�� �����ͼ� �ִ� ��쿡 Render�Լ��� ȣ�����ش�.
	CMeshRender* pMeshRender = GetMeshRender(); 

	if (pMeshRender != nullptr && pMeshRender->IsEnable())
		pMeshRender->Render();

	CCollider2D* pCollider = GetCollider2D();

	if (pCollider != nullptr && pCollider->IsEnable())
		pCollider->Render();

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		if(m_vecScript[i]->IsEnable())
			m_vecScript[i]->Render();
	}
}

void CGameObject::Enable()
{
	for (size_t i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_pComArray[i] != nullptr && m_pComArray[i]->IsEnable())
			m_pComArray[i]->Enable();

		// 1. GameObject�� Ȱ��ȭ�� �ȴٰ� �ص� Component�� Ȱ��ȭ�� �� ���� �ƴ�.
		// 2. GameObject�� ��Ȱ��ȭ���� Ȱ��ȭ�� �� ��쿡 . . .
		// 2-1. Component Ȱ��ȭ    Ȱ��ȭ �Լ� ȣ��.
		// 2-2.			  ��Ȱ��ȭ   1���� ������ ó������ ����.
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Enable();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		if(m_vecScript[i]->IsEnable())
			m_vecScript[i]->Enable();
	}
}

void CGameObject::Disable()
{
	for (size_t i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_pComArray[i] != nullptr && m_pComArray[i]->IsEnable())
			m_pComArray[i]->Disable();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Disable();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		if (m_vecScript[i]->IsEnable())
			m_vecScript[i]->Disable();
	}

}
