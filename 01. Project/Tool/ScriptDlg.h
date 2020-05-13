#pragma once


#include "ComponentDlg.h"							

// CScriptDlg 대화 상자

class CScriptDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CScriptDlg)

public:
	CScriptDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CScriptDlg();

	size_t	m_iScriptNum;

public:
	virtual void UpdateCtrl();
	virtual void Update();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCRIPTDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_Script;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDelete();
};
