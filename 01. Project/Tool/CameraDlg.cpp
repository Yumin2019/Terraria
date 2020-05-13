// CameraDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CameraDlg.h"
#include "afxdialogex.h"
#include "ListDlg.h"

#include <Engine/Camera.h>
#include <Engine/Scene.h>
#include <Engine/Layer.h>
#include <Engine/SceneMgr.h>

// CCameraDlg 대화 상자

IMPLEMENT_DYNAMIC(CCameraDlg, CDialogEx)

CCameraDlg::CCameraDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(COMPONENT_TYPE::CAMERA),
	m_bEditError(false)
{

}

CCameraDlg::~CCameraDlg()
{
}


void CCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_EditCombo);
	DDX_Control(pDX, IDC_EDIT1, m_EditRange);
	DDX_Control(pDX, IDC_EDIT2, m_EditFOV);
	DDX_Control(pDX, IDC_EDIT3, m_EditScale);
	DDX_Control(pDX, IDC_CHECK1, m_EditCheck);

	m_EditCombo.AddString(L"Orthographic");
	m_EditCombo.AddString(L"Perspective");

	m_EditCheck.SetCheck(1);
	DDX_Control(pDX, IDC_COMBO2, m_Layer);
}


BEGIN_MESSAGE_MAP(CCameraDlg, CDialogEx)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT1, IDC_EDIT3, &CCameraDlg::EditPresent)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT1, IDC_EDIT3, &CComponentDlg::OnKillFocus)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT1, IDC_EDIT3, &CComponentDlg::OnSetFocus)
	ON_BN_CLICKED(IDC_CHECK1, &CCameraDlg::OnCheckBox)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CCameraDlg::OnProjTypeChanged)
	ON_BN_CLICKED(IDC_BUTTON1, &CCameraDlg::OnBnClickedAddLayer)
	ON_BN_CLICKED(IDC_BUTTON4, &CCameraDlg::OnBnClickedDeleteLayer)
END_MESSAGE_MAP()


void CCameraDlg::EditPresent(UINT id)
{
	// Edit을 이용해서 UpdateCtrl을 하다가 호출된 EditPre 함수는 처리하지 않는다.
	if (IsSetCtrl() || m_bEditError)
		return;

	CCamera* pCamera = GetTarget()->GetCamera();

	// Range FOV Scale
	CString strGet;
	CString strSet;
	float fData = 0.f;

	// Range 
	m_EditRange.GetWindowTextW(strGet);
	fData = _wtof(strGet);

	if (fData <= 1.f)
	{
		m_bEditError = true;
		pCamera->SetFar(10.f);

		strSet.Format(L"%.2f", pCamera->GetFar());
		m_EditRange.SetWindowTextW(strSet);

	}
	else
		pCamera->SetFar(fData);

	// Scale
	m_EditScale.GetWindowTextW(strGet);
	fData = _wtof(strGet);

	if (fData <= 0.f)
	{
		m_bEditError = true;
		pCamera->SetScale(0.1f);

		strSet.Format(L"%.2f", pCamera->GetScale());
		m_EditScale.SetWindowTextW(strSet);
	}
	else
		pCamera->SetScale(fData);


	// FOV
	m_EditFOV.GetWindowTextW(strGet);
	fData = _wtof(strGet);

	if (fData <= 0.f)
	{
		m_bEditError = true;
		pCamera->SetFOV(XMConvertToRadians(1.f));

		strSet.Format(L"%.2f", 1.f);
		m_EditFOV.SetWindowTextW(strSet);
	}
	else
	{
		fData = XMConvertToRadians(fData); // TO Radian
		pCamera->SetFOV(fData);
	}

	

	m_bEditError = false;

}

void CCameraDlg::OnCheckBox()
{
	//Event tEvent = {};

	//if (m_EditCheck.GetCheck())
	//	tEvent.eType = EVENT_TYPE::ENABLE_COMPONENT;
	//else
	//	tEvent.eType = EVENT_TYPE::DISABLE_COMPONENT;

	//tEvent.wParam = (DWORD_PTR)GetTarget()->GetComponent(GetType());
	//CEventMgr::GetInst()->AddEvent(tEvent);

	//((CMainFrame*)AfxGetMainWnd())->GetGameView()->SetFocus();
}

