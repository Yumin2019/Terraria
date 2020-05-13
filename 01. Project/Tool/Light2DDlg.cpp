// Light2DDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "Light2DDlg.h"
#include "afxdialogex.h"

#include <Engine/Light2D.h>


// CLight2DDlg 대화 상자

IMPLEMENT_DYNAMIC(CLight2DDlg, CDialogEx)

CLight2DDlg::CLight2DDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(COMPONENT_TYPE::LIGHT2D)
{

}

CLight2DDlg::~CLight2DDlg()
{
}


void CLight2DDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditR);
	DDX_Control(pDX, IDC_EDIT2, m_EditG);
	DDX_Control(pDX, IDC_EDIT3, m_EditB);
	DDX_Control(pDX, IDC_EDIT4, m_EditRange);
	DDX_Control(pDX, IDC_CHECK1, m_EditCheck);

	m_EditCheck.SetCheck(1);
}


BEGIN_MESSAGE_MAP(CLight2DDlg, CDialogEx)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT1, IDC_EDIT4, &CLight2DDlg::EditPresent)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT1, IDC_EDIT4, &CComponentDlg::OnKillFocus)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT1, IDC_EDIT4, &CComponentDlg::OnSetFocus)
	ON_BN_CLICKED(IDC_CHECK1, &CLight2DDlg::OnCheckBox)
END_MESSAGE_MAP()


void CLight2DDlg::UpdateCtrl()
{
	SetCtrl(true);

	CString strSet;

	// r g b range
	CLight2D* pLight = GetTarget()->GetLight2D();

	strSet.Format(L"%.2f", pLight->GetRange());
	m_EditRange.SetWindowTextW(strSet);

	strSet.Format(L"%d", pLight->GetR());
	m_EditR.SetWindowTextW(strSet);

	strSet.Format(L"%d", pLight->GetG());
	m_EditG.SetWindowTextW(strSet);

	strSet.Format(L"%d", pLight->GetB());
	m_EditB.SetWindowTextW(strSet);

	EnableUpdate();

	SetCtrl(false);

}

void CLight2DDlg::EditPresent(UINT id)
{
	if (IsSetCtrl())
		return;

	CLight2D* pLight = GetTarget()->GetLight2D();

	CString strGet;

	m_EditRange.GetWindowTextW(strGet);

	float fRange = _wtof(strGet);

	pLight->SetLightRange(fRange);


	m_EditR.GetWindowTextW(strGet);
	float r = _wtoi(strGet) / 255.f;

	m_EditG.GetWindowTextW(strGet);
	float g = _wtoi(strGet) / 255.f;

	m_EditB.GetWindowTextW(strGet);
	float b = _wtoi(strGet) / 255.f;


	pLight->SetLightColor(r, g, b);
}

void CLight2DDlg::OnCheckBox()
{
	Event tEvent = {};

	if (m_EditCheck.GetCheck())
		tEvent.eType = EVENT_TYPE::ENABLE_COMPONENT;
	else
		tEvent.eType = EVENT_TYPE::DISABLE_COMPONENT;

	tEvent.wParam = (DWORD_PTR)GetTarget()->GetComponent(GetType());
	CToolEventMgr::GetInst()->AddEvent(tEvent);

	((CMainFrame*)AfxGetMainWnd())->GetGameView()->SetFocus();
}

// CLight2DDlg 메시지 처리기
