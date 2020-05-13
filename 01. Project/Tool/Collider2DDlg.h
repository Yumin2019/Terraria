#pragma once

#include "ComponentDlg.h"
// CCollider2DDlg 대화 상자

class CCollider2DDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CCollider2DDlg)

public:
	CCollider2DDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCollider2DDlg();

public:
	virtual void UpdateCtrl();

	afx_msg void EditPresent(UINT id);


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLLIDER2D };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	CEdit m_Edit[6];
	afx_msg void OnBnClickedDisable();
	CButton m_EditDis;
	CComboBox m_EditCombo;
	afx_msg void OnCollTypeChanged();
};
