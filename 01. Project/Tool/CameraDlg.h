#pragma once

#include "ComponentDlg.h"
// CCameraDlg 대화 상자

class CCameraDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CCameraDlg)

public:
	CCameraDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCameraDlg();

private:
	bool		m_bEditError;

public:
	virtual void EditPresent(UINT id);
	void OnCheckBox();
	void OnProjTypeChanged();

	virtual void UpdateCtrl();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_EditCombo;
	CEdit m_EditRange;
	CEdit m_EditFOV;
	CEdit m_EditScale;
	CButton m_EditCheck;
	CComboBox m_Layer;
	afx_msg void OnBnClickedAddLayer();
	afx_msg void OnBnClickedDeleteLayer();
};
