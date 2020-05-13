// ButtonDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "ButtonDlg.h"
#include "afxdialogex.h"

#include "TileDlg.h"
#include "LayerCheckDlg.h"
#include "GameView.h"
#include "HierachyView.h"
#include <Engine/Core.h>


#include <Script/TileMapMgr.h>
#include <Script/PoolMgr.h>
#include <Script/UtilMgr.h>

#include <Engine/Core.h>
#include <Engine/Layer.h>
#include <Engine/Scene.h>
#include <Engine/GameObject.h>
#include <Engine/Transform.h>
#include <Engine/Animator2D.h>
#include <Engine/TileMap2D.h>
#include <Script/SaveLoadMgr.h>
#include <Engine/PathMgr.h>
#include <Engine/EventMgr.h>
#include "InfoView.h"

bool CButtonDlg::g_bShowTile;
bool CButtonDlg::g_bShowLayer;

// CButtonDlg 대화 상자

IMPLEMENT_DYNAMIC(CButtonDlg, CDialogEx)

CButtonDlg::CButtonDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BUTTONDLG, pParent),
	m_pLayerDlg(nullptr),
	m_pTileDlg(nullptr)
{

}

CButtonDlg::~CButtonDlg()
{
	SAFE_DELETE(m_pTileDlg);
	SAFE_DELETE(m_pLayerDlg);


	if (CCore::GetInst()->GetSceneMode() == SCENE_MODE::STOP)
	{
		// Tool 종료시 보험으로 testScene을 저장함. 
		CScene* pScene = CSceneMgr::GetInst()->GetScene();

		wstring strPath = CPathMgr::GetResPath();
		strPath += L"Scene/insurance/testScene";

		CString LogWriteString;
		CTime time = CTime::GetCurrentTime();
		LogWriteString.Format(L" %d.%d %dh %dm %ds.scene", time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

		strPath += LogWriteString;

		// 보험폴더 / testScene 1.1 1h 1m 1s.scene 저장.
		CSaveLoadMgr::SaveScene(strPath);
	}
}

bool CButtonDlg::Init()
{
	// TileDlg 는 Play를 위해서 생성이 되어 있어야 한다.( Tile Option의 정보를 관리하고 있다 )
	m_pTileDlg = new CTileDlg;
	m_pTileDlg->Create(IDD_TILEDLG);
	m_pTileDlg->Init();

	return true;
}

void CButtonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON5, m_PlayBtn);
	DDX_Control(pDX, IDC_BUTTON3, m_bPauseBtn);
	DDX_Control(pDX, IDC_CHECK1, m_ToolCam);


	m_ToolCam.SetCheck(0);
}


BEGIN_MESSAGE_MAP(CButtonDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CButtonDlg::OnBnClickedTile)
	ON_BN_CLICKED(IDC_BUTTON2, &CButtonDlg::OnBnClickedLayer)
	ON_BN_CLICKED(IDC_BUTTON5, &CButtonDlg::OnBnClickedPlayBtn)
	ON_BN_CLICKED(IDC_BUTTON3, &CButtonDlg::OnBnClickedPauseBtn)
	ON_BN_CLICKED(IDC_CHECK1, &CButtonDlg::OnBnClickedToolCam)
END_MESSAGE_MAP()



void CButtonDlg::PostNcDestroy()
{
	delete this;
	CDialogEx::PostNcDestroy();
}


void CButtonDlg::OnOK() {}
void CButtonDlg::OnCancel() {}

// CButtonDlg 메시지 처리기



void CButtonDlg::OnBnClickedTile()
{
	m_pTileDlg->ShowWindow(true);
	g_bShowTile = true;
}


void CButtonDlg::OnBnClickedLayer()
{
	if (!m_pLayerDlg)
	{
		// LayerCheck Popup
		m_pLayerDlg = new CLayerCheckDlg;
		m_pLayerDlg->Create(IDD_LAYERCHECK);
		m_pLayerDlg->Init();
	}

	m_pLayerDlg->ShowWindow(TRUE);
	g_bShowLayer = true;
}


#include <Script/TileMapMgr.h>


