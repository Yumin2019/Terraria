
// MainFrame.cpp: CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "Tool.h"

#include "MainFrame.h"

#include "GameView.h"
#include "ButtonView.h"
#include "DebugView.h"
#include "HierachyView.h"
#include "InfoView.h"

#include "ButtonView.h"
#include "ButtonDlg.h"
#include "LayerCheckDlg.h"
#include "ToolEventMgr.h"

#include "ListDlg.h"

#include <Script/PrototypeMgr.h>
#include <Engine/ResMgr.h>
#include <Engine/SceneMgr.h>
#include <Engine/Material.h>
#include <Engine/Scene.h>
#include <Engine/Layer.h>
#include <Engine/MeshRender.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_ACTIVATE()
	ON_COMMAND(ID_EMPTYOBJECT, &CMainFrame::CreateEmptyObject)
	ON_COMMAND(ID_ADDCOMPONENT_ANIMATOR2D32788, &CMainFrame::AddAnimator2D)
	ON_COMMAND(ID_ADDCOMPONENT_CAMERA32789, &CMainFrame::AddCamera)
	ON_COMMAND(ID_ADDCOMPONENT_COLLIDER2D32790, &CMainFrame::AddCollider2D)
	ON_COMMAND(ID_ADDCOMPONENT_LIGHT2D32791, &CMainFrame::AddLight2D)
	ON_COMMAND(ID_ADDCOMPONENT_MESHRENDER32792, &CMainFrame::AddMeshRender)
	ON_COMMAND(ID_DELETECOMPONENT_ANIMATOR2D, &CMainFrame::DeleteAnimator2D)
	ON_COMMAND(ID_DELETECOMPONENT_CAMERA, &CMainFrame::DeleteCamera)
	ON_COMMAND(ID_DELETECOMPONENT_COLLIDER2D, &CMainFrame::DeleteCollider2D)
	ON_COMMAND(ID_DELETECOMPONENT_LIGHT2D, &CMainFrame::DeleteLight2D)
	ON_COMMAND(ID_DELETECOMPONENT_MESHRENDER, &CMainFrame::DeleteMeshRender)
	ON_COMMAND(ID_32771, &CMainFrame::OnClickedSaveScene)
	ON_COMMAND(ID_32772, &CMainFrame::OnClickedSceneLoad)
	ON_COMMAND(ID_ADDCOMPONENT_TILEMAP2D, &CMainFrame::AddTileMap2D)
	ON_COMMAND(ID_DELETECOMPONENT_TILEMAP2D, &CMainFrame::DeleteTileMap2D)
	ON_COMMAND(ID_PROTOTYPE_CREATEPROTOTYPE, &CMainFrame::OnClickedClonePrototypeObj)
	ON_COMMAND(ID_PROTOTYPE_ADDPROTOTYPE, &CMainFrame::OnClickedAddPrototype)
	ON_COMMAND(ID_PROTOTYPE_DELETEPROTOTYPE, &CMainFrame::OnClickedDeletePrototype)
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept :
	m_bCreate(false)
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
	DESTROY_SINGLE(CPrototypeMgr);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Frame Window Size
	CRect rt(0, 0, FRAME_X, FRAME_Y);
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);
	SetWindowPos(nullptr, 0, 0, rt.Width(), rt.Height(), 0);

	m_bCreate = true;

	// C ~ 클래스는, 윈도우 함수를 토대로 객체화 시켜놓은 클래스이다.
	// 기존의 데이터형과 Conversion을 지원하며, 추가적인 함수를 제공한다.

	// Frame Wnd 창 고정 분할
	m_mainSplitter.CreateStatic(this, 1, 3); // 1행 3열로 분할한다.
	m_mainSplitter.SetBorder(CPoint(2, 2), CPoint(1, 1));

	m_mainSplitter.SetColumnInfo(0, GAMEVIEW_X, GAMEVIEW_X); // 스플리터 창의 열. 이상적인너비 최소 최대.
	// 0열의 크기를 지정한다.

	m_mainSplitter.CreateView(0, 1, RUNTIME_CLASS(CHierachyView), CSize(HIERACHYVIEW_X, HIERACHYVIEW_Y), nullptr);
	// 0 행 1열에 (2번째 칸)에 Hierachy 뷰를 만든다. (행 열 runtimeclass지정, 초기크기, ..)

	m_mainSplitter.CreateView(0, 2, RUNTIME_CLASS(CInfoView), CSize(INFOVIEW_X, INFOVIEW_Y), nullptr);
	// 0 행 2열에 InfoView를 만든다. 사이즈로 0 0 을 입력하면, 남는 크기를 가져가는 처리이다.

	m_subSplitter.SetBorder(CPoint(2, 2), CPoint(1, 1));
	m_subSplitter.CreateStatic(&m_mainSplitter, 3, 1, WS_CHILD | WS_VISIBLE, m_mainSplitter.IdFromRowCol(0, 0));
	// 부모를 지정하고, 행 열을 지정하여 모양을 결정. child 옵션을 준다. 어느 위치의 Splitter인지 지정한다.
	// 즉, 첫번째 칸은 3행 1열로 자르는 Splitter인 것이다.

	m_subSplitter.CreateView(0, 0, RUNTIME_CLASS(CButtonView), CSize(BTNVIEW_X, BTNVIEW_Y), nullptr);
	m_subSplitter.CreateView(1, 0, RUNTIME_CLASS(CGameView), CSize(GAMEVIEW_X, GAMEVIEW_Y), nullptr);
	m_subSplitter.CreateView(2, 0, RUNTIME_CLASS(CDebugView), CSize(DBGVIEW_X, DBGVIEW_Y), nullptr);
	// 0 행부터 버튼뷰, 게임뷰, 디버그 뷰를 생성한다.

	m_subSplitter.GetWindowRect(rt);
	m_subSplitter.SetWindowPos(nullptr, 0, 0, rt.Width(), rt.Height(), 0);

	GetGameView()->SetFocus();

	return 0;
}


