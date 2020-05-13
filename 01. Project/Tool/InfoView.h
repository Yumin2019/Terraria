#pragma once


class CGameObject;
class CComponentDlg;
class CInspectorDlg;
class CScriptDlg;
// CInfoView 보기

class CInfoView : public CView
{
	DECLARE_DYNCREATE(CInfoView)

protected:
	CInfoView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CInfoView();

	CComponentDlg* m_pDlgArr[(UINT)COMPONENT_TYPE::END];
	CScriptDlg*		m_pScript;
	CInspectorDlg* m_pInspector;
	CGameObject* m_pTarget;

public:
	CComponentDlg* GetComDlg(COMPONENT_TYPE eType) const { return m_pDlgArr[(UINT)eType]; }
	CInspectorDlg* GetInspectorDlg() const { return m_pInspector; }

public:
	void SetTarget(CGameObject* pTarget);

public:
	bool Init();
	void Update();

	void AddCamera();
	void AddAnimator2D();
	void AddCollider2D();
	void AddMeshRender();
	void AddLight2D();
	void AddTileMap2D();

	void DeleteCamera();
	void DeleteAnimator2D();
	void DeleteCollider2D();
	void DeleteMeshRender();
	void DeleteLight2D();
	void DeleteTileMap2D();
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


