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
	// PostNcDestory �Լ��� �����찡 ������ ���Ŀ� ȣ���� �Ǵ� �Լ��μ�,
	// �ַ� ��ü�� ������ ���ȴ�.
	// �Ҹ����� ��� virtual�� ������ �Ǿ������Ƿ�, �ڽ� Ŭ���� ���� 
	// �ڽ��� �Ҹ��ڸ� ȣ���ϰ� �θ��� �Ҹ��ڸ� ȣ���� ���̴�.
	// �ƹ�ư, ����� ó����  �ϵ��� �Ѵ�.
	delete this;
	CDialogEx::PostNcDestroy();
}

void CComponentDlg::Update()
{
	if (!m_pTarget)
		assert(nullptr);
	
	// �������̶�� ó���� ���� �ʴ´�.
	if (IsEdit())
		return;

	// Ctrl�� Update�� �̷������.
	if (IsUpdate())
	{
		// �� Dlg���� �ʿ�� �ϴ� ������ Update �Ǿ����� ���ο� ���� ó���� �Ѵ�.

		if (GetTarget()->GetComponent(m_eComType)->IsChanged())
		{
			UpdateCtrl();
			ComponentSetChange(GetTarget()->GetComponent(m_eComType), false);
		}
		else
			return;
	}
	// ������Ʈ�� �� ���� ����.
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

		// Ctrl Wnd���� EnterŰ�� ���� ���. GameView�� Focus
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
