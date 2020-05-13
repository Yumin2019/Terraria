// HierachyDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "HierachyDlg.h"
#include "afxdialogex.h"

#include "MainFrame.h"
#include "InfoView.h"

#include <Engine/SceneMgr.h>
#include <Engine/EventMgr.h>

#include <Engine/Scene.h>
#include <Engine/Layer.h>
#include <Engine/GameObject.h>


// CHierachyDlg 대화 상자

IMPLEMENT_DYNAMIC(CHierachyDlg, CDialogEx)

CHierachyDlg::CHierachyDlg(CWnd* pParent /*=nullptr*/)
{
}

CHierachyDlg::~CHierachyDlg()
{
}

bool CHierachyDlg::Init()
{
	CRect rc;
	GetClientRect(&rc);

	m_EditTree.SetWindowPos(nullptr, 0, 0, rc.Width(), rc.Height(), 0);

	// 트리컨트롤 내용 갱신
	Renew();

	return true;
}

void CHierachyDlg::Update()
{
	/*
	삭제예정 오브젝트를 실제로 삭제한다.
	Dead처리를 해주고 부모라면 Parent에서 삭제. (댕글링 포인터가 될 포인터를 제거)
	vector를 제외하고, 이곳에서 실질적인 삭제.
	*/
	for (size_t i = 0; i < m_vecDead.size(); ++i)
	{
		SAFE_DELETE(m_vecDead[i]);
	}
	m_vecDead.clear();


	const vector<Event>& vecEvent = CEventMgr::GetInst()->GetEventVec();

	for (size_t i = 0; i < vecEvent.size(); ++i)
	{
		switch (vecEvent[i].eType)
		{
		case EVENT_TYPE::CREATE_OBJECT:
			// 지금 생성된 오브젝트.
			// 생성되자마자 자식을 갖고 싶은 경우에는 AddChild를 이용하고 있다.
			AddGameObject((CGameObject*)vecEvent[i].wParam, nullptr);
			break;
		case EVENT_TYPE::DELETE_OBJECT:
		{
			// DWORD_PTR 키 값을 통해서 handle을 받아온다.
			HTREEITEM hFind  = FindItemByData(vecEvent[i].wParam);
			if (hFind == nullptr)
				assert(false); 
			// Game-> 오브젝트 삭제 -> Tool 해당 오브젝트의 Handle 받아오는 것이 정상.
			// 받아오지 못했다는 것은 게임과 동기화가 되어있지 않다는 소리.

			DeleteItem(hFind); // 맵에서 정보를 빼고, Tree에서도 제거.
		}
			break;

		case EVENT_TYPE::ADD_CHILD:
		{
			// wParam : ParentObject lParam : ChildObject
			HTREEITEM hDest = FindItemByData(vecEvent[i].wParam);
			HTREEITEM hSrc = FindItemByData(vecEvent[i].lParam);

			// Child 쪽을 삭제한다.
			DeleteItem(hSrc);

			// Parent 쪽을 삭제한다. (갱신 하기 위해서)
			CGameObject* pDestObj = (CGameObject*)m_EditTree.GetItemData(hDest);
			HTREEITEM hDestParent = m_EditTree.GetParentItem(hDest);
			DeleteItem(hDest);

			// 목적지에 아이템을 새로 추가한다.
			AddGameObject(pDestObj, hDestParent);

			// 부분 갱신.
			
		}
			break;
		case EVENT_TYPE::CLEAR_PARENT:
			break;
		}
	}
}

void CHierachyDlg::Renew()
{
	// 계층관계 갱신을 위해서 SceneMgr에게서 ObjVec를 받아온다.

	m_EditTree.DeleteAllItems();

	CScene* pScene = CSceneMgr::GetInst()->GetScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecParent = pScene->GetLayer(i)->GetParentObjVec();

		for (UINT j = 0; j < vecParent.size(); ++j)
		{
			AddGameObject(vecParent[j], nullptr);
		}
	}

	// 이 결과로, 부모 오브젝트를 추가하며, 추가적으로 자식까지 계층관계를 형성해둔다.
}

void CHierachyDlg::SelectObject(CGameObject * pObject)
{
	HTREEITEM hItem = FindItemByData((DWORD_PTR)pObject);
	if (hItem == nullptr)
		assert(false);
	m_EditTree.SelectItem(hItem);
}

void CHierachyDlg::AddGameObject(CGameObject * pGameObj, HTREEITEM hParent)
{
	HTREEITEM hObj = AddItem(pGameObj->GetName().c_str(), hParent, (DWORD_PTR)pGameObj);

	const vector<CGameObject*>& vecChild = pGameObj->GetChildVec();
	for (UINT i = 0; i < vecChild.size(); ++i)
	{
		AddGameObject(vecChild[i], hObj);
	}

	// A의 자식 B C D가 있고, B C D는 더 이상 자식을 가지지 않았다.
	// 이런 상황에서 A를 추가한다고 해보자.
	
	// A가 만들어진다.
	// B C D는 A를 부모로 갖는다. 끝.

	// 만약 B C D가 자식을 하나씩 가져서 B1, C1, D1이라는 자식이 있었더라면 ....
	// B와 B1이 연결되고 C와 C1이 연결. D와 D1이 연결. 되어 함수가 종료된다.
}

