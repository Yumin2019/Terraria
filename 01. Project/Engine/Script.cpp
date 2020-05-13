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

	// ���� ���̾ �߰��� �ϴ� ��� assert

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

	// nullptr�� ������ ��쵵 ������ ������ �ʴ´�. safe_delete
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
	// �ش� �̸��� Layer�� ã�� ���� ��� assert.
	CLayer* pLayer = CSceneMgr::GetInst()->GetScene()->FindLayer(strLayerName);

	if (pLayer == nullptr)
		assert(false);

	// ���� Layer�� �ٲٷ��ϰ� �ϴ� Layer�� ���� ��� return.
	if (GetObj()->GetLayerIdx() == pLayer->GetLayerIdx())
		return;

	// Layer�� �����鼭, ���� Layer�ʹ� �ٸ� ���.
	Event tEvent = {};
	tEvent.eType = EVENT_TYPE::TRANSFER_LAYER;
	tEvent.wParam = (DWORD_PTR)GetObj();
	tEvent.lParam = ((DWORD_PTR)pLayer->GetLayerIdx() << 16 | bMoveAll);
	// LayerIdx�� int������ 4byte����, 2Byte (Word)�� ũ��� ����ϱ� ���ؼ�,
	// 16��Ʈ�� �о �װ��� LayerIdx�� �������ְ�, MoveAll (1Byte ����)�� Or������ �Ͽ�,
	// ���������� 1byte�� ����ϴ� �� ����� �ѵ�.

	// �ƹ�ư, 2byte��ŭ�� MoveAll�� �����͸� ... �� ���� 4byte��ŭ�� LayerIdx�� �����Ͽ� ����Ѵ�.

	CEventMgr::GetInst()->AddEvent(tEvent);
}

void CScript::TransferLayer(int iLayerIdx, bool bMoveAll)
{
	// �ε����� ������ �Ѿ ��� assert
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