void CButtonDlg::OnBnClickedPlayBtn()
{
	GameViewSetFocus;

	if (CCore::GetInst()->GetSceneMode() == SCENE_MODE::PLAY)
		return;

	// nullptr 셋팅.
	((CInfoView*)MAINFRAME->GetInfoView())->SetTarget(nullptr);

	CScene* pScene = CSceneMgr::GetInst()->GetScene();


	bool bSave = false;
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pScene->GetLayer(i);

		if (!pLayer->GetParentObjVec().empty())
		{
			bSave = true;
			break;
		}
	}

	if (bSave)
	{
		wstring strPath = CPathMgr::GetResPath();
		strPath += L"Scene/testScene.scene";
		CSaveLoadMgr::SaveScene(strPath);
	}

	CCore::GetInst()->SetSceneMode(SCENE_MODE::PLAY);

	// 모든 Object Enable = false
	for (size_t i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pScene->GetLayer(i);
		const vector<CGameObject*>& vecObj = pLayer->GetLayerObjVec();

		for (size_t j = 0; j < vecObj.size(); ++j)
			vecObj[j]->SetEnable(false);
	}

	// Title Layer만 다시 Enable = true
	CLayer* pTitleLayer = pScene->FindLayer(L"Title");
	const vector<CGameObject*>& vecTitle = pTitleLayer->GetLayerObjVec();

	for (size_t i = 0; i < vecTitle.size(); ++i)
		vecTitle[i]->SetEnable(true);


	// 모든 애니메이션 처리 시작
	for (size_t i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pScene->GetLayer(i);
		const vector<CGameObject*>& vecObj = pLayer->GetLayerObjVec();

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			if (vecObj[j]->GetAnimator2D())
			{
				vecObj[j]->GetAnimator2D()->SetPause(false);
			}
		}
	}

	// TileInfo 셋팅.
	// 옵션값 셋팅하는 부분이다.  
	
	CLayer* pLayer = pScene->GetLayer(TILE_LAYER_IDX);
	const vector<CGameObject*>& vecObj = pLayer->GetLayerObjVec(); // TileObject를 얻어온다.
	TILE* pTileOption = m_pTileDlg->GetTileOption(); // TileOptionArray 정보이다. 
	int iTileNum = IMAGE_NUM_X * IMAGE_NUM_Y;

	for (int iTileMap = 0; iTileMap < vecObj.size(); ++iTileMap)
	{
		if (!vecObj[iTileMap]->GetTileMap2D())
			assert(false);

		CTileMap2D* pTileMap = vecObj[iTileMap]->GetTileMap2D();

		for (int iIdx = 0; iIdx < 4000; ++iIdx)
		{
			// 참조 idx를 구한다.
			int iTileX = pTileMap->GetTileBuffer(iIdx).iX;
			int iTileY = pTileMap->GetTileBuffer(iIdx).iY;

			// 이미지에서의 인덱스를 얻었으니 해당 인덱스의 정보에 접근해서 option을 얻어온다.
			int iTileIdx = iTileY * IMAGE_NUM_X + iTileX;

			if (iTileIdx < 0 || iTileIdx >= iTileNum)
				assert(false);

			TILE eTileOption = pTileOption[iTileIdx];

			// 이렇게 얻은 Option을 토대로, TileInfo를 얻는다.
			// 이것을 TileMap의 정보로 셋팅한다.

			pTileMap->SetTileInfo(iIdx, CTileDlg::g_tTileInfo[(UINT)eTileOption]);
			
		}
	}

	int arrCount[(UINT)LIST_ITEM::END] = { 15, 7, 7 };

	// My Mgr Init
	CTileMapMgr::GetInst()->Init();
	CPoolMgr::GetInst()->Init(35, arrCount);
	CUtilMgr::GetInst()->Init();
	

	pScene->Awake();
	pScene->Start();

	// Awake, Start시점에서 호출된 Event를 처리한다. 
	CEventMgr::GetInst()->Update();

	// 해당 Event를 Tool과 연동한다.
	((CHierachyView*)MAINFRAME->GetHierachyView())->Update();

	// Awake, Start호출이후에 Event가 삭제되기 때문이다.
}

#include "HierachyView.h"
#include "ButtonView.h"

void CButtonDlg::OnBnClickedPauseBtn()
{
	GameViewSetFocus;

	wstring strPath = CPathMgr::GetResPath();
	strPath += L"Scene/testScene.scene";
	CSaveLoadMgr::LoadScene(strPath);

	CCore::GetInst()->SetSceneMode(SCENE_MODE::STOP);


	// nullptr 셋팅.
	MAINFRAME->PostLoadScene();


}


void CButtonDlg::OnBnClickedToolCam()
{
	bool bCheck = m_ToolCam.GetCheck();

	// ToolCamera에 접근하자.
	((CGameView*)MAINFRAME->GetGameView())->SetViewInput(bCheck);
}
