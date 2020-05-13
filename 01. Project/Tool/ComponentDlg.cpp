#include "stdafx.h"
#include "ComponentDlg.h"

#include <Engine/GameObject.h>
#include <Engine/Component.h>
#include <Engine/privatefunc.h>


CComponentDlg::CComponentDlg(COMPONENT_TYPE eType) :
	m_pTarget(nullptr),
	m_eComType(eType),
	m_bUpdate(false),
	m_bEdit(false),
	m_bSetCtrl(false)
{
}

CComponentDlg::~CComponentDlg()
{
}

void CComponentDlg::PostNcDestroy()
{
	// PostNcDestory 함수는 윈도우가 삭제된 이후에 호출이 되는 함수로서,
	// 주로 객체의 정리에 사용된다.
	// 소멸자의 경우 virtual로 지정이 되어있으므로, 자식 클래스 또한 
	// 자신의 소멸자를 호출하고 부모의 소멸자를 호출할 것이다.
	// 아무튼, 공통된 처리를  하도록 한다.
	delete this;
	CDialogEx::PostNcDestroy();
}

void CComponentDlg::Update()
{
	if (!m_pTarget)
		assert(nullptr);
	
	// 편집중이라면 처리를 하지 않는다.
	if (IsEdit())
		return;

	// Ctrl의 Update가 이루어졌다.
	if (IsUpdate())
	{
		// 각 Dlg마다 필요로 하는 정보가 Update 되었는지 여부에 따라서 처리를 한다.

		if (GetTarget()->GetComponent(m_eComType)->IsChanged())
		{
			UpdateCtrl();
			ComponentSetChange(GetTarget()->GetComponent(m_eComType), false);
		}
		else
			return;
	}
	// 업데이트를 한 적이 없다.
	else
	{
		UpdateCtrl(); // CtrlUpdate
		RedrawWindow();
		ComponentSetChange(GetTarget()->GetComponent(m_eComType), false);
	}
}

BOOL CComponentDlg::PreTranslateMessage(MSG * pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
			((CMainFrame*)AfxGetMainWnd())->GetGameView()->SetFocus();

		// Ctrl Wnd에서 Enter키를 누른 경우. GameView에 Focus
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
