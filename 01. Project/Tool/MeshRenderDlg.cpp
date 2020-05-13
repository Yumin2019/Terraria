// MeshRenderDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "MeshRenderDlg.h"
#include "afxdialogex.h"

#include <Engine/MeshRender.h>
#include <Engine/ResMgr.h>
#include <Engine/PathMgr.h>

#include "ListDlg.h"
#include "MtrlDataDlg.h"


// CMeshRenderDlg 대화 상자

IMPLEMENT_DYNAMIC(CMeshRenderDlg, CDialogEx)

CMeshRenderDlg::CMeshRenderDlg(CWnd* pParent /*=nullptr*/) 
	: CComponentDlg(COMPONENT_TYPE::MESHRENDER),
	m_pMtrlData(nullptr),
	m_bShow(false)
{

}

CMeshRenderDlg::~CMeshRenderDlg()
{
	SAFE_DELETE(m_pMtrlData);
}

void CMeshRenderDlg::UpdateCtrl()
{
	// Target의 Mesh와 Mtrl을 출력한다.

	SetCtrl(true);

	CString strMesh;
	CString strMtrl;

	CMeshRender* pMeshRender = GetTarget()->GetMeshRender();

	Ptr<CMesh> pMesh = pMeshRender->GetMesh();
	Ptr<CMaterial> pMtrl = pMeshRender->GetSharedMaterial();

	// null일 가능성도 있음.

	if (pMesh != nullptr)
		strMesh = CPathMgr::GetFileName(pMesh->GetName().c_str());

	if (pMtrl != nullptr)
		strMtrl = CPathMgr::GetFileName(pMtrl->GetName().c_str());

	m_EditMesh.SetWindowTextW(strMesh);
	m_EditMtrl.SetWindowTextW(strMtrl);

	SetCtrl(false);

	EnableUpdate();
}
void CMeshRenderDlg::EndDialog()
{
	m_pMtrlData->ShowWindow(false);
	m_bShow = false;

	CComponentDlg::EndDialog();
}

void CMeshRenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditMesh);
	DDX_Control(pDX, IDC_EDIT2, m_EditMtrl);
	DDX_Control(pDX, IDC_CHECK1, m_EditDis);

	m_EditDis.SetCheck(1);
}


BEGIN_MESSAGE_MAP(CMeshRenderDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK1, &CMeshRenderDlg::OnBnClickedDisable)
	ON_BN_CLICKED(IDC_BUTTON1, &CMeshRenderDlg::OnBnClickedMesh)
	ON_BN_CLICKED(IDC_BUTTON2, &CMeshRenderDlg::OnBnClickedMtrl)
	ON_BN_CLICKED(IDC_BUTTON4, &CMeshRenderDlg::OnBnClickedSetDataBtn)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CMeshRenderDlg 메시지 처리기


void CMeshRenderDlg::OnBnClickedDisable()
{
	Event tEvent = {};

	if (m_EditDis.GetCheck()) 
		tEvent.eType = EVENT_TYPE::ENABLE_COMPONENT;
	else
		tEvent.eType = EVENT_TYPE::DISABLE_COMPONENT;

	tEvent.wParam = (DWORD_PTR)GetTarget()->GetComponent(GetType());
	CToolEventMgr::GetInst()->AddEvent(tEvent);

	((CMainFrame*)AfxGetMainWnd())->GetGameView()->SetFocus();

}


void CMeshRenderDlg::OnBnClickedMesh()
{
	// ResMgr에게서 필요한 Res에 관련된 파일을 가져온다.
	const map<wstring, CResource*>& mapRes = CResMgr::GetInst()->GetResMap(RES_TYPE::MESH);
	vector<CString> vecString;

	map<wstring, CResource*>::const_iterator iter;
	map<wstring, CResource*>::const_iterator iterEnd = mapRes.end();

	for (iter = mapRes.begin(); iter != iterEnd; ++iter)
	{
		// Res의 이름을 넣어준다.
		vecString.push_back(iter->first.c_str());
	}

	CListDlg dlg(L"Mesh", vecString);

	int id = dlg.DoModal(); // 루프가 시작되어, 다른 View에 대한 처리를 하지 않게 된다.

	if (id == IDCANCEL)
		return;

	CString strSelect = dlg.GetSelectString();

	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(strSelect.GetBuffer());
	GetTarget()->GetMeshRender()->SetMesh(pMesh);

}


void CMeshRenderDlg::OnBnClickedMtrl()
{
	const map<wstring, CResource*>& mapRes = CResMgr::GetInst()->GetResMap(RES_TYPE::MATERIAL);
	vector<CString> vecString;
	vector<wstring> vecFullName;

	vecString.reserve(mapRes.size());
	vecFullName.reserve(mapRes.size());

	map<wstring, CResource*>::const_iterator iter;
	map<wstring, CResource*>::const_iterator iterEnd = mapRes.end();

	for (iter = mapRes.begin(); iter != iterEnd; ++iter)
	{
		vecString.push_back(CPathMgr::GetFileName(iter->first.c_str()));
		vecFullName.push_back(iter->first);
	}

	CListDlg dlg(L"Material", vecString);

	int id = dlg.DoModal(); // 루프가 시작되어, 다른 View에 대한 처리를 하지 않게 된다.

	if (id == IDCANCEL)
		return;

	UINT iSelectIdx = dlg.GetSelectIdx();

	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(vecFullName[iSelectIdx]);
	GetTarget()->GetMeshRender()->SetMaterial(pMtrl);

	UpdateCtrl();
}


void CMeshRenderDlg::OnBnClickedSetDataBtn()
{
	// Clone 버튼이다.

	// MtrlDataDlg를 보여준다.
	m_pMtrlData->ShowWindow(true);
	m_bShow = true;
	Ptr<CMaterial> pMtrl;

	wstring strName = GetTarget()->GetMeshRender()->GetSharedMaterial()->GetName();
	
	bool bClone = false;
	for (UINT i = 0; i < 9; ++i)
	{
		if (strName.find(std::to_wstring(i)) != -1) // 이름에 숫자가 있는지 확인.
		{
			bClone = true;
			break;
		}
	}

	if (strName == L"Standard2DMtrl")
		bClone = false;

	// %d.mtrl

	if (bClone)
	{
		// Clone 이면 그대로 사용
		pMtrl = GetTarget()->GetMeshRender()->GetSharedMaterial();
	}
	else
	{
		// 원본이면 복사.
		pMtrl = GetTarget()->GetMeshRender()->GetCloneMaterial();
	}

	// Init에서 필요한 정보만 출력 & 정보 출력.
	m_pMtrlData->Init(pMtrl, GetTarget()->GetName());

	// Clone 출력.
	UpdateCtrl();

}


int CMeshRenderDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComponentDlg::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pMtrlData = new CMtrlDataDlg;
	m_pMtrlData->Create(IDD_MATERIALDATA);
	m_pMtrlData->ShowWindow(false);
	m_pMtrlData->SetDlg(this);

	return 0;
}

