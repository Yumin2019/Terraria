#pragma once

#include "PopUpDlg.h"
// CLayerCheckDlg 대화 상자

class CLayerCheckDlg : public CPopUpDlg
{
	DECLARE_DYNAMIC(CLayerCheckDlg)

public:
	CLayerCheckDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CLayerCheckDlg();

public:
	virtual void UpdateCtrl();

private:
	UINT*		m_LayerCheck;

public:
	virtual bool Init();

	

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LAYERCHECK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CButton m_EditLayer[32];
	CComboBox m_EditCombo;
	afx_msg void OnBnClickedAllCheck();
	CButton m_EditCheckAll;
	CButton m_EditPresent;
	afx_msg void OnBnClickedPresent();
	afx_msg void OnChangedTargetLayer();

	CEdit m_Name;
	virtual void OnCancel();
	afx_msg void OnClickedIdxChange();
};
