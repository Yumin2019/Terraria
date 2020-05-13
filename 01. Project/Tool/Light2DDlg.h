#pragma once

#include "ComponentDlg.h"
// CLight2DDlg 대화 상자

class CLight2DDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CLight2DDlg)

public:
	CLight2DDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CLight2DDlg();

public:
	virtual void UpdateCtrl();
	virtual void EditPresent(UINT id);

	void OnCheckBox();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIGHT2D };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditR;
	CEdit m_EditG;
	CEdit m_EditB;
	CEdit m_EditRange;
	CButton m_EditCheck;
};
