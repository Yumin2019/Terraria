#include "stdafx.h"
#include "Layer.h"

#include "GameObject.h"
#include "Scene.h"
#include "SceneMgr.h"

CLayer::CLayer()
{
}


CLayer::~CLayer()
{
	Safe_Delete_VecList(m_vecParentObj);
	// 각 벡터에 할당된 Object를 해제
}

void CLayer::AddGameObject(CGameObject * pObject, bool bMoveAll)
{

	/*
		GameObject를 Layer에 넣어주는 함수이다. 처음에 넣어주는 Object의 경우에는
		반드시 해당 Layer에 넣어주며, 그 Object의 자식들은 bool 값의 영향을 받아서,
		이 Layer로 옮겨질지, 아니면 그냥 자신의 Layer에 그대로 남아있을지 정하게 된다.
	*/
	

	if (pObject->GetParent() == nullptr)
	{
		// 넣어준 오브젝트의 부모가 없는 경우 : 최상위 오브젝트이다.
		m_vecParentObj.push_back(pObject);

		// 만약 다른 Layer에 이미 소속이 되어있던 경우에는 그 Layer에서 이 오브젝트를 없애주는 처리를 한다.
		if (pObject->GetLayerIdx() != -1)
		{
			if (m_iLayerIdx != pObject->GetLayerIdx())
			{
				// 이전 Layer가 현재 가려고 하는 Layer와 다르다는 가정하에 이루어 진다.
				// Parent Layer로 가기 위해서, 이전 소속에서 있었을 가능성이 있으므로 이 정보를 날리는 것이 맞다.
				// Layer가 변경되었을 수가 있으니까,

				CScene* pScene = CSceneMgr::GetInst()->GetScene();
				pScene->GetLayer(pObject->GetLayerIdx())->RemoveParentObj(pObject);
				// 이 오브젝트의 부모가 없었으니... 당연히 그 Layer에서 부모 vector에 존재할 것이다.
			}
		}

	}

	// 다른 Layer의 소속이 아닌 경우라면, 처리를 하지 않고 바로 LayerIdx를 준다.
	pObject->SetLayerIdx(m_iLayerIdx);

	list<CGameObject*> queue;

	const vector<CGameObject*>& vecChild = pObject->GetChildVec();
	queue.insert(queue.end(), vecChild.begin(), vecChild.end());
	// queue의 특정 지점부터 ... vec의 begin end 사이의 요소를 넣어준다.
	// Object에 대한 처리는 넘기고, 자식들에 대한 처리를 해주는 것이다.

	// BFS의 방식대로, 상위 계층부터 하위 계층까지 모두 순회하며 자식에 접근하고...
	// 자식을 하나씩, 현재 Layer로 바꿔준다.

	while (!queue.empty())
	{
		CGameObject* pChild = queue.front();
		queue.pop_front();

		// Child의 ChilldVec를 queue에 추가한다.
		const vector<CGameObject*>& vecChildChild = pChild->GetChildVec();
		queue.insert(queue.end(), vecChildChild.begin(), vecChildChild.end());

		// 현재 뽑은 Child에 대해서 처리를 해준다.
		if (bMoveAll)
		{
			pChild->SetLayerIdx(m_iLayerIdx);
			// 자식오브젝트의 경우에는 해당 Layer에서 부모에 해당하지 않았을 것이다.
			// 그리고 자식의 입장에서 Layer의 소속이라는 것은 단순히 Idx만 지정하면 그만이다.
		}
		else
		{
			// 자신의 Layer를 유지하는 경우이다. 하지만, Layer가 지정되어 있지 않은 경우라면 예외처리를 해준다.
			if (pChild->GetLayerIdx() == -1)
				pChild->SetLayerIdx(m_iLayerIdx);
		}
	}
}

