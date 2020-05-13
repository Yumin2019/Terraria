#pragma once

#include <Engine/GameObject.h>
#include "ToolEventMgr.h"

#include "MainFrame.h" 

class CComponentDlg :
	public CDialogEx
{
public:
	CComponentDlg(COMPONENT_TYPE eType);
	virtual ~CComponentDlg();

private:
	CGameObject*		m_pTarget; // 모든 Component Dlg는 자신이 가리키는 타겟이 존재하며 이 정보를 출력함.
	COMPONENT_TYPE		m_eComType; // 어떤 dlg의 관한 정보인지..
	bool				m_bUpdate; // 컨트롤 업데이트 여부
	bool				m_bEdit;   // Editing Or Not

	bool				m_bSetCtrl; // UpdateCtrl함수 호출 중에 EditPresent함수가 호출되는 상황을 막기 위한 flag

public:
	virtual void Start() {}
	void SetCtrl(bool bCtrl) { m_bSetCtrl = bCtrl; }
	bool IsSetCtrl() const { return m_bSetCtrl; }

	void SetTarget(CGameObject* pTarget)
	{
		if (m_pTarget != pTarget)
		{
			// 새로운 타겟이 지정이 된 경우.
			m_pTarget = pTarget;
			m_bUpdate = false;
			EndDialog();
		}
	}

	virtual void EndDialog() { ShowWindow(false); }
	void EnableUpdate() { m_bUpdate = true; }
	void SetEdit(bool bEdit) { m_bEdit = bEdit; }
	virtual void PostNcDestroy();

public:
	void GameViewFocus()
	{
		((CMainFrame*)AfxGetMainWnd())->GetGameView()->SetFocus();
	}
	
public: 
	CGameObject* GetTarget() const { return m_pTarget; }
	bool IsUpdate() const { return m_bUpdate; }
	bool IsEdit() const { return m_bEdit; }
	COMPONENT_TYPE GetType() const { return m_eComType; }

public:
	void OnOK() {}
	void OnCancel() {}
	afx_msg void OnSetFocus(UINT id) { m_bEdit = true; }
	afx_msg void OnKillFocus(UINT id) { m_bEdit = false; }
	afx_msg BOOL PreTranslateMessage(MSG* pMsg); // EnterKey GameView Focus

	virtual void Update(); // 각 Component의 Update처리이다.
	virtual void UpdateCtrl() = 0; // 각 다이얼로그마다, Ctrl을 통한 처리는 다르다.
	virtual bool Init() { return true; }
	
	afx_msg virtual void EditPresent(UINT id) {} // 재정의를 통해서 각 Dlg에서 표현한 것을 Target에 표현.

};

