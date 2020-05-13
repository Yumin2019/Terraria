#pragma once

#include "ComponentDlg.h"

class CMtrlDataDlg;

// CMeshRenderDlg 대화 상자

class CMeshRenderDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CMeshRenderDlg)

public:
	CMeshRenderDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMeshRenderDlg();

private:
	CMtrlDataDlg*	m_pMtrlData;
	bool			m_bShow;

public:
	virtual void UpdateCtrl();
	virtual void EndDialog();
public:
	CMtrlDataDlg* GetMtrlDataDlg() const { return m_pMtrlData; }
	bool	IsShow() const { return m_bShow; }

public:
	void SetShow(bool bShow) { m_bShow = bShow; }

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESHRENDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditMesh;
	CEdit m_EditMtrl;
	CButton m_EditDis;
	afx_msg void OnBnClickedDisable();
	afx_msg void OnBnClickedMesh();
	afx_msg void OnBnClickedMtrl();
	afx_msg void OnBnClickedSetDataBtn();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
