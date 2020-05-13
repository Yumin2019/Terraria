#pragma once


// CListDlg 대화 상자

class CListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CListDlg)

public:
	CListDlg(const CString& strCaption, const vector<CString>& vecList);   // 표준 생성자입니다.
	virtual ~CListDlg();


private:
	CString			m_strCaption;
	vector<CString> m_vecList;
	CString			m_strSelect;
	UINT			m_iSelect;

public:
	CString& GetSelectString() { return m_strSelect; }
	UINT GetSelectIdx() const { return m_iSelect; }
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_EditList;
	virtual BOOL OnInitDialog();
	afx_msg void OnDoubleClick(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnOK();
};
