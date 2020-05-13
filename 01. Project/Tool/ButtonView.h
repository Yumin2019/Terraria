#pragma once

class CButtonDlg;
// CButtonView 보기

class CButtonView : public CView
{
	DECLARE_DYNCREATE(CButtonView)

protected:
	CButtonView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CButtonView();

private:
	CButtonDlg*	m_pBtnDlg;

public:
	CButtonDlg* GetButtonDlg() const { return m_pBtnDlg; }

public:
	bool Init();
	void Update();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


