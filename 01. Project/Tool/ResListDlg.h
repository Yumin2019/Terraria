#pragma once


#include "TreeCtrlDlg.h"
// CResListDlg 대화 상자


class CResListDlg : public CTreeCtrlDlg
{
	DECLARE_DYNAMIC(CResListDlg)

public:
	CResListDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CResListDlg();


private:
	RES_TYPE	m_eDragType; // Drag하고 있는 아이템의 타입이다.

public:
	virtual bool Init();
	virtual void Update();
	virtual void Renew();
	virtual void DragFlag();


private:
	void LoadResource();
	void LoadResource(const CString& strFolderPath);
	void Load(const CString& strFullPath);


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESLIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void PostNcDestroy();
};