void CCameraDlg::OnProjTypeChanged()
{
	PROJ_TYPE eType = (PROJ_TYPE)m_EditCombo.GetCurSel();
	GetTarget()->GetCamera()->SetProjType(eType);

	UpdateCtrl();

	GAMEVIEW->SetFocus();
}

void CCameraDlg::UpdateCtrl()
{
	SetCtrl(true);

	CCamera* pCamera = GetTarget()->GetCamera();
	CString str;

	str.Format(L"%.2f", pCamera->GetFar());
	m_EditRange.SetWindowTextW(str);

	float fFOV = XMConvertToDegrees(pCamera->GetFOV()); // to Degree
	str.Format(L"%.2f", fFOV);
	m_EditFOV.SetWindowTextW(str);

	str.Format(L"%.2f", pCamera->GetScale());
	m_EditScale.SetWindowTextW(str);

	PROJ_TYPE eType = pCamera->GetProjType();
	m_EditCombo.SetCurSel((UINT)eType);

	if (eType == PROJ_TYPE::ORTHGRAPHIC)
	{
		// 직교는 Scale 사용 FOV X
		m_EditScale.SetReadOnly(false);
		m_EditFOV.SetReadOnly(true);
	}
	else
	{
		// 원근은 FOV 사용 Scale X
		m_EditScale.SetReadOnly(true);
		m_EditFOV.SetReadOnly(false);
	}

	// 찍는 Layer에 대한 처리.
	m_Layer.ResetContent();

	if (pCamera->IsLayerAllCheck())
		m_Layer.AddString(L"ALL LAYER");

	else
	{
		CScene* pScene = CSceneMgr::GetInst()->GetScene();

		// 자신이 찍고 있는 Layer를 ComboBox에 List로 추가한다.

		for (int i = 0; i < MAX_LAYER; ++i)
		{
			if (pCamera->IsLayerCheck(i))
			{
				// 해당 Layer와 충돌을 한다면...
				CLayer* pLayer = pScene->GetLayer(i);

				// 이름을 만들어준다.
				wstring strLayer = std::to_wstring(i) + L" " + pLayer->GetName();

				m_Layer.AddString(strLayer.c_str());

			}
		}

	}

	m_Layer.SetCurSel(0);
	
	SetCtrl(false);
	EnableUpdate();
}


// CCameraDlg 메시지 처리기


void CCameraDlg::OnBnClickedAddLayer()
{
	CCamera* pCamera = GetTarget()->GetCamera();
	CScene* pScene = CSceneMgr::GetInst()->GetScene();

	// 모든 Layer에 대해서 출력한다.
	vector<CString> vecString;

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pScene->GetLayer(i);
		wstring strLayer = std::to_wstring(i) + L" " + pLayer->GetName();

		vecString.push_back(strLayer.c_str());
	}

	CListDlg dlg(L"All Layer", vecString);

	int id = dlg.DoModal(); 

	if (id == IDCANCEL)
		return;

	CString strSelect = dlg.GetSelectString();

	// 선택한 Layer의 인덱스를 알아온다.
	int iLayer = _wtoi(strSelect.Left(2));

	if (pCamera->IsLayerCheck(iLayer) == false)
		pCamera->SetLayerCheck(iLayer, true);

	GameViewFocus();
}


void CCameraDlg::OnBnClickedDeleteLayer()
{
	CCamera* pCamera = GetTarget()->GetCamera();
	CScene* pScene = CSceneMgr::GetInst()->GetScene();

	// 자신이 충돌처리를 하는 Layer만 가져온다.
	vector<CString> vecString;

	for (int i = 0; i < MAX_LAYER; ++i)
	{

		if (pCamera->IsLayerCheck(i))
		{
			CLayer* pLayer = pScene->GetLayer(i);
			wstring strLayer = std::to_wstring(i) + L" " + pLayer->GetName();

			vecString.push_back(strLayer.c_str());
		}
	}

	CListDlg dlg(L"Check Layer", vecString);

	int id = dlg.DoModal();

	if (id == IDCANCEL)
		return;

	CString strSelect = dlg.GetSelectString();

	// 선택한 Layer의 인덱스를 알아온다.
	int iLayer = _wtoi(strSelect.Left(2));

	pCamera->SetLayerCheck(iLayer, false);

	GameViewFocus();
}
