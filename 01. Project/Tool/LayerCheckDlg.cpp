// LayerCheckDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "LayerCheckDlg.h"
#include "afxdialogex.h"
#include "InfoView.h"
#include "InspectorDlg.h"
#include "ListDlg.h"

#include <Engine/SceneMgr.h>
#include <Engine/Layer.h>
#include <Engine/Scene.h>
#include <Engine/CollisionMgr.h>


// CLayerCheckDlg 대화 상자

IMPLEMENT_DYNAMIC(CLayerCheckDlg, CDialogEx)

CLayerCheckDlg::CLayerCheckDlg(CWnd* pParent /*=nullptr*/) :
	m_LayerCheck(nullptr)
{

}

CLayerCheckDlg::~CLayerCheckDlg()
{
}

void CLayerCheckDlg::UpdateCtrl()
{
	// ComboBox의 Layer에서 충돌이 일어나야 하는 Layer에 대한 정보를 EditCtrl로 옮겨준다.

	CString strGet;

	// 지금 checkbox의 Layer의 인덱스를 가져온다.
	m_EditCombo.GetWindowTextW(strGet);

	int iComboIdx = _wtoi(strGet.Left(2));

	for (size_t i = 0; i < MAX_LAYER; ++i)
	{
		if (iComboIdx < i)
		{
			// Min : iComboIdx  Max : i
			// i번째에 해당하는 EditCtrl을 셋팅한다.


			if (m_LayerCheck[iComboIdx] & (1 << i))
				m_EditLayer[i].SetCheck(1);
			else
				m_EditLayer[i].SetCheck(0);
		}
		else
		{
			// Min : i Max : iComboIdx

			if (m_LayerCheck[i] & (1 << iComboIdx))
				m_EditLayer[i].SetCheck(1);
			else
				m_EditLayer[i].SetCheck(0);
		}

	}

	GameViewFocus();
}

bool CLayerCheckDlg::Init()
{
	CScene* pScene = CSceneMgr::GetInst()->GetScene();
	CLayer** pLayerArr = pScene->GetLayerArr();

	m_EditCombo.ResetContent();

	// Combobox에 Layer 목록을 넣는 작업 + Checkbox에 Layer목록을 넣는 작업.

	for (size_t i = 0; i < MAX_LAYER; ++i)
	{
		// 번호' 'Layer이름
		wstring strLayer = std::to_wstring(i) + L" " + pLayerArr[i]->GetName();
		m_EditCombo.AddString(strLayer.c_str());

		m_EditLayer[i].SetWindowTextW(strLayer.c_str());
	}

	m_LayerCheck = CCollisionMgr::GetInst()->GetLayerCheck();

	m_EditCombo.SetCurSel(0);

	CString strName;
	m_EditCombo.GetWindowTextW(strName);
	m_Name.SetWindowTextW(strName.Right(strName.GetLength() - 2)); 
	// 숫자 부분 빼고 문자열을 받아온다.
	// 그것을 이름으로 설정해준다.

	UpdateCtrl();

	return true;
}


void CLayerCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	int id = IDC_CHECK1;
	for (size_t i = 0; i < MAX_LAYER; ++i)
	{
		DDX_Control(pDX, id + i, m_EditLayer[i]);
	}

	DDX_Control(pDX, IDC_COMBO1, m_EditCombo);
	DDX_Control(pDX, IDC_CHECK33, m_EditCheckAll);
	DDX_Control(pDX, IDC_BUTTON1, m_EditPresent);

	DDX_Control(pDX, IDC_EDIT1, m_Name);
}


BEGIN_MESSAGE_MAP(CLayerCheckDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK33, &CLayerCheckDlg::OnBnClickedAllCheck)
	ON_BN_CLICKED(IDC_BUTTON1, &CLayerCheckDlg::OnBnClickedPresent)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CLayerCheckDlg::OnChangedTargetLayer)
	ON_BN_CLICKED(IDC_BUTTON4, &CLayerCheckDlg::OnClickedIdxChange)
END_MESSAGE_MAP()


// CLayerCheckDlg 메시지 처리기


void CLayerCheckDlg::OnBnClickedAllCheck()
{

	// 누른 시점에서 체크 on / off에 따라서 끄고 키는 처리를 한다.
	bool bCheck = m_EditCheckAll.GetCheck();

	for (size_t i = 0; i < MAX_LAYER; ++i)
	{
		m_EditLayer[i].SetCheck(bCheck);
	}


}