bool CMainFrame::Init()
{
	((CButtonView*)GetButtonView())->Init();
	((CGameView*)GetGameView())->Init();
	((CDebugView*)GetDebugView())->Init();
	((CHierachyView*)GetHierachyView())->Init();
	((CInfoView*)GetInfoView())->Init();

	return true;
}

void CMainFrame::Update()
{
	//((CButtonView*)GetButtonView())->Update();
	((CGameView*)GetGameView())->Update();
	//((CDebugView*)GetDebugView())->Update();
	((CHierachyView*)GetHierachyView())->Update();
	((CInfoView*)GetInfoView())->Update();
	
}

void CMainFrame::PostLoadScene()
{
	// 댕글링 포인터. 설정 (HierachyView)
	RenewHierachy();

	// Nullptr 셋팅.
	((CInfoView*)GetInfoView())->SetTarget(nullptr);

	if(((CButtonView*)GetButtonView())->GetButtonDlg()->GetLayerDlg())
		((CButtonView*)GetButtonView())->GetButtonDlg()->GetLayerDlg()->Init();

	// Default Mtrl의 정보를 가져온다.
	const vector<wstring>& vecDefMtrl = CResMgr::GetInst()->GetDefMtrl();
	const map<wstring, CResource*> mapMtrl = CResMgr::GetInst()->GetResMap(RES_TYPE::MATERIAL);
	vector<wstring> vecMtrl;
	vecMtrl.reserve(mapMtrl.size());

	// vecMtrl에 Map의 키 값을 넣는 과정.
	for (const auto& pair : mapMtrl)
	{
		vecMtrl.push_back(pair.first);
	}

	// Map의 Mtrl Key 값과 기본 Mtrl의 key 값을 비교하여 Clone 수를 알아내는 작업.


	for (size_t i = 0; i < vecDefMtrl.size(); ++i)
	{
		int iClone = 0;

		for (size_t j = 0; j < vecMtrl.size(); ++j)
		{
			if (vecMtrl[j].find(vecDefMtrl[i]) != std::wstring::npos) // DefMtrl의 문자열이 있는지 검사한다.
			{
				++iClone;
			}
		}

		// vecDefMtrl의 문자열을 토대로 Resource를 찾는다.
		CResMgr::GetInst()->FindRes<CMaterial>(vecDefMtrl[i])->SetCloneCount(iClone - 1);
	}

	// Default Mtrl의 이름을 받아온다. ~Mtrl
	// 이것과 왼쪽부터 문자열을 비교하여 같은 것의 개수를 센다.
	// ~Mtrl ~Mtrl_Clone_1 이런 식으로 2개가 나온다면, -1을 해서 1개의 CloneCount가 있는 것이다. 
	// 이후에 이 값을 통해서 키 값으로 mtrl을 찾고 Count를 셋팅해주자.


}

