#pragma once

#include "SetDlg.h"
#include <Engine/Material.h>

// CMtrlDataDlg 대화 상자

class CMtrlDataDlg : public CSetDlg
{
	DECLARE_DYNAMIC(CMtrlDataDlg)

public:
	CMtrlDataDlg();   // 표준 생성자입니다.
	virtual ~CMtrlDataDlg();

private:
	Ptr<CMaterial>	m_pMtrl;


public:
	bool Init(Ptr<CMaterial> pMtrl, const wstring& strName);
	virtual int	 IsCollisionTex(const CPoint& point); // CtrlWnd와 충돌이 되었는지 여부 판단 & 인덱스반환.
	virtual void CollisionTex(const CString& strTexName, DWORD_PTR dwData, int idx);
	// 충돌인덱스에 대한 후처리 함수.
	// 충돌한 아이템의 핸들을 줘서, Res의 주소를 받아온다.
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MATERIALDATA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// EditCtrl
	CEdit m_Int[4];
	CEdit m_Float[4];
	CEdit m_Vec2X[4];
	CEdit m_Vec2Y[4];
	CEdit m_Vec4X[4];
	CEdit m_Vec4Y[4];
	CEdit m_Vec4Z[4];
	CEdit m_Vec4W[4];
	CEdit m_Tex[4];
	CEdit m_Info;

	// Text
	CStatic m_IntText[4];
	CStatic m_FloatText[4];
	CStatic m_Vec2Text[4][3];
	CStatic m_Vec4Text[4][5];
	CStatic m_TexText[4];
	virtual void OnOK();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
