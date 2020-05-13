#include "stdafx.h"
#include "Script.h"

int CScript::g_iReverse = 0;

CScript::CScript(UINT iScriptType) :
	CComponent(COMPONENT_TYPE::SCRIPT),
	m_iScriptType(iScriptType)

{
}

CScript::~CScript()
{
}

void CScript::CreateObject(CGameObject * pObj, int iLayerIdx)
{
	Event tEvent = {};
	tEvent.eType = EVENT_TYPE::CREATE_OBJECT;
	tEvent.lParam = (DWORD_PTR)iLayerIdx;
	tEvent.wParam = (DWORD_PTR)pObj;

	CEventMgr::GetInst()->AddEvent(tEvent);
}

void CScript::CreateObject(CGameObject * pObj, const wstring & strLayerName)
{
	CLayer* pLayer = CSceneMgr::GetInst()->GetScene()->FindLayer(strLayerName);

	if (pLayer == nullptr)
		assert(false);

	// 없는 레이어에 추가를 하는 경우 assert

	Event tEvent = {};
	tEvent.eType = EVENT_TYPE::CREATE_OBJECT;
	tEvent.lParam = (DWORD_PTR)pLayer->GetLayerIdx();
	tEvent.wParam = (DWORD_PTR)pObj;

	CEventMgr::GetInst()->AddEvent(tEvent);
}

void CScript::DeleteObject(CGameObject * pObj)
{
	Event tEvent = {};
	tEvent.eType = EVENT_TYPE::DELETE_OBJECT;
	tEvent.wParam = (DWORD_PTR)pObj;

	CEventMgr::GetInst()->AddEvent(tEvent);

	// nullptr이 들어오는 경우도 문제가 되지는 않는다. safe_delete
}

void CScript::AddChild(CGameObject * pChild)
{
	Event tEvent = {};
	tEvent.eType = EVENT_TYPE::ADD_CHILD;
	tEvent.wParam = (DWORD_PTR)GetObj();
	tEvent.lParam = (DWORD_PTR)pChild;

	CEventMgr::GetInst()->AddEvent(tEvent);
}

void CScript::AddChild(CGameObject * pParent, CGameObject * pChild)
{
	Event tEvent = {};
	tEvent.eType = EVENT_TYPE::ADD_CHILD;
	tEvent.wParam = (DWORD_PTR)pParent;
	tEvent.lParam = (DWORD_PTR)pChild;

	CEventMgr::GetInst()->AddEvent(tEvent);
}

void CScript::ClearParent()
{
	Event tEvent = {};
	tEvent.eType = EVENT_TYPE::CLEAR_PARENT;
	tEvent.wParam = (DWORD_PTR)GetObj();

	CEventMgr::GetInst()->AddEvent(tEvent);
}

void CScript::ClearParent(CGameObject * pTarget)
{
	Event tEvent = {};
	tEvent.eType = EVENT_TYPE::CLEAR_PARENT;
	tEvent.wParam = (DWORD_PTR)pTarget;

	CEventMgr::GetInst()->AddEvent(tEvent);
}

void CScript::TransferLayer(const wstring & strLayerName, bool bMoveAll)
{
	// 해당 이름의 Layer를 찾지 못한 경우 assert.
	CLayer* pLayer = CSceneMgr::GetInst()->GetScene()->FindLayer(strLayerName);

	if (pLayer == nullptr)
		assert(false);

	// 현재 Layer와 바꾸려하고 하는 Layer가 같은 경우 return.
	if (GetObj()->GetLayerIdx() == pLayer->GetLayerIdx())
		return;

	// Layer가 있으면서, 현재 Layer와는 다른 경우.
	Event tEvent = {};
	tEvent.eType = EVENT_TYPE::TRANSFER_LAYER;
	tEvent.wParam = (DWORD_PTR)GetObj();
	tEvent.lParam = ((DWORD_PTR)pLayer->GetLayerIdx() << 16 | bMoveAll);
	// LayerIdx는 int형으로 4byte지만, 2Byte (Word)의 크기로 계산하기 위해서,
	// 16비트를 밀어서 그곳에 LayerIdx를 저장해주고, MoveAll (1Byte 변수)와 Or연산을 하여,
	// 내부적으로 1byte를 사용하는 것 같기는 한데.

	// 아무튼, 2byte만큼은 MoveAll의 데이터를 ... 그 위로 4byte만큼을 LayerIdx로 저장하여 사용한다.

	CEventMgr::GetInst()->AddEvent(tEvent);
}

