#include "stdafx.h"
#include "EventMgr.h"

#include "SceneMgr.h"
#include "Scene.h"
#include "GameObject.h"
#include "Component.h"

CEventMgr::CEventMgr()
{
}


CEventMgr::~CEventMgr()
{
}

void CEventMgr::Update()
{
	// ���� ������ GameObject�� �޸𸮸� �����Ѵ�.

	for (size_t i = 0; i < m_vecDead.size(); ++i)
	{
		// ��ü�� �ױ� ����, ���� ���迡�� ���ܽ�Ų��.
		m_vecDead[i]->ClearParent();
		SAFE_DELETE(m_vecDead[i]);
	}

	m_vecDead.clear();

	// Event�� ó���Ѵ�.
	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		Execute(m_vecEvent[i]);
	}

}

void CEventMgr::Execute(const Event & tEvent)
{
	switch (tEvent.eType)
	{
	case EVENT_TYPE::CREATE_OBJECT:
		CSceneMgr::GetInst()->GetScene()->
			AddGameObject((int)tEvent.lParam, (CGameObject*)tEvent.wParam, true);
		// �Ѿ�� Event ����ü�� lPram�� int������ casting�Ͽ�, ���̾��� ��ȣ�� �Ѱ��ְ� (�׷��� �����͸� �ִϱ�)
		// wParam���� CGameObject*�� �����صξ�����, (int�����ٰ� ����) �̰��� �ٽ� �־��ִ� �������� ����ȯ��
		// ���ش�.

		// �־��� GameObject�� �ش� ���̾ �߰����ش�. (���� ��� ���� �߰��ȴ�)

		break;
	case EVENT_TYPE::DELETE_OBJECT:
		// �̹� ���� ���¿� �ִ� ������Ʈ�� ������Ʈ�� ���̴� event�� �ٽ� ������ ���
		// �׳� �Ѱ��ش�.

		if (((CGameObject*)tEvent.wParam)->IsActive() == false)
			break;

		// ���� ������ ���� ��쿡�� ���δ�.
		((CGameObject*)tEvent.wParam)->ActiveDisable();

		// ���� ������Ʈ vec�� �߰����ش�.
		m_vecDead.push_back(((CGameObject*)tEvent.wParam));

		break;

	case EVENT_TYPE::ADD_CHILD:
		((CGameObject*)tEvent.wParam)->AddChild((CGameObject*)tEvent.lParam);

		break;

	case EVENT_TYPE::CLEAR_PARENT:
		((CGameObject*)tEvent.wParam)->ClearParent();

		break;

	case EVENT_TYPE::TRANSFER_LAYER:
	{
		bool bMoveAll = LOWORD(tEvent.lParam);
		int iLayerIdx = HIWORD(tEvent.lParam);
		CSceneMgr::GetInst()->GetScene()->AddGameObject(iLayerIdx, ((CGameObject*)tEvent.wParam), bMoveAll);
	}
		break;

	case EVENT_TYPE::TRANSFER_SCENE:
		break;

	case EVENT_TYPE::ENABLE_GAMEOBJECT:
		((CGameObject*)tEvent.wParam)->Enable();
		((CGameObject*)tEvent.wParam)->m_bEnable = true;
		break;

	case EVENT_TYPE::DISABLE_GAMEOBJECT:

		// ���� ��쿡�� ó������ �ʴ´�.
		if (((CGameObject*)tEvent.wParam)->IsActive() == false)
			break;

		((CGameObject*)tEvent.wParam)->Disable();
		((CGameObject*)tEvent.wParam)->m_bEnable = false;
		break;

	case EVENT_TYPE::ENABLE_COMPONENT:

		((CComponent*)tEvent.wParam)->Enable();
		((CComponent*)tEvent.wParam)->m_bEnable = true;

		break;

	case EVENT_TYPE::DISABLE_COMPONENT:

		((CComponent*)tEvent.wParam)->Disable();
		((CComponent*)tEvent.wParam)->m_bEnable = false;
		break;
	}
}