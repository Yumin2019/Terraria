// Animator2DDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "Animator2DDlg.h"
#include "afxdialogex.h"
#include "ListDlg.h"

#include "AddAnimDlg.h"

#include <Engine/Animator2D.h>
// CAnimator2DDlg 대화 상자

IMPLEMENT_DYNAMIC(CAnimator2DDlg, CDialogEx)

CAnimator2DDlg::CAnimator2DDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(COMPONENT_TYPE::ANIMATOR2D),
	m_pAddAnim(nullptr),
	m_bShow(false)
{

}

CAnimator2DDlg::~CAnimator2DDlg()
{
	SAFE_DELETE(m_pAddAnim);
}

#include <Engine/Animation2D.h>
#include <Engine/Animator2D.h>
#include <Engine/GameObject.h>

void CAnimator2DDlg::UpdateCtrl()
{
	m_EditCombo.ResetContent();
	m_vecAnim.clear();

	CAnimator2D* pAnimator = GetTarget()->GetAnimator2D();
	wstring strCurAnim = pAnimator->GetCurAnim();
	// 콤보박스에 추가된 애니메이션의 목록을 추가.
	const map<wstring, CAnimation2D*>& mapAnim = pAnimator->GetAnimations();

	map<wstring, CAnimation2D*>::const_iterator iter;
	map<wstring, CAnimation2D*>::const_iterator iterEnd = mapAnim.end();
	int idx = 0;

	for (iter = mapAnim.begin(); iter != iterEnd; ++iter, ++idx)
	{
		m_EditCombo.AddString(iter->first.c_str());
		m_vecAnim.push_back(iter->first);

		// 현재 애니메이션을 콤보박스에서 보여준다.
		if (iter->first == strCurAnim)
		{
			m_EditCombo.SetCurSel(idx);
		}

	}

	if (!pAnimator->IsPause())
	{
		// Play모드라면, || 를 출력해준다.
		m_EditPlay.SetWindowTextW(L"||");
	}
	else
		m_EditPlay.SetWindowTextW(L"▶");

	EnableUpdate();
}

void CAnimator2DDlg::EndDialog()
{
	m_pAddAnim->ShowWindow(false);
	m_bShow = false;

	CComponentDlg::EndDialog();
}

void CAnimator2DDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_EditPlay);
	DDX_Control(pDX, IDC_BUTTON2, m_EditPause);
	DDX_Control(pDX, IDC_BUTTON3, m_EditAdd);
	DDX_Control(pDX, IDC_COMBO1, m_EditCombo);
	DDX_Control(pDX, IDC_CHECK1, m_Check);

	m_Check.SetCheck(1);
}


BEGIN_MESSAGE_MAP(CAnimator2DDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAnimator2DDlg::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_BUTTON2, &CAnimator2DDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON3, &CAnimator2DDlg::OnBnClickedAnimationAdd)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAnimator2DDlg::OnChangedCombo)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_CHECK1, &CAnimator2DDlg::OnBnClickedCheckBox)
	ON_BN_CLICKED(IDC_BUTTON6, &CAnimator2DDlg::OnClickedDeleteAnimation)
	ON_BN_CLICKED(IDC_BUTTON7, &CAnimator2DDlg::OnBnClickedEditAnimation)
END_MESSAGE_MAP()


// CAnimator2DDlg 메시지 처리기


void CAnimator2DDlg::OnBnClickedPlay()
{
	CAnimator2D* pAnimator = GetTarget()->GetAnimator2D();
	// Play 상태 : ||를 눌렀다.
	if (!pAnimator->IsPause())
	{
		pAnimator->SetPause(true);
		m_EditPlay.SetWindowTextW(L"▶");

	}
	// 일시정지 상태 : ▶를 눌렀다.
	else
	{
		pAnimator->SetPause(false);
		m_EditPlay.SetWindowTextW(L"||");
	}

	GameViewFocus();
}


void CAnimator2DDlg::OnBnClickedStop()
{
	CAnimator2D* pAnimator = GetTarget()->GetAnimator2D();

	// ㅁ 버튼은, 애니메이션의 초기상태로 돌리고, stop하는 기능이다.
	// 그리고 반드시 화살표가 되어있다.
	m_EditPlay.SetWindowTextW(L"▶");
	pAnimator->StopAnimation();
	GameViewFocus();

}


