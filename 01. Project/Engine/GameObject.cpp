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
	CEntity(game), // Name복사
	m_pComArray{}, //  Component 배열 초기화.
	m_iLayerIdx(-1), // 우리가 원하는 동작은 객체의 생성이지, Layer까지 지정한 것은 아니다.
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

	// 부모 자식의 관계에서 자식인 객체를 복사한다.
	// 그러면, 그 상속관계에 복사된 객체가 포함되는 것이 맞는가 ?
	// 애초에 Parent객체는 이 복사된 객체를 알고 있을까 ? NO -> nullptr로 처리
	// 객체하나를 복사했는데, 자식까지 복사 NO -> 아무 처리도 안 한다.

	// Script를 복사한다.
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
	// 각 요소(Component*)에 대한 동적할당을 해제해준다.
	Safe_Delete_Array(m_pComArray);

	// 자식의 객체를 할당해제 한다. Layer-> Parent Delete -> Child Delete 순으로 처리가 된다.
	Safe_Delete_VecList(m_vecChild);

	// Script를 삭제한다.
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

	// 넣어준 Component의 경우. 생성자를 통해서 이미 Type이 정해져 있는 상태이다.
	COMPONENT_TYPE eType = pComponent->GetComponentType();
	

	if (eType == COMPONENT_TYPE::SCRIPT)
	{
		m_vecScript.push_back((CScript*)pComponent);
	}
	else
	{
		// 이전에 추가 되었을 가능성.
		SAFE_DELETE(m_pComArray[(UINT)eType]);

		m_pComArray[(UINT)eType] = pComponent;
		// 얻어온 Type을 토대로 자리에 pComponent를 넣어준다.

	}
	
	pComponent->SetGameObject(this);
	// Component는 자신의 소속 Object를 알고 있도록 한다.
}

void CGameObject::DeleteComponent(COMPONENT_TYPE eType)
{
	// 삭제하려고 했으나, 해당 component가 없다면 assert
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
		// 활성화 -> 비활성화
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
		// 비활성화 -> 활성화
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
	// 넣어준 Child는 자신의 자식이 될 수 없다.
	// nullptr도 아니다.
	if (pChild == nullptr || pChild == this)
		assert(false);

	// 상위 계층의 오브젝트가 자식으로 오는 경우는 있을 수 없다. A->B->C->A 라고 한다면, 
	// A는 부모인 C를 원하고, C는 상위 계층인 A를 원하게 된다. (먼저 처리하길 원한다)
	// 이런 경우 행렬계산의 우선순위를 해결할 수 없는 문제점이 발생한다.
	else if (IsAncestor(pChild))
		assert(false);

	// 들어온 오브젝트가 이전에 누군가의 자식이였던 경우에는 그 관계를 없애고 
	// 이 GameObject의 자식이 되어야 한다.
	pChild->ClearParent(this);

	// 자식을 넣어줄 때, 자식이 어느 Layer에 소속에도 들어가 있지 않은 경우라면,
	// 자식을 부모의 Layer에 넣어주는 처리를 한다.
	if (pChild->GetLayerIdx() == -1)
	{
		pChild->m_iLayerIdx = m_iLayerIdx;
	}
}

