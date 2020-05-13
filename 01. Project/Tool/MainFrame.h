
// MainFrame.h: CMainFrame 클래스의 인터페이스
//

#pragma once
#include "MySplitterWnd.h"

#define MAINFRAME ((CMainFrame*)AfxGetMainWnd())
#define GAMEVIEW MAINFRAME->GetGameView()

class CMainFrame : public CFrameWnd
{
public:
	CMainFrame() noexcept;
protected:
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMySplitterWnd		m_mainSplitter; // ctrl RR
	CMySplitterWnd		m_subSplitter;
	
	bool				m_bCreate = false;

public:
	bool Init();
	void Update();

	CWnd* GetButtonView() const { return m_subSplitter.GetPane(0, 0); }
	CWnd* GetGameView() const { return m_subSplitter.GetPane(1, 0); }
	CWnd* GetDebugView() const { return m_subSplitter.GetPane(2, 0); }

	CWnd* GetHierachyView() const { return m_mainSplitter.GetPane(0, 1); }
	CWnd* GetInfoView() const { return m_mainSplitter.GetPane(0, 2); }

	// 재정의입니다.

	void PostLoadScene();
	void RenewHierachy();
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void CreateEmptyObject();
	afx_msg void AddAnimator2D();
	afx_msg void AddCamera();
	afx_msg void AddCollider2D();
	afx_msg void AddLight2D();
	afx_msg void AddMeshRender();
	afx_msg void DeleteAnimator2D();
	afx_msg void DeleteCamera();
	afx_msg void DeleteCollider2D();
	afx_msg void DeleteLight2D();
	afx_msg void DeleteMeshRender();
	afx_msg void OnClickedSaveScene();
	afx_msg void OnClickedSceneLoad();
	afx_msg void AddTileMap2D();
	afx_msg void DeleteTileMap2D();
	afx_msg void OnClickedClonePrototypeObj();
	afx_msg void OnClickedAddPrototype();
	afx_msg void OnClickedDeletePrototype();
};


