// ScriptDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "ScriptDlg.h"
#include "afxdialogex.h"
#include "resource.h"


// CScriptDlg 대화 상자

IMPLEMENT_DYNAMIC(CScriptDlg, CDialogEx)

CScriptDlg::CScriptDlg(CWnd* pParent /*=nullptr*/) :
	CComponentDlg(COMPONENT_TYPE::SCRIPT),
	m_iScriptNum(0)
{

}

CScriptDlg::~CScriptDlg()
{
}

#include <Engine/Script.h>
#include <Script/ScriptMgr.h>

void CScriptDlg::UpdateCtrl()
{
	m_Script.ResetContent();

	const vector<CScript*>& vecScript = GetTarget()->GetScriptVec();

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		m_Script.AddString(CScriptMgr::GetScriptName(vecScript[i]));
	}

	if (vecScript.empty())
		m_Script.AddString(L"none");

	m_Script.SetCurSel(0);
	m_iScriptNum = vecScript.size();

	EnableUpdate();
}

void CScriptDlg::Update()
{
	if (IsEdit())
		return;

	if (IsUpdate())
	{
		// 업데이트를 했는데. 변경사항이 있으면 처리.
		if (m_iScriptNum == GetTarget()->GetScriptSize())
			return;
		else
		{
			UpdateCtrl();
		}

	}
	else
	{
		// Update하지 않았으면 무조건 처리.
		UpdateCtrl();
		RedrawWindow();
	}
	
}

void CScriptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Script);
}


BEGIN_MESSAGE_MAP(CScriptDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CScriptDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON1, &CScriptDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_BUTTON4, &CScriptDlg::OnBnClickedDelete)
END_MESSAGE_MAP()


// CScriptDlg 메시지 처리기


void CScriptDlg::OnCbnSelchangeCombo1()
{
	GameViewFocus();
}

#include "ListDlg.h"

void CScriptDlg::OnBnClickedAdd()
{
	// 제공하는 모든 Script를 출력한다.
	vector<wstring> vecScriptName;
	vector<CString> vecString;
	CScriptMgr::GetScriptInfo(vecScriptName);

	for (size_t i = 0; i < vecScriptName.size(); ++i)
	{
		vecString.push_back(vecScriptName[i].c_str());
	}

	CListDlg dlg(L"All Script", vecString);

	int id = dlg.DoModal(); // 루프가 시작되어, 다른 View에 대한 처리를 하지 않게 된다.

	if (id == IDCANCEL)
		return;
	
	CString strSelect = dlg.GetSelectString();

	// 선택한 인덱스에 따른(SCRIPT_TYPE) 스크립트를 할당받아서 넣어준다.
	CScript* pScript = CScriptMgr::GetScript(strSelect.GetBuffer());
	UINT iType = pScript->GetScriptType();

	// 중복체크를 해줘야지.
	const vector<CScript*> vecScript = GetTarget()->GetScriptVec();

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		if (vecScript[i]->GetScriptType() == iType)
		{
			// 이미 있는 Script를 넣으려고 하는 상황이다.
			SAFE_DELETE(pScript);
			GameViewFocus();
			return;
		}
	}

	GetTarget()->AddComponent(pScript);

	GameViewFocus();
}


void CScriptDlg::OnBnClickedDelete()
{
	// 가지고 있는 Script를 출력해준다.
	const vector<CScript*> vecScript = GetTarget()->GetScriptVec();
	vector<CString> vecString;

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecString.push_back(CScriptMgr::GetScriptName(vecScript[i]));
	}

	CListDlg dlg(L"Owned Script", vecString);

	int id = dlg.DoModal(); // 루프가 시작되어, 다른 View에 대한 처리를 하지 않게 된다.

	if (id == IDCANCEL)
		return;

	// 선택한 스크립트 이름을 토대로 그것을 할당하여, 어떤 Script idx인지 알아온다.
	// 그리고 삭제한다.
	CScript* pScript = CScriptMgr::GetScript(dlg.GetSelectString().GetBuffer());

	UINT iScriptType = pScript->GetScriptType();

	SAFE_DELETE(pScript);

	// 이것을 토대로 Script vector에서 없앤다.
	GetTarget()->DeleteScript(iScriptType);
	
	if (GetTarget()->HasScript() == false)
		m_Script.ResetContent();
	// 없어지면 남은 하나의 잔상이 생긴다. 이것을 없애주는 처리.

	GameViewFocus();
}
