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
	// �� ���Ϳ� �Ҵ�� Object�� ����
}

void CLayer::AddGameObject(CGameObject * pObject, bool bMoveAll)
{

	/*
		GameObject�� Layer�� �־��ִ� �Լ��̴�. ó���� �־��ִ� Object�� ��쿡��
		�ݵ�� �ش� Layer�� �־��ָ�, �� Object�� �ڽĵ��� bool ���� ������ �޾Ƽ�,
		�� Layer�� �Ű�����, �ƴϸ� �׳� �ڽ��� Layer�� �״�� ���������� ���ϰ� �ȴ�.
	*/
	

	if (pObject->GetParent() == nullptr)
	{
		// �־��� ������Ʈ�� �θ� ���� ��� : �ֻ��� ������Ʈ�̴�.
		m_vecParentObj.push_back(pObject);

		// ���� �ٸ� Layer�� �̹� �Ҽ��� �Ǿ��ִ� ��쿡�� �� Layer���� �� ������Ʈ�� �����ִ� ó���� �Ѵ�.
		if (pObject->GetLayerIdx() != -1)
		{
			if (m_iLayerIdx != pObject->GetLayerIdx())
			{
				// ���� Layer�� ���� ������ �ϴ� Layer�� �ٸ��ٴ� �����Ͽ� �̷�� ����.
				// Parent Layer�� ���� ���ؼ�, ���� �Ҽӿ��� �־��� ���ɼ��� �����Ƿ� �� ������ ������ ���� �´�.
				// Layer�� ����Ǿ��� ���� �����ϱ�,

				CScene* pScene = CSceneMgr::GetInst()->GetScene();
				pScene->GetLayer(pObject->GetLayerIdx())->RemoveParentObj(pObject);
				// �� ������Ʈ�� �θ� ��������... �翬�� �� Layer���� �θ� vector�� ������ ���̴�.
			}
		}

	}

	// �ٸ� Layer�� �Ҽ��� �ƴ� �����, ó���� ���� �ʰ� �ٷ� LayerIdx�� �ش�.
	pObject->SetLayerIdx(m_iLayerIdx);

	list<CGameObject*> queue;

	const vector<CGameObject*>& vecChild = pObject->GetChildVec();
	queue.insert(queue.end(), vecChild.begin(), vecChild.end());
	// queue�� Ư�� �������� ... vec�� begin end ������ ��Ҹ� �־��ش�.
	// Object�� ���� ó���� �ѱ��, �ڽĵ鿡 ���� ó���� ���ִ� ���̴�.

	// BFS�� ��Ĵ��, ���� �������� ���� �������� ��� ��ȸ�ϸ� �ڽĿ� �����ϰ�...
	// �ڽ��� �ϳ���, ���� Layer�� �ٲ��ش�.

	while (!queue.empty())
	{
		CGameObject* pChild = queue.front();
		queue.pop_front();

		// Child�� ChilldVec�� queue�� �߰��Ѵ�.
		const vector<CGameObject*>& vecChildChild = pChild->GetChildVec();
		queue.insert(queue.end(), vecChildChild.begin(), vecChildChild.end());

		// ���� ���� Child�� ���ؼ� ó���� ���ش�.
		if (bMoveAll)
		{
			pChild->SetLayerIdx(m_iLayerIdx);
			// �ڽĿ�����Ʈ�� ��쿡�� �ش� Layer���� �θ� �ش����� �ʾ��� ���̴�.
			// �׸��� �ڽ��� ���忡�� Layer�� �Ҽ��̶�� ���� �ܼ��� Idx�� �����ϸ� �׸��̴�.
		}
		else
		{
			// �ڽ��� Layer�� �����ϴ� ����̴�. ������, Layer�� �����Ǿ� ���� ���� ����� ����ó���� ���ش�.
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
		// �θ� ������Ʈ�� ���� vecParent ��ü���� ���ֹ����� ó���� ���ش�.
		// �̵��� �θ� ������ �ִٸ� �� �̻� �ֻ��� ��ü�� ���� �ڰ��� ����.

		if ((*iter)->GetParent() != nullptr)
		{
			// �θ� �ִ� ���. �� ��ü�� ParentVec���� �����ش�.
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

	// pObj�� Parent��� �ν��� �ϰ� RemoveParentObj�Լ��� ȣ���ߴµ�, �� ��ü�� ���ٴ� ���� ���� ������ �ִٴ� �Ҹ�
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


	// Update�������� ���� ������Ʈ�� ���ؼ� �����ִ� ó���� ���ش�.
	// ��� �������� IsActive�� üũ���� �����Ƿ�, ������ ����� ���Ѵ�.

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

		// ������Ʈ�� Ȱ�����ο� ���� LateUpdate�� ó�����ְ�,
		// ���ο� ������� �׻� Layer�� Vector�� �־��ش�.
		// �̸� ���ؼ� CollisonMgr������ Ȱ�� ���ο� ������� ������Ʈ ����� ������ �� �ִ�.
		// ���� �ش� ������Ʈ Ȥ�� Collider2D Component�� ��Ȱ��ȭ �Ǿ��ִ� ��쿡 �ݵ�� �浹 falseó���� 
		// �ϰ� �Ǵµ�, �̷� ��츦 ���ؼ� Ȱ��ȭ�� ���ξ��� vector�� �߰����ش�.
		// �浹 Exit������ ȣ���� ���ؼ�, ���Ϳ� �߰�����߸� �Ѵ�.

		if ((*iter)->IsActive() == false)
		{
			iter = m_vecParentObj.erase(iter);
			iterEnd = m_vecParentObj.end();
			continue;
		}

		++iter;

		/*
		���⼭�� Dead ������ Obj�� �����ִ� ó���� ���ش�.
		
		*/
	}

}

void CLayer::Render()
{
	// Layer�� Render�� Gameobject�� ���ؼ� ����� Layer�Ҽ��� ��� Object�� ��Ƽ� �ѹ��� ó���� ���ش�..

	for (size_t i = 0; i < m_vecLayerObj.size(); ++i)
	{
		m_vecLayerObj[i]->Render();
	}
}
