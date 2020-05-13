// InspectorDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "InspectorDlg.h"
#include "afxdialogex.h"


// CInspectorDlg 대화 상자

IMPLEMENT_DYNAMIC(CInspectorDlg, CDialogEx)

CInspectorDlg::CInspectorDlg(CWnd* pParent /*=nullptr*/) :
	m_bSetCtrl(false)
{

}

CInspectorDlg::~CInspectorDlg()
{
}

void CInspectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CInspectorDlg::PostNcDestroy()
{
	delete this;

	CDialogEx::PostNcDestroy();
}

BOOL CInspectorDlg::PreTranslateMessage(MSG * pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
			((CMainFrame*)AfxGetMainWnd())->GetGameView()->SetFocus();

		// Ctrl Wnd에서 Enter키를 누른 경우. GameView에 Focus
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CInspectorDlg::Update()
{
	if (!m_pTarget)
		assert(nullptr);

	// 편집중이라면 처리를 하지 않는다.
	if (IsEdit())
		return;

	// Ctrl의 Update가 이루어졌다.
	if (IsUpdate())
	{
		if (GetTarget()->IsChanged())
			UpdateCtrl();
		else
			return;
	}
	// 업데이트를 한 적이 없다.
	else
	{
		UpdateCtrl(); // CtrlUpdate
		RedrawWindow();
	}
}


BEGIN_MESSAGE_MAP(CInspectorDlg, CDialogEx)
END_MESSAGE_MAP()


// CInspectorDlg 메시지 처리기
