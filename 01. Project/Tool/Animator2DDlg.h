#pragma once

#include "ComponentDlg.h"

class CAddAnimDlg;

// CAnimator2DDlg 대화 상자

class CAnimator2DDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CAnimator2DDlg)

public:
	CAnimator2DDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAnimator2DDlg();

private:
	// combobox는 랜덤엑세스를 필요로하는데, map의 특성상 이런 것이 구현되어 있지 않다.
	// 따라서 애니메이션의 이름을 담아둘 vector를 하나 선언한다.

	vector<wstring>			m_vecAnim;
	CAddAnimDlg*			m_pAddAnim;
	bool					m_bShow;

public:
	CAddAnimDlg* GetAddAnimDlg() const { return m_pAddAnim; }

	void SetShow(bool bShow) { m_bShow = bShow; }
	bool IsShow() const { return m_bShow; }

public:
	virtual void UpdateCtrl();
	virtual void EndDialog();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATOR2D };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedAnimationAdd();
	afx_msg void OnChangedCombo();
	CButton m_EditPlay;
	CButton m_EditPause;
	CButton m_EditAdd;
	CComboBox m_EditCombo;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CButton m_Check;
	afx_msg void OnBnClickedCheckBox();
	afx_msg void OnClickedDeleteAnimation();
	afx_msg void OnBnClickedEditAnimation();
};
