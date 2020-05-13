#pragma once

#include "SetDlg.h"

#include <Engine/Texture.h>

class CAnimator2D;

// CAddAnimDlg 대화 상자

class CAnimation2D;

class CAddAnimDlg : public CSetDlg
{
	DECLARE_DYNAMIC(CAddAnimDlg)

public:
	CAddAnimDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAddAnimDlg();

private:
	Ptr<CTexture>	m_pTexture;
	CAnimator2D*	m_pAnimator; // 자신이 가리키는 애니메이터 !
	CAnimation2D*	m_pEditAnim;

public:
	bool Init(CAnimator2D* pAnimator, const wstring& strName);
	bool InitEdit(CAnimator2D* pAnimator, CAnimation2D* pAnimation);
	virtual int	 IsCollisionTex(const CPoint& point); 
	virtual void CollisionTex(const CString& strTexName, DWORD_PTR dwData, int idx);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDANIM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Name;
	CEdit m_Tex;
	CEdit m_FrameCnt;
	CEdit m_PlayTime;
	CComboBox m_Option;
	afx_msg void OnChangedOption();
	CEdit m_Vec2[2][2];
	CButton m_HorizonBox;
	afx_msg void OnBnClickedAnimationAdd();
	virtual void OnOK();
	CButton m_AddBtn;
};
