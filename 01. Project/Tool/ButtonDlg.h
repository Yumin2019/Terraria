#pragma once


class CTileDlg;
class CLayerCheckDlg;

// CButtonDlg 대화 상자

class CButtonDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CButtonDlg)

public:
	CButtonDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CButtonDlg();


private:
	CTileDlg*		m_pTileDlg;
	CLayerCheckDlg* m_pLayerDlg;


public:
	static bool			g_bShowTile;
	static bool			g_bShowLayer;

public:
	bool Init();
	CTileDlg*			GetTileDlg() const { return m_pTileDlg; }
	CLayerCheckDlg*		GetLayerDlg() const { return m_pLayerDlg; }



// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BUTTONDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedTile();
	afx_msg void OnBnClickedLayer();
	CButton m_PlayBtn;
	CButton m_bPauseBtn;
	afx_msg void OnBnClickedPlayBtn();
	afx_msg void OnBnClickedPauseBtn();
	afx_msg void OnBnClickedToolCam();
	CButton m_ToolCam;
};