void CAnimator2DDlg::OnBnClickedAnimationAdd()
{
	// AnimDlg를 보여주자.
	m_pAddAnim->ShowWindow(true);
	m_bShow = true;
	m_pAddAnim->Init(GetTarget()->GetAnimator2D(), GetTarget()->GetName());
}


void CAnimator2DDlg::OnChangedCombo()
{
	// 선택한 애니메이션의 이름을 가져온다.
	int idx = m_EditCombo.GetCurSel();

	GetTarget()->GetAnimator2D()->PlayAnimation(m_vecAnim[idx]);

	GameViewFocus();
}


int CAnimator2DDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComponentDlg::OnCreate(lpCreateStruct) == -1)
		return -1;

	// MeshRenderDlg에 소속되어 있는 MtrlDataDlg 처럼 처리를 하도록 한다.
	// AddAnimDlg를 생성

	m_pAddAnim = new CAddAnimDlg;
	m_pAddAnim->Create(IDD_ADDANIM);
	m_pAddAnim->ShowWindow(false);
	m_pAddAnim->SetDlg(this);

	return 0;
}


void CAnimator2DDlg::OnBnClickedCheckBox()
{
	Event tEvent = {};

	if (m_Check.GetCheck())
		tEvent.eType = EVENT_TYPE::ENABLE_COMPONENT;
	else
		tEvent.eType = EVENT_TYPE::DISABLE_COMPONENT;

	tEvent.wParam = (DWORD_PTR)GetTarget()->GetComponent(GetType());
	CToolEventMgr::GetInst()->AddEvent(tEvent);

	((CMainFrame*)AfxGetMainWnd())->GetGameView()->SetFocus();

	// EventMgr-> ToolEvent로 전환.
}


void CAnimator2DDlg::OnClickedDeleteAnimation()
{
	vector<CString> vecString;
	map<wstring, CAnimation2D*>& mapAnim = GetTarget()->GetAnimator2D()->GetAnimations();
	// 애니메이션의 이름을 모두 받아온다.

	for (const auto& pair : mapAnim)
	{
		vecString.push_back(pair.first.c_str());
	}

	CListDlg dlg(L"Delete Anim", vecString);

	int id = dlg.DoModal(); // 루프가 시작되어, 다른 View에 대한 처리를 하지 않게 된다.

	if (id == IDCANCEL)
		return;

	// 선택한 인덱스를 삭제한다.
	wstring strSelect = dlg.GetSelectString();

	if (GetTarget()->GetAnimator2D()->GetCurAnim() == strSelect)
	{
		// 현재 애니메이션이 삭제하려고 하는 애니메이션이다.
		// 처리를 막는다.

		MessageBox(nullptr, L"CurAnim", MB_OK);
		return;
	}

	mapAnim.erase(strSelect);

	UpdateCtrl();

	((CMainFrame*)AfxGetMainWnd())->GetGameView()->SetFocus();
}


void CAnimator2DDlg::OnBnClickedEditAnimation()
{

	vector<CString> vecString;
	map<wstring, CAnimation2D*>& mapAnim = GetTarget()->GetAnimator2D()->GetAnimations();
	// 애니메이션의 이름을 모두 받아온다.

	for (const auto& pair : mapAnim)
	{
		vecString.push_back(pair.first.c_str());
	}

	CListDlg dlg(L"Edit Anim", vecString);

	int id = dlg.DoModal();

	if (id == IDCANCEL)
		return;

	wstring strSelect = dlg.GetSelectString();

	if (GetTarget()->GetAnimator2D()->GetCurAnim() == strSelect)
	{
		// 현재 애니메이션이 삭제하려고 하는 애니메이션이다.
		// 처리를 막는다.

		MessageBox(nullptr, L"CurAnim", MB_OK);
		return;
	}

	// 선택한 애니메이션의 이름을 토대로 포인터를 알아낸다.
	CAnimation2D* pAnim = GetTarget()->GetAnimator2D()->FindAnimation(strSelect);

	if (pAnim == nullptr)
	{
		assert(false);
		return;
	}
	// 애니메이션의 정보를 토대로 생성하고 편집.
	// AddAnimDlg를 보이도록 한다.

	m_pAddAnim->ShowWindow(true);
	m_bShow = true;
	m_pAddAnim->InitEdit(GetTarget()->GetAnimator2D(), pAnim); // 편집작업을 위한 Dlg Open.

	((CMainFrame*)AfxGetMainWnd())->GetGameView()->SetFocus();
}
