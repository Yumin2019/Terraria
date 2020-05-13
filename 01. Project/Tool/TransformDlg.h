#pragma once

#include "ComponentDlg.h"

class CGameObject;
// CTransformDlg 대화 상자

class CTransformDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CTransformDlg)

public:
	CTransformDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTransformDlg();

public:
	virtual void UpdateCtrl();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRANSFORM };
#endif


private:
	Vec2	m_tPivotArr[5];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	CEdit m_Edit[9];
	afx_msg void EditPresent(UINT id);
	afx_msg void OnChangedPivot();
	CComboBox m_Pivot;
	afx_msg void OnClicked2DScaleBtn();
};