void CLayer::CheckParentObj()
{
	vector<CGameObject*>::iterator iter;
	vector<CGameObject*>::iterator iterEnd = m_vecParentObj.end();

	for (iter = m_vecParentObj.begin(); iter != iterEnd;)
	{
		// 부모 오브젝트를 가진 vecParent 객체들을 없애버리는 처리를 해준다.
		// 이들이 부모를 가지고 있다면 더 이상 최상위 객체로 있을 자격이 없다.

		if ((*iter)->GetParent() != nullptr)
		{
			// 부모가 있는 경우. 이 객체를 ParentVec에서 지워준다.
			iter = m_vecParentObj.erase(iter);
			iterEnd = m_vecParentObj.end();

			continue;
		}
		else
			++iter;
	}
}

void CLayer::RemoveParentObj(CGameObject * pObj)
{
	vector<CGameObject*>::iterator iter;
	vector<CGameObject*>::iterator iterEnd = m_vecParentObj.end();

	for (iter = m_vecParentObj.begin(); iter != iterEnd; ++iter)
	{
		if (*iter == pObj)
		{
			m_vecParentObj.erase(iter);
			return;
		}
	}

	// pObj가 Parent라고 인식을 하고 RemoveParentObj함수를 호출했는데, 이 객체가 없다는 것은 무슨 문제가 있다는 소리
	assert(false);
}


void CLayer::Awake()
{
	vector<CGameObject*>::iterator iter;
	vector<CGameObject*>::iterator iterEnd = m_vecParentObj.end();

	for (iter = m_vecParentObj.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->IsEnable())
			(*iter)->Awake();
	}
}

void CLayer::Start()
{
	vector<CGameObject*>::iterator iter;
	vector<CGameObject*>::iterator iterEnd = m_vecParentObj.end();

	for (iter = m_vecParentObj.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->IsEnable())
			(*iter)->Start();
	}
}

void CLayer::Update()
{


	// Update시점에서 죽은 오브젝트에 대해서 없애주는 처리를 해준다.
	// 모든 시점에서 IsActive를 체크하지 않으므로, 성능의 향상을 꾀한다.

	vector<CGameObject*>::iterator iter;
	vector<CGameObject*>::iterator iterEnd = m_vecParentObj.end();

	for (iter = m_vecParentObj.begin(); iter != iterEnd; )
	{
	
		if ((*iter)->IsEnable())
			(*iter)->Update();

		++iter;
	}
	
}

void CLayer::LateUpdate()
{

	vector<CGameObject*>::iterator iter;
	vector<CGameObject*>::iterator iterEnd = m_vecParentObj.end();

	for (iter = m_vecParentObj.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->IsEnable())
			(*iter)->LateUpdate();
	}
}

void CLayer::FinalUpdate()
{

	m_vecLayerObj.clear();

	vector<CGameObject*>::iterator iter;
	vector<CGameObject*>::iterator iterEnd = m_vecParentObj.end();

	for (iter = m_vecParentObj.begin(); iter != iterEnd; )
	{
		if ((*iter)->IsEnable())
			(*iter)->FinalUpdate();

		(*iter)->RegisterToLayer();

		// 오브젝트의 활성여부에 따라 LateUpdate를 처리해주고,
		// 여부와 상관없이 항상 Layer의 Vector에 넣어준다.
		// 이를 통해서 CollisonMgr에서는 활성 여부에 상관없이 오브젝트 목록을 가져올 수 있다.
		// 만약 해당 오브젝트 혹은 Collider2D Component가 비활성화 되어있던 경우에 반드시 충돌 false처리를 
		// 하게 되는데, 이런 경우를 위해서 활성화의 여부없이 vector에 추가해준다.
		// 충돌 Exit까지의 호출을 위해서, 벡터에 추가해줘야만 한다.

		if ((*iter)->IsActive() == false)
		{
			iter = m_vecParentObj.erase(iter);
			iterEnd = m_vecParentObj.end();
			continue;
		}

		++iter;

		/*
		여기서는 Dead 상태의 Obj를 없애주는 처리를 해준다.
		
		*/
	}

}

void CLayer::Render()
{
	// Layer의 Render는 Gameobject를 통해서 모아진 Layer소속의 모든 Object를 모아서 한번에 처리를 해준다..

	for (size_t i = 0; i < m_vecLayerObj.size(); ++i)
	{
		m_vecLayerObj[i]->Render();
	}
}
