// LayerDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "LayerDlg.h"
#include "afxdialogex.h"
#include "ListDlg.h"
#include "ToolEventMgr.h"
#include "InfoView.h"

#include <string>

#include <Engine/SceneMgr.h>
#include <Engine/Scene.h>
#include <Engine/Layer.h>
#include <Engine/Collider2D.h>


// CLayerDlg 대화 상자

IMPLEMENT_DYNAMIC(CLayerDlg, CDialogEx)

CLayerDlg::CLayerDlg(CWnd* pParent /*=nullptr*/) :
	m_bMoveAll(false)
{

}

CLayerDlg::~CLayerDlg()
{
}

void CLayerDlg::UpdateCtrl()
{
	SetCtrl(true);
	// 체크박스 옆 이름 셋팅 
	m_EditCheck.SetWindowTextW(GetTarget()->GetName().c_str());

	// Object의 소속 Layer출력
	int iLayerIdx = GetTarget()->GetLayerIdx();

	if (iLayerIdx < 0 || iLayerIdx >= MAX_LAYER)
		return;

	CLayer* pLayer = CSceneMgr::GetInst()->GetScene()->GetLayer(iLayerIdx);

	wstring strLayer = std::to_wstring(iLayerIdx) + L" " + pLayer->GetName();

	m_EditText.SetWindowTextW(strLayer.c_str());

	EnableUpdate();
	SetCtrl(false);

}

void CLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_EditText);
	DDX_Control(pDX, IDC_BUTTON1, m_EditButton);
	DDX_Control(pDX, IDC_CHECK1, m_EditCheck);

	m_EditCheck.SetCheck(1);
}


BEGIN_MESSAGE_MAP(CLayerDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK1, &CLayerDlg::OnCheckBox)
	ON_BN_CLICKED(IDC_BUTTON1, &CLayerDlg::OnBnClickedLayerBtn)
	ON_BN_CLICKED(IDC_CHECK2, &CLayerDlg::OnBnClickedMove)
	ON_BN_CLICKED(IDC_BUTTON2, &CLayerDlg::OnClickedCloneBtn)
END_MESSAGE_MAP()


// CLayerDlg 메시지 처리기


void CLayerDlg::OnCheckBox()
{
	Event tEvent = {};

	if (m_EditCheck.GetCheck())
		tEvent.eType = EVENT_TYPE::ENABLE_GAMEOBJECT;
	else
		tEvent.eType = EVENT_TYPE::DISABLE_GAMEOBJECT;

	tEvent.wParam = (DWORD_PTR)GetTarget();
	CToolEventMgr::GetInst()->AddEvent(tEvent);

	((CMainFrame*)AfxGetMainWnd())->GetGameView()->SetFocus();
}


void CLayerDlg::OnBnClickedLayerBtn()
{
	CLayer** pLayerArr = CSceneMgr::GetInst()->GetScene()->GetLayerArr();

	vector<CString> vecString;
	wstring strLayer;

	for (size_t i = 0; i < MAX_LAYER; ++i)
	{
		strLayer = std::to_wstring(i) + L" " + pLayerArr[i]->GetName();
		vecString.push_back(strLayer.c_str());
	}

	CListDlg dlg(L"Layer", vecString);

	int id = dlg.DoModal(); // 루프가 시작되어, 다른 View에 대한 처리를 하지 않게 된다.

	if (id == IDCANCEL)
		return;

	CString strSelect = dlg.GetSelectString();

	// 선택된 Layer로 옮겨주자.

	int iLayerIdx = _wtoi(strSelect.Left(2));

	// 현재 Layer와 옮기려는 Layer가 같은 경우에는 처리하지 않는다.
	if (iLayerIdx == GetTarget()->GetLayerIdx())
		return;
	// 다른 누군가와 충돌인 경우 처리하지 않는다.
	else if (GetTarget()->GetCollider2D() && GetTarget()->GetCollider2D()->GetCollisionCount())
	{
		MessageBox(L"Object Collision", nullptr, MB_OK);
		return;
	}

	Event tEvent = {};
	tEvent.eType = EVENT_TYPE::TRANSFER_LAYER;
	tEvent.wParam = (DWORD_PTR)GetTarget();
	tEvent.lParam = ((DWORD_PTR)iLayerIdx << 16 | m_bMoveAll);

	CToolEventMgr::GetInst()->AddEvent(tEvent);

	// Ctrl을 해당 Layer의 이름으로 지정해주자.
	m_EditText.SetWindowTextW(strSelect);

	((CMainFrame*)AfxGetMainWnd())->GetGameView()->SetFocus();

}



void CLayerDlg::OnBnClickedMove()
{
	m_bMoveAll = !m_bMoveAll;
}


void CLayerDlg::OnClickedCloneBtn()
{
	// TargetObj를 Clone하자.
	CGameObject* pClone = GetTarget()->Clone();
	pClone->SetName(pClone->GetName() + L"Clone");

	if (GetTarget()->GetLayerIdx() == -1)
		assert(false);

	CScene* pScene = CSceneMgr::GetInst()->GetScene();
	CLayer* pLayer = pScene->GetLayer(GetTarget()->GetLayerIdx());

	// 생성된 오브젝트는 원본과 동알한 Layer에 넣어주자
	pLayer->AddGameObject(pClone);

	// 하이러라키 업데이트
	MAINFRAME->RenewHierachy();

	// 새로 생성된 오브젝트를 Target해주자.
	((CInfoView*)MAINFRAME->GetInfoView())->SetTarget(pClone);
}
