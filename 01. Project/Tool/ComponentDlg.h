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
	CGameObject*		m_pTarget; // ��� Component Dlg�� �ڽ��� ����Ű�� Ÿ���� �����ϸ� �� ������ �����.
	COMPONENT_TYPE		m_eComType; // � dlg�� ���� ��������..
	bool				m_bUpdate; // ��Ʈ�� ������Ʈ ����
	bool				m_bEdit;   // Editing Or Not

	bool				m_bSetCtrl; // UpdateCtrl�Լ� ȣ�� �߿� EditPresent�Լ��� ȣ��Ǵ� ��Ȳ�� ���� ���� flag

public:
	virtual void Start() {}
	void SetCtrl(bool bCtrl) { m_bSetCtrl = bCtrl; }
	bool IsSetCtrl() const { return m_bSetCtrl; }

	void SetTarget(CGameObject* pTarget)
	{
		if (m_pTarget != pTarget)
		{
			// ���ο� Ÿ���� ������ �� ���.
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

	virtual void Update(); // �� Component�� Updateó���̴�.
	virtual void UpdateCtrl() = 0; // �� ���̾�α׸���, Ctrl�� ���� ó���� �ٸ���.
	virtual bool Init() { return true; }
	
	afx_msg virtual void EditPresent(UINT id) {} // �����Ǹ� ���ؼ� �� Dlg���� ǥ���� ���� Target�� ǥ��.

};

