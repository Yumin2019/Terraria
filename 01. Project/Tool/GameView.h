#pragma once


// CGameView 보기

enum DRAG_TYPE
{
	DT_BEGIN,
	DT_DRAG,
	DT_END
};

class CGameView : public CView
{
	DECLARE_DYNCREATE(CGameView)

protected:
	CGameView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CGameView();

private:
	class CGameObject*		m_pToolCam;
	DRAG_TYPE				m_eDragType;

	int m_iIdxX;
	int m_iIdxY;


	bool m_bViewInput;

public:
	bool Init();
	void Update();
	void SetViewInput(bool bInput) { m_bViewInput = bInput; }

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
	virtual BOOL PreCreatWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};


