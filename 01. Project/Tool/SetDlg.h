#pragma once
#include <Engine/Ptr.h>

class CComponentDlg;

class CSetDlg :
	public CDialogEx
{
public:
	CSetDlg();
	virtual ~CSetDlg();

private:
	CComponentDlg*		m_pComDlg;

public:
	CComponentDlg*	GetDlg() const { return m_pComDlg; }
	void SetDlg(CComponentDlg* pDlg) { m_pComDlg = pDlg; }

public:
	virtual int	 IsCollisionTex(const CPoint& point) = 0;
	virtual void CollisionTex(const CString& strTexName, DWORD_PTR dwData, int idx) = 0;
};