bool CGameObject::IsAncestor(CGameObject * pObj) const
{
	// 상위 계층으로 올라가면서, pObj가 있는지 확인한다.

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
	// pNextParent 는 다음번에 이 GameObject의 부모가 되는 객체가 들어오게 된다.
	// 기본 인자로 nullptr을 사용하는데, 이 경우에는 단순히 계층 관계를 없애는 의미가 되고,
	// 오브젝트가 있는 경우에는 이전의 관계를 고려하여(없는 경우도 있다) 새로운 부모에 대한 처리를 해준다.

	if (m_pParent == nullptr)
	{
		// 부모가 없는 경우.

		// 정리할 것이 없으며, 다음 부모로 지정할 것이 없다.
		if (pNextParent == nullptr)
			return;

		else
		{
			// 다음 부모를 지정한다.
			pNextParent->m_vecChild.push_back(this);
			m_pParent = pNextParent;

		
		}

		// 최상위 부모 오브젝트
		// 다음 부모가 지정되는 경우 (Layer ParentList에서 직접 빠지게 된다.)
		if (m_iLayerIdx != -1)
		{

			// 이 오브젝트는 부모가 없다가, 부모가 생긴 것이다. 따라서, 이 오브젝트는 자신의 Layer에서
			// ParentVec에 존재한다. 이 경우에는 자신의 Layer의 ParentVec에서 없어져야 한다. (부모가 생겼으니까)
			CLayer* pLayer = CSceneMgr::GetInst()->GetScene()->GetLayer(m_iLayerIdx);
			pLayer->CheckParentObj();
		}


		// 만약에 이전 Layer가 존재하지 않는다면... 특정 Layer에서
		// ParentVec에 존재하지도 않았다는 것이다. Layer가 있어야 부모 Vec를 생각해볼 수 있는데, Layer가 없으니
		// 부모 vector쪽은 생각하지 않아도 되고(지우는 것을), 자식으로 추가되는 경우만 생각하면 된다. 
	}
	else
	{
		// 부모가 있는 경우 

		// 부모가 있는 경우라면, ClearParent 니까 일단 부모쪽에서 자신의 정보를 없애버린다.
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
			// 다음 부모가 지정되지 않은 경우라면, 단순히 부모 자식의 관계를 없앤다는 의미이다.
			// 이것은 해당 오브젝트가 최상위 객체가 된다는 것을 의미하기도 한다.
			m_pParent = nullptr;

			if (m_bActive)
			{
				// 자신의 소속 Layer에서 최상위 객체로 넣어준다.
				CLayer* pLayer = CSceneMgr::GetInst()->GetScene()->GetLayer(m_iLayerIdx);
				pLayer->AddGameObject(this); // 부모가 없으니, Parent로 들어간다.

				// 부모의 Layer로 올라가는 경우에는 부모의 행렬로 올라가는데, 갑자기 부모의 영향을 전혀 받지 않게 된다.
				
			}

			// 계층 관계를 이루고 있는 오브젝트 사이에서, 자신이 죽을 
			// 오브젝트가 아닌 경우에는 Layer의 ParentVec로 이동시킨다.

			// 객체가 사라지는 순간에 ClearParent함수를 호출하게 된다.
			// (죽기 전에 계층 관계를 정리해야 하니까)
			// 여기서 해당 객체는 죽은 상태가 되는데, 
			// 이 경우에 이 오브젝트는 죽을 것이므로 ParentVec
			// 으로 옮기는 처리를 하지 않는다.
		
		}

		else
		{
			// 부모에서 자식의 정보를 지우고, 자식은 새 부모를 잡는다.
			m_pParent = pNextParent;
			pNextParent->m_vecChild.push_back(this);

			// 여기서 AddChild를 호출해서 문제가 왜 안 생겼는지는 모르겠는데 안 생겼음.
		}
	}
}

void CGameObject::Awake()
{


	if (!m_bEnable)
		return;

	// 부모가 비활성화 되어 있다. -> 자식또한 Awake의 기회를 받지 않음.
	// 부모가 활성화 -> 자식의 활성화 여부에 따라서, 처리하게 된다.

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
	// Layer의 오브젝트를 모아준다.
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

	// Render가 가능한 Object는 MeshRender를 Component로 가지는 오브젝트이다.
	// MeshRender를 가져와서 있는 경우에 Render함수를 호출해준다.
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

		// 1. GameObject가 활성화가 된다고 해도 Component의 활성화가 된 것은 아님.
		// 2. GameObject의 비활성화에서 활성화가 된 경우에 . . .
		// 2-1. Component 활성화    활성화 함수 호출.
		// 2-2.			  비활성화   1번의 이유로 처리하지 않음.
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
