#include "stdafx.h"
#include "ToolEventMgr.h"
#include "InfoView.h"
#include "MainFrame.h"

#include <Engine/GameObject.h>
#include <Engine/Component.h>
#include <Engine/Scene.h>
#include <Engine/SceneMgr.h>

CToolEventMgr::CToolEventMgr()
{
}


CToolEventMgr::~CToolEventMgr()
{
}

void CToolEventMgr::Update()
{
	// 삭제 예정인 GameObject의 메모리를 해제한다.

	for (size_t i = 0; i < m_vecDead.size(); ++i)
	{
		// 객체가 죽기 전에, 계층 관계에서 제외시킨다.
		m_vecDead[i]->ClearParent();
		SAFE_DELETE(m_vecDead[i]);
	}

	m_vecDead.clear();

	// Event를 처리한다.
	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		Execute(m_vecEvent[i]);
	}
	
	m_vecEvent.clear();
}

void CToolEventMgr::Execute(const Event & tEvent)
{
	switch (tEvent.eType)
	{
	case EVENT_TYPE::CREATE_OBJECT:
		CSceneMgr::GetInst()->GetScene()->
			AddGameObject((int)tEvent.lParam, (CGameObject*)tEvent.wParam, true);
		// 넘어온 Event 구조체의 lPram을 int형으로 casting하여, 레이어의 번호로 넘겨주고 (그렇게 데이터를 주니까)
		// wParam에는 CGameObject*를 저장해두었으니, (int형에다가 저장) 이것을 다시 넣어주는 과정에서 형변환을
		// 해준다.

		// 넣어준 GameObject를 해당 레이어에 추가해준다. (다음 장면 부터 추가된다)

		break;
	case EVENT_TYPE::DELETE_OBJECT:
		// 이미 죽은 상태에 있는 오브젝트가 오브젝트를 죽이는 event로 다시 들어오는 경우
		// 그냥 넘겨준다.

		if (((CGameObject*)tEvent.wParam)->IsActive() == false)
			break;

		// 아직 죽이지 않은 경우에는 죽인다.
		((CGameObject*)tEvent.wParam)->ActiveDisable();

		// 죽을 오브젝트 vec에 추가해준다.
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
		((CGameObject*)tEvent.wParam)->SetEnable(true);
		break;

	case EVENT_TYPE::DISABLE_GAMEOBJECT:

		// 죽은 경우에만 처리하지 않는다.
		if (((CGameObject*)tEvent.wParam)->IsActive() == false)
			break;

		((CGameObject*)tEvent.wParam)->Disable();
		((CGameObject*)tEvent.wParam)->SetEnable(false);
		break;

	case EVENT_TYPE::ENABLE_COMPONENT:

		((CComponent*)tEvent.wParam)->Enable();
		((CComponent*)tEvent.wParam)->SetEnable(true);

		break;

	case EVENT_TYPE::DISABLE_COMPONENT:

		((CComponent*)tEvent.wParam)->Disable();
		((CComponent*)tEvent.wParam)->SetEnable(false);
		break;
	}
}
