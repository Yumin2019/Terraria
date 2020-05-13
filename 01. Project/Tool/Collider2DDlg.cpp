// Collider2DDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "Collider2DDlg.h"
#include "afxdialogex.h"

#include <Engine/Collider2D.h>


// CCollider2DDlg 대화 상자

IMPLEMENT_DYNAMIC(CCollider2DDlg, CDialogEx)

CCollider2DDlg::CCollider2DDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(COMPONENT_TYPE::COLLIDER2D)
{

}

CCollider2DDlg::~CCollider2DDlg()
{
}

void CCollider2DDlg::UpdateCtrl()
{
	SetCtrl(true);

	CCollider2D* pCollider2D = GetTarget()->GetCollider2D();
	// 0 : scale 
	// 1 : offset
	Vec3 tTransform[2] = { pCollider2D->GetScale(), pCollider2D->GetOffset() };

	CString str;

	for (size_t i = 0; i < 2; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			str.Format(L"%f", tTransform[i][j]);
			m_Edit[i * 3 + j].SetWindowTextW(str);
		}
	}

	//COLLIDER2D_TYPE 순서 그대로 문자열을 추가하여 그대로 사용하면 된다.
	COLLIDER2D_TYPE eType = GetTarget()->GetCollider2D()->GetColliderType();

	m_EditCombo.SetCurSel((UINT)eType);

	SetCtrl(false);
	EnableUpdate();
}

void CCollider2DDlg::EditPresent(UINT id)
{
	if (IsSetCtrl())
		return;

	CString str;

	Vec3 tData[2] = {};

	for (size_t i = 0; i < 2; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			m_Edit[i * 3 + j].GetWindowTextW(str);
			tData[i][j] = _wtof(str);
		}
	}

	GetTarget()->GetCollider2D()->SetScale(tData[0]);
	GetTarget()->GetCollider2D()->SetOffset(tData[1]);
}

void CCollider2DDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	int id = IDC_EDIT12;
	for (size_t i = 0; i < 6; ++i)
	{
		DDX_Control(pDX, id + i, m_Edit[i]);
	}
	DDX_Control(pDX, IDC_CHECK1, m_EditDis);
	DDX_Control(pDX, IDC_COMBO1, m_EditCombo);

	m_EditDis.SetCheck(1);

	m_EditCombo.AddString(L"Rect");
	m_EditCombo.AddString(L"Circle");
	m_EditCombo.AddString(L"None");
}




BEGIN_MESSAGE_MAP(CCollider2DDlg, CDialogEx)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT12, IDC_EDIT17, &CCollider2DDlg::EditPresent)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT12, IDC_EDIT17, &CComponentDlg::OnKillFocus)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT12, IDC_EDIT17, &CComponentDlg::OnSetFocus)
	ON_BN_CLICKED(IDC_CHECK1, &CCollider2DDlg::OnBnClickedDisable)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CCollider2DDlg::OnCollTypeChanged)
END_MESSAGE_MAP()


// CCollider2DDlg 메시지 처리기


void CCollider2DDlg::OnBnClickedDisable()
{
	Event tEvent = {};

	if (m_EditDis.GetCheck())
		tEvent.eType = EVENT_TYPE::ENABLE_COMPONENT;
	else
		tEvent.eType = EVENT_TYPE::DISABLE_COMPONENT;

	tEvent.wParam = (DWORD_PTR)GetTarget()->GetComponent(GetType());
	CToolEventMgr::GetInst()->AddEvent(tEvent);

	((CMainFrame*)AfxGetMainWnd())->GetGameView()->SetFocus();
}





void CCollider2DDlg::OnCollTypeChanged()
{
	// Type을 바꿔버림.
	COLLIDER2D_TYPE eType = (COLLIDER2D_TYPE)m_EditCombo.GetCurSel();
	GetTarget()->GetCollider2D()->SetCollider2DType(eType);

	UpdateCtrl();

	GAMEVIEW->SetFocus();
}
