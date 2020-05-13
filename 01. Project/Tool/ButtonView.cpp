// ButtonView.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "ButtonView.h"
#include "ButtonDlg.h"


// CButtonView

IMPLEMENT_DYNCREATE(CButtonView, CView)

CButtonView::CButtonView() :
	m_pBtnDlg(nullptr)
{

}

CButtonView::~CButtonView()
{
}

bool CButtonView::Init()
{
	m_pBtnDlg->Init();

	return true;
}

void CButtonView::Update()
{
}

BEGIN_MESSAGE_MAP(CButtonView, CView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CButtonView 그리기

void CButtonView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CButtonView 진단

#ifdef _DEBUG
void CButtonView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CButtonView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CButtonView 메시지 처리기


int CButtonView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rcView;
	GetClientRect(&rcView);

	m_pBtnDlg = new CButtonDlg;
	m_pBtnDlg->Create(IDD_BUTTONDLG, this);
	m_pBtnDlg->SetWindowPos(nullptr, 0, 0, BTNVIEW_X, BTNVIEW_Y, 0);
	m_pBtnDlg->ShowWindow(TRUE);

	return 0;
}