void CMainFrame::RenewHierachy()
{
	((CHierachyView*)GetHierachyView())->RenewHierachy();
	((CHierachyView*)GetHierachyView())->RenewRes();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_OVERLAPPED | FWS_ADDTOTITLE
		 | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;


	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;
}


// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기
void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 그렇지 않으면 기본 처리합니다.
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	if (!m_bCreate)
		return;

	CRect rt;
	//m_subSplitter.GetPane(0, 0)->GetClientRect(rt);
	int w = rt.Width();
	int h = rt.Height();
}

#include <Engine/Transform.h>
#include <Engine/GameObject.h>
#include <Engine/SceneMgr.h>
#include <Engine/Layer.h>
#include <Engine/Scene.h>


void CMainFrame::CreateEmptyObject()
{
	// Transform만 가진 Object를 생성해서 출력.
	CGameObject* pObj = new CGameObject;
	pObj->AddComponent(new CTransform);
	pObj->SetName(L"Empty Object");

	CSceneMgr::GetInst()->GetScene()->FindLayer(L"Default")->AddGameObject(pObj);
	((CHierachyView*)GetHierachyView())->RenewHierachy();
	((CInfoView*)GetInfoView())->SetTarget(pObj);
}


void CMainFrame::AddAnimator2D()
{
	((CInfoView*)GetInfoView())->AddAnimator2D();
}


void CMainFrame::AddCamera()
{
	((CInfoView*)GetInfoView())->AddCamera();
}


void CMainFrame::AddCollider2D()
{
	((CInfoView*)GetInfoView())->AddCollider2D();

}


void CMainFrame::AddLight2D()
{
	((CInfoView*)GetInfoView())->AddLight2D();

}


void CMainFrame::AddMeshRender()
{
	((CInfoView*)GetInfoView())->AddMeshRender();
}


void CMainFrame::AddTileMap2D()
{
	((CInfoView*)GetInfoView())->AddTileMap2D();
}



void CMainFrame::DeleteAnimator2D()
{
	((CInfoView*)GetInfoView())->DeleteAnimator2D();
}


void CMainFrame::DeleteCamera()
{
	((CInfoView*)GetInfoView())->DeleteCamera();

}


void CMainFrame::DeleteCollider2D()
{
	((CInfoView*)GetInfoView())->DeleteCollider2D();
}



void CMainFrame::DeleteLight2D()
{
	((CInfoView*)GetInfoView())->DeleteLight2D();
}


void CMainFrame::DeleteMeshRender()
{
	((CInfoView*)GetInfoView())->DeleteMeshRender();
}


void CMainFrame::DeleteTileMap2D()
{
	((CInfoView*)GetInfoView())->DeleteTileMap2D();
}


#include <Script/SaveLoadMgr.h>
#include <Engine/PathMgr.h>
#include <Engine/Core.h>
#include "HierachyView.h"

void CMainFrame::OnClickedSaveScene()
{
	static wchar_t szFilter[] = L"Scene 파일(*.scene)|*.scene|모든 파일(*.*)|*.*||";

	CFileDialog dlg(FALSE, nullptr, nullptr, OFN_HIDEREADONLY, szFilter);

	CString strInitPath = CPathMgr::GetResPath();
	strInitPath += L"Scene\\";
	dlg.m_ofn.lpstrInitialDir = strInitPath;

	CString pathName;
	if (dlg.DoModal() == IDOK)
	{
		pathName = dlg.GetPathName();
	}
	else
		return;

	CSaveLoadMgr::SaveScene(pathName.GetBuffer());
}


void CMainFrame::OnClickedSceneLoad()
{
	static wchar_t szFilter[] = L"Scene 파일(*.scene)|*.scene|모든 파일(*.*)|*.*||";

	CFileDialog dlg(TRUE, nullptr, nullptr, OFN_HIDEREADONLY, szFilter);

	CString strInitPath = CPathMgr::GetResPath();
	strInitPath += L"Scene\\";
	dlg.m_ofn.lpstrInitialDir = strInitPath;

	CString pathName;
	if (dlg.DoModal() == IDOK)
	{
		pathName = dlg.GetPathName();
	}
	else
		return;

	// Load과정에서 현재 Scene을 날린다.
	// InfoView에서 문제가 생길 가능성이 있다. (댕글링)

	CSaveLoadMgr::LoadScene(pathName.GetBuffer());

	CCore::GetInst()->SetSceneMode(SCENE_MODE::STOP);
	
	CMainFrame::PostLoadScene();
}