void CScript::TransferLayer(int iLayerIdx, bool bMoveAll)
{
	// 인덱스의 범위를 넘어간 경우 assert
	if (iLayerIdx < 0 || iLayerIdx >= MAX_LAYER)
		assert(false);

	if (GetObj()->GetLayerIdx() == iLayerIdx)
		return;

	
	Event tEvent = {};
	tEvent.eType = EVENT_TYPE::TRANSFER_LAYER;
	tEvent.wParam = (DWORD_PTR)GetObj();
	tEvent.lParam = ((DWORD_PTR)iLayerIdx << 16 | bMoveAll);

	CEventMgr::GetInst()->AddEvent(tEvent);
}

void CScript::TransferLayer(CGameObject * pTarget, const wstring & strLayerName, bool bMoveAll)
{
	CLayer* pLayer = CSceneMgr::GetInst()->GetScene()->FindLayer(strLayerName);

	if (pLayer == nullptr)
		assert(false);

	if (pTarget->GetLayerIdx() == pLayer->GetLayerIdx())
		return;

	Event tEvent = {};
	tEvent.eType = EVENT_TYPE::TRANSFER_LAYER;
	tEvent.wParam = (DWORD_PTR)pTarget;
	tEvent.lParam = ((DWORD_PTR)pLayer->GetLayerIdx() << 16 | bMoveAll);

	CEventMgr::GetInst()->AddEvent(tEvent);
}

void CScript::TransferLayer(CGameObject * pTarget, int iLayerIdx, bool bMoveAll)
{
	if (iLayerIdx < 0 || iLayerIdx >= MAX_LAYER)
		assert(false);

	if (pTarget->GetLayerIdx() == iLayerIdx)
		return;


	Event tEvent = {};
	tEvent.eType = EVENT_TYPE::TRANSFER_LAYER;
	tEvent.wParam = (DWORD_PTR)pTarget;
	tEvent.lParam = ((DWORD_PTR)iLayerIdx << 16 | bMoveAll);

	CEventMgr::GetInst()->AddEvent(tEvent);
}

void CScript::EnableObj(CGameObject * pObj)
{
	Event tEvent;
	tEvent.eType = EVENT_TYPE::DISABLE_GAMEOBJECT;
	tEvent.wParam = (DWORD_PTR)pObj;

	CEventMgr::GetInst()->AddEvent(tEvent);
}

void CScript::DisableObj(CGameObject * pObj)
{
	Event tEvent;
	tEvent.eType = EVENT_TYPE::DISABLE_GAMEOBJECT;
	tEvent.wParam = (DWORD_PTR)pObj;

	CEventMgr::GetInst()->AddEvent(tEvent);
}

void CScript::EnableComponent(CComponent * pCom)
{
	Event tEvent;
	tEvent.eType = EVENT_TYPE::ENABLE_COMPONENT;
	tEvent.wParam = (DWORD_PTR)pCom;

	CEventMgr::GetInst()->AddEvent(tEvent);
}

void CScript::DisableComponent(CComponent * pCom)
{
	Event tEvent;
	tEvent.eType = EVENT_TYPE::DISABLE_COMPONENT;
	tEvent.wParam = (DWORD_PTR)pCom;

	CEventMgr::GetInst()->AddEvent(tEvent);
}

