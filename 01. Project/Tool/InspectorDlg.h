#pragma once


#include <Engine/GameObject.h>
#include <Engine/EventMgr.h>

#include "MainFrame.h" 

class CInspectorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInspectorDlg)

public:
	CInspectorDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CInspectorDlg();

private:
	CGameObject*		m_pTarget; 
	bool				m_bUpdate;
	bool				m_bEdit;
	bool				m_bSetCtrl; // UpdateCtrl함수 호출 중에 EditPresent함수가 호출되는 상황을 막기 위한 flag

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSPECTOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	void SetCtrl(bool bCtrl) { m_bSetCtrl = bCtrl; }
	bool IsSetCtrl() const { return m_bSetCtrl; }

public:
	void SetTarget(CGameObject* pTarget)
	{
		if (m_pTarget != pTarget)
		{
			// 새로운 타겟이 지정이 된 경우.
			m_pTarget = pTarget;
			m_bUpdate = false;
		}
	}

	void EnableUpdate() { m_bUpdate = true; }
	void SetEdit(bool bEdit) { m_bEdit = bEdit; }
	virtual void PostNcDestroy();

public:
	CGameObject* GetTarget() const { return m_pTarget; }
	bool IsUpdate() const { return m_bUpdate; }
	bool IsEdit() const { return m_bEdit; }

public:
	void OnOK() {}
	void OnCancel() {}
	afx_msg void OnSetFocus(UINT id) { m_bEdit = true; }
	afx_msg void OnKillFocus(UINT id) { m_bEdit = false; }
	afx_msg BOOL PreTranslateMessage(MSG* pMsg); // EnterKey GameView Focus

	virtual void Update();
	virtual void UpdateCtrl() = 0; // 각 다이얼로그마다, Ctrl을 통한 처리는 다르다.
	virtual bool Init() { return true; }

	afx_msg virtual void EditPresent(UINT id) {} // 재정의를 통해서 각 Dlg에서 표현한 것을 Target에 표현.
};