void CMainFrame::OnClickedClonePrototypeObj()
{

	/*
	프로토타입의 객체들을 ListDlg를 통해 출력해주고, 입력을 받으면, 해당 물체를 Clone하여 Default Layer에 넣어준다.
	*/


	const map<wstring, CGameObject*>& mapProto = CPrototypeMgr::GetInst()->GetPrototypes();
	vector<CString> vecString;

	for (const auto& pair : mapProto)
	{
		vecString.push_back(pair.first.c_str());
	}

	CListDlg dlg(L"Prototype Object", vecString);

	int id = dlg.DoModal();

	if (id == IDCANCEL)
		return;

	CString strSelect = dlg.GetSelectString();

	// 선택한 오브젝트를 동적할당하여, Default Layer에 넣어준다.
	CGameObject* pObj = CPrototypeMgr::GetInst()->FindObj(strSelect.GetString())->Clone();
	CLayer* pLayer = CSceneMgr::GetInst()->GetScene()->FindLayer(L"Default");

	if (pLayer == nullptr)
	{
		SAFE_DELETE(pObj);
		assert(false);
	}

	pLayer->AddGameObject(pObj);

	// 만약 플레이 중이라면.
	if (CCore::GetInst()->GetSceneMode() == SCENE_MODE::PLAY)
	{
		if (pObj->GetMeshRender())
			pObj->GetMeshRender()->GetCloneMaterial()->DisableFileSave();

		pObj->Awake();
		pObj->Start();
	}

	((CHierachyView*)GetHierachyView())->RenewHierachy();

	GetGameView()->SetFocus();
}


void CMainFrame::OnClickedAddPrototype()
{
	/* 프로토타입 추가 함수 ListDlg를 통해서 현재 Scene에 있는 오브젝트의 이름을 보여주고, 입력을 받아서 프로토타입의 객체로 담는다. */

	if (CCore::GetInst()->GetSceneMode() != SCENE_MODE::STOP)
		return;

	vector<CString> vecString;
	CScene* pScene = CSceneMgr::GetInst()->GetScene();

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		// 자식의 오브젝트는 안 된다. ㅇㅇ
		// 오직 부모의 오브젝트만, 또한 자식을 가지고 있는 경우는 제외.

		CLayer* pLayer = pScene->GetLayer(i);
		const vector<CGameObject*>& vecObj = pLayer->GetParentObjVec();

		for (int j = 0; j < vecObj.size(); ++j)
		{
			if (vecObj[j]->GetChildVec().empty())
			{
				// 자식이 없는 경우만 넣는다.
				vecString.push_back(vecObj[j]->GetName().c_str());
			}
		}
	}

	CListDlg dlg(L"Add Prototype", vecString);

	int id = dlg.DoModal();

	if (id == IDCANCEL)
		return;

	CString strSelect = dlg.GetSelectString();

	// 선택한 오브젝트를 찾아온다.
	vector<CGameObject*> vecFind;
	CSceneMgr::GetInst()->FindObjByTag(strSelect.GetBuffer(), vecFind);

	// 찾은 오브젝트를 복사하여 Map에 넣어준다.
	CPrototypeMgr::GetInst()->CreatePrototype(vecFind[0]->GetName(), vecFind[0]);
	
	GetGameView()->SetFocus();

}


void CMainFrame::OnClickedDeletePrototype()
{
	/* 현재 프토토타입으로 생성되어 있는 객체의 이름을 보여주고, 입력을 받아서 삭제한다. */

	if (CCore::GetInst()->GetSceneMode() != SCENE_MODE::STOP)
		return;

	vector<CString> vecString;

	const map<wstring, CGameObject*>& mapProto = CPrototypeMgr::GetInst()->GetPrototypes();

	for (const auto& pair : mapProto)
	{
		vecString.push_back(pair.first.c_str());
	}

	CListDlg dlg(L"Delete Prototype", vecString);

	int id = dlg.DoModal();

	if (id == IDCANCEL)
		return;

	CString strSelect = dlg.GetSelectString();

	// 선택한 프로토타입을 없앤다.
	CPrototypeMgr::GetInst()->DeletePrototype(strSelect.GetBuffer());

	GetGameView()->SetFocus();
}
