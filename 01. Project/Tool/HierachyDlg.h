#pragma once

#include "TreeCtrlDlg.h"

class CGameObject;
// CHierachyDlg 대화 상자

class CHierachyDlg : public CTreeCtrlDlg
{
	DECLARE_DYNAMIC(CHierachyDlg)

public:
	CHierachyDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CHierachyDlg();

private:
	vector<CGameObject*>	m_vecDead; 
	// EventMgr에서 처럼. Dead처리를 할 오브젝트를 
	// 미리 받아둔 상태에서 Game의 로직을 돌려서 Layer에서 죽은 처리가 된 오브젝트를 제거하고,
	// 그 이후에 delete를 하기 위해서 가지고 있는다.


public:
	virtual bool Init();
	virtual void Update();
	virtual void Renew();
	void SelectObject(CGameObject* pObject); // 아이템을 선택.

private:
	void AddGameObject(CGameObject* pGameObj, HTREEITEM hParent);
	// 게임오브젝트를 넣어주고, Parent 를 넣어주면 TreeCtrl을 이용해서 계층 관계를 재귀적으로 생성한다.

	virtual void DragDrop(HTREEITEM hSrcItem, HTREEITEM hDestItem);
	// 드래그 앤 드롭 이후에 후 처리를 위한 가상함수이다.
	
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HIERACHY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSelChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEndLabel(NMHDR *pNMHDR, LRESULT *pResult);


};
