// DebugView.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "DebugView.h"


// CDebugView

IMPLEMENT_DYNCREATE(CDebugView, CView)

CDebugView::CDebugView()
{

}

CDebugView::~CDebugView()
{
}

BEGIN_MESSAGE_MAP(CDebugView, CView)
END_MESSAGE_MAP()


// CDebugView 그리기

bool CDebugView::Init()
{
	return false;
}

void CDebugView::Update()
{
}

void CDebugView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CDebugView 진단

#ifdef _DEBUG
void CDebugView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CDebugView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDebugView 메시지 처리기