void CLayerCheckDlg::OnBnClickedPresent()
{
	// LayerCollision 처리.
	// 해당 레이어에 충돌중인 오브젝트가 있는 경우에 처리하지 않도록 하자.

	CString strGet;

	// 지금 checkbox의 Layer의 인덱스를 가져온다.
	m_EditCombo.GetWindowTextW(strGet);

	int iComboIdx = _wtoi(strGet.Left(2));

	// 예외처리
	// 1. iComboIdx에 해당하는 Layer에 충돌중인 오브젝트가 존재.
	// 2. i에 해당하는 Layer에 충돌중인 오브젝트가 존재.
	// 위의 두 사항은 결국 모든 Layer를 순회하는 경우에 포함된다.
	// CheckBox에서 설정을 해둔 옵션 그대로 적용이 된다. 즉, 기존의 상황에 대해서 고려하지 않고, 활성화와 비활성화의
	// 상태를 고려하여 모두 처리하고 있다.
	// 따라서 모든 레이어에 대해서 확인을 해줘야 한다.

	// 기존에 a b c에 대해서 충돌중이고 d e f 를 새로 충돌처리하고 나머지는 처리하지 않는 경우.
	// d e f와 combo인덱스에 해당하는 레이어만 따져서 충돌중이 아니니 적용을 할 것이 아니라,
	// 충돌되지 않아야 하는 d e f에 대해서도 당연히 따져줘야 한다.(기존에 충돌중인 상황이니까)
	// 이렇듯 미지의 레이어 d e f는 사실상 몇 개인지 알 수 없다. (그전의 레이어 설정에 따라 달라질 부분이다.)

	// 어떤 레이어도 충돌이 아닌 경우에만 처리한다.
	for (size_t i = 0; i < MAX_LAYER; ++i)
	{
		if (CCollisionMgr::GetInst()->OnCollisionLayer(i))
			return; // Debug msg
	}

	for (size_t i = 0; i < MAX_LAYER; ++i)
	{
		if (m_EditLayer[i].GetCheck()) // 해당 레이어와 충돌처리를 할지에 대한 여부.
			CCollisionMgr::GetInst()->CheckCollisionLayer(i, iComboIdx); 
		else
			CCollisionMgr::GetInst()->UnCheckCollisionLayer(i, iComboIdx);
	}

	// 문자열의 이름을 바꿔준다.
	CString strName;
	m_Name.GetWindowTextW(strName);

	CScene* pScene = CSceneMgr::GetInst()->GetScene();
	CLayer* pLayer = pScene->GetLayer(iComboIdx);
	pLayer->SetName(strName.GetBuffer());

	m_EditCombo.DeleteString(iComboIdx);

	wstring strLayerSet = std::to_wstring(iComboIdx) + L" ";
	strLayerSet += strName;
	m_EditCombo.InsertString(iComboIdx, strLayerSet.c_str());
	m_EditCombo.SetCurSel(iComboIdx);
	
	m_EditLayer[iComboIdx].SetWindowTextW(strLayerSet.c_str());

	CInspectorDlg* pInspectorDlg = ((CInfoView*)((CMainFrame*)AfxGetMainWnd())->GetInfoView())->GetInspectorDlg();
	
	if (pInspectorDlg->GetTarget())
		pInspectorDlg->UpdateCtrl();

	// Update를 받아야 하는 쪽.
	// CheckBox 의 Text ㅇㅇ 
	// ComboBox 의 Text  ㅇㅇ 
	// EditCtrl Text 위에서 바꿔서 같이 처리 ㅇㅇ
	// Target의 Layer(Inspector) 


	// 업데이트 된 내용을 쏴주자.
	UpdateCtrl();

}



void CLayerCheckDlg::OnChangedTargetLayer()
{
	CString strName;
	m_EditCombo.GetWindowTextW(strName);
	m_Name.SetWindowTextW(strName.Right(strName.GetLength() - 2));

	UpdateCtrl();
}


#include "ButtonDlg.h"
void CLayerCheckDlg::OnCancel()
{
	CButtonDlg::g_bShowLayer = false;
	CPopUpDlg::OnCancel();
}


void CLayerCheckDlg::OnClickedIdxChange()
{
	// Target Layer를 알아온다.
	CString strGet;
	m_EditCombo.GetWindowTextW(strGet);

	int iLayer1 = _wtoi(strGet.Left(2));

	vector<CString> vecString;
	vecString.resize(MAX_LAYER);

	for (size_t i = 0; i < MAX_LAYER; ++i)
	{
		// 번호' 'Layer이름을 받아온다.
		m_EditLayer[i].GetWindowTextW(vecString[i]);
	}

	// List Dlg를 통해서 바꾸려고 하는 Layer를 받아온다.
	CListDlg dlg(L"Layer Select", vecString);

	int id = dlg.DoModal(); // 루프가 시작되어, 다른 View에 대한 처리를 하지 않게 된다.

	if (id == IDCANCEL)
		return;

	CString strSelect = dlg.GetSelectString();
	int iLayer2 = _wtoi(strSelect.Left(2));

	if (iLayer1 == iLayer2)
		return;

	// Layer 변경.
	CSceneMgr::GetInst()->GetScene()->ChangeLayer(iLayer1, iLayer2);

	Init();

	GameViewFocus();

}
