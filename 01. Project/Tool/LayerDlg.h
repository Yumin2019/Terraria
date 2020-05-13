#pragma once

#include "InspectorDlg.h"
// CLayerDlg 대화 상자

class CLayerDlg : public CInspectorDlg
{
	DECLARE_DYNAMIC(CLayerDlg)

public:
	CLayerDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CLayerDlg();

private:
	bool		m_bMoveAll;

public:
	virtual void UpdateCtrl();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LAYER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditText;
	CButton m_EditButton;
	CButton m_EditCheck;
	afx_msg void OnCheckBox();
	afx_msg void OnBnClickedLayerBtn();
	afx_msg void OnBnClickedMove();
	afx_msg void OnClickedCloneBtn();
};
