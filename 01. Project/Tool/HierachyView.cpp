// HierachyView.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "HierachyView.h"


#include "HierachyDlg.h"
#include "ResListDlg.h"

// CHierachyView

IMPLEMENT_DYNCREATE(CHierachyView, CView)

CHierachyView::CHierachyView() :
	m_DlgArr{}
{

}

CHierachyView::~CHierachyView()
{
}

BEGIN_MESSAGE_MAP(CHierachyView, CView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CHierachyView 그리기

bool CHierachyView::Init()
{
	CRect rcView;
	GetClientRect(&rcView);

	UINT iHeight = rcView.Height() / 2;

	m_DlgArr[0] = new CHierachyDlg;
	m_DlgArr[0]->Create(IDD_HIERACHY, this);
	m_DlgArr[0]->SetWindowPos(nullptr, 0, 0, rcView.Width(), iHeight, 0);
	m_DlgArr[0]->ShowWindow(true);

	m_DlgArr[1] = new CResListDlg;
	m_DlgArr[1]->Create(IDD_RESLIST, this);
	m_DlgArr[1]->SetWindowPos(nullptr, 0, iHeight, rcView.Width(), iHeight, 0);
	m_DlgArr[1]->ShowWindow(true);


	m_DlgArr[0]->Init();
	m_DlgArr[1]->Init();

	return true;
}

void CHierachyView::Update()
{
	m_DlgArr[0]->Update();
	m_DlgArr[1]->Update();
}

void CHierachyView::RenewHierachy()
{
	m_DlgArr[0]->Renew();
}

void CHierachyView::RenewRes()
{
	m_DlgArr[1]->Renew();
}

void CHierachyView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CHierachyView 진단

#ifdef _DEBUG
void CHierachyView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CHierachyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CHierachyView 메시지 처리기


int CHierachyView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	



	return 0;
}