void CHierachyDlg::DragDrop(HTREEITEM hSrcItem, HTREEITEM hDestItem)
{
	// src가 null인 경우는 이전에 예외처리가 걸려있을 것이다..... ... 

	// 이 함수는 이미 Handle사이의 연결관계가 끝나고 호출이 될 함수이다.

	CGameObject* pSrcObj = (CGameObject*)m_EditTree.GetItemData(hSrcItem);
	// src에 대한 오브젝트를 얻어온다.

	if (hDestItem == nullptr)
	{
		// DestItem이 nullptr라면, 최상위 객체가 되길 원하는 것이다.
		// 해당 레이어의 최상위오브젝트로 만든다.
		pSrcObj->ClearParent();
	}
	else
	{
		// DestItem이 정해져 있는 경우에는 해당 아이템으로부터 GameObject*를 얻어서
		// 그 Object의 자식으로 만든다.
		CGameObject* pDestObj = (CGameObject*)m_EditTree.GetItemData(hDestItem);
		pDestObj->AddChild(pSrcObj);
	}
}


void CHierachyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_EditTree);

	
}


BEGIN_MESSAGE_MAP(CHierachyDlg, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CHierachyDlg::OnSelChange)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE1, &CHierachyDlg::OnEndLabel)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE1, &CTreeCtrlDlg::OnBeginDrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CHierachyDlg 메시지 처리기


BOOL CHierachyDlg::PreTranslateMessage(MSG * pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			((CMainFrame*)AfxGetMainWnd())->GetGameView()->SetFocus();
		}
		else if (pMsg->wParam == VK_DELETE)
		{
			HTREEITEM hTarget = m_EditTree.GetSelectedItem(); // 입력을 하고 선택을 하고.
			CGameObject* pTargetObj = (CGameObject*)m_EditTree.GetItemData(hTarget);

			pTargetObj->ActiveDisable();
			pTargetObj->ClearParent(); // 죽은 상태 : ParentVec로 옮기는 처리를 하지 않는다.
			m_vecDead.push_back(pTargetObj);

			// 원래 부모인 경우라면.
			// Dead Vec에 넣어둔 상태에서 Engine을 한번 돌려서, Layer ParentVec에서 제거
			// Tool에서 삭제.

			// 자식의 경우에는 부모의 벡터에서 없애주고, 이후에 이곳에서 없앤다.

			DeleteItem(hTarget);

			// 오브젝트를 누르지 않은 상태에서 del 키를 누르면 어떤 식으로 처리가 될까?
			// HierachyDlg를 누르면 포커싱이 그곳으로 이동하면서
			// 자동으로 Sel이 선택되는 듯 하다. (포커싱이 있으면 셀을 고르게 되어 있음)
			// 따라서 하이어라키 다이얼로그에 포커싱이 있는 한 셀을 고르지 않은 상태는
			// 존재하지 않는 것으로 보인다.
		}
	}
	return CTreeCtrlDlg::PreTranslateMessage(pMsg);
}

void CHierachyDlg::OnSelChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	// pMNTreeView는 선택된 아이템의 핸들을 가지고 있다.
	CGameObject* pTargetObj = (CGameObject*)m_EditTree.GetItemData(pNMTreeView->itemNew.hItem);

	// Hierachy에서 선택한 오브젝트를 InfoView에도 적용하기 위해서 Target을 설정해둔다.
	((CInfoView*)((CMainFrame*)AfxGetMainWnd())->GetInfoView())->SetTarget(pTargetObj);

	*pResult = 0;
}

#include "InspectorDlg.h"

void CHierachyDlg::OnEndLabel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

	CEdit* pEdit = m_EditTree.GetEditControl();
	// 현재 편집하고 있는 Ctrl을 얻어온다.

	if (pEdit != nullptr)
	{
		CString str;
		pEdit->GetWindowTextW(str); // 오브젝트의 수정한 이름을 받아온다.
		m_EditTree.SetItemText(pTVDispInfo->item.hItem, str);
		// EditTree.SetItemText(handle, string)
		// pTVDispInfo 는 현재 수정중인 아이템의 핸들을 가지고 있다.
		// 퀘스트 : SetItemText함수를 호출하지 않으면 Sel의 String이 그대로 
		// 복구가 될까 ? -> 그렇다. 

		CGameObject* pObj = (CGameObject*)m_EditTree.GetItemData(pTVDispInfo->item.hItem);
		// 현재 선택한 아이템의 핸들로부터 CGameObject*를 얻어온다.

		pObj->SetName(str.GetBuffer());
		// 선택한 이름으로 바꾼다.

		// InfoView's Inspector에 대한 이름 처리.
		((CInfoView*)((CMainFrame*)AfxGetMainWnd())->GetInfoView())->GetInspectorDlg()->UpdateCtrl();
	}

	// 이름을 수정하고 Enter키를 누르면 터짐.
	// 마우스를 눌러서 살포시 포커싱을 바꾸면 터지지는 않음.

	*pResult = 0;
}
