#pragma once

#include "MainFrame.h"

class CPopUpDlg :
	public CDialogEx
{
public:
	CPopUpDlg();
	~CPopUpDlg();
	virtual void OnOK();
	virtual void OnCancel();
	
	void GameViewFocus() { ((CMainFrame*)AfxGetMainWnd())->GetGameView()->SetFocus(); }
};

