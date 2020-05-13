
// Tool.cpp: 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Tool.h"
#include "MainFrame.h"
#include "PenMgr.h"
#include "ToolEventMgr.h"

#include <Engine/Core.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolApp

BEGIN_MESSAGE_MAP(CToolApp, CWinApp)
END_MESSAGE_MAP()


// CToolApp 생성

CToolApp::CToolApp() noexcept
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 응용 프로그램을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Tool.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CToolApp 개체입니다.

CToolApp theApp;


// CToolApp 초기화

BOOL CToolApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면 AfxInitRichEdit2()가 있어야 합니다.
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));


	// 주 창을 만들기 위해 이 코드에서는 새 프레임 창 개체를
	// 만든 다음 이를 응용 프로그램의 주 창 개체로 설정합니다.
	CFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;


	m_pMainWnd = pFrame;
	// 프레임을 만들어 리소스와 함께 로드합니다.
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, nullptr,
		nullptr);

	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	CRect rt;
	::GetWindowRect(((CMainFrame*)m_pMainWnd)->GetGameView()->m_hWnd, &rt); // MainWnd(메인프레임)
	// 을 이용해서, 게임뷰의 핸들을 얻어서 이것의 크기를 얻어온다. (미리 설정을 해두었으니)
	
	CCore::GetInst()->Init(((CMainFrame*)m_pMainWnd)->GetGameView()->m_hWnd, Resolution(rt.Width(), rt.Height()), true);
	// 엔진에게 GameView의 핸들을 전달하고, GameView의 크기를 전달한다.
	// 그 결과로 GameView에서 게임이 출력되게 될 것이다.

	CCore::GetInst()->SetSceneMode(SCENE_MODE::STOP);
	TestSceneCreate();


	// PenMgr
	CPenMgr::GetInst()->Init();

	((CMainFrame*)m_pMainWnd)->Init();


	return TRUE;
}

int CToolApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}



#include <Engine/Scene.h>

// Manager 
#include <Engine/Core.h>
#include <Engine/CollisionMgr.h>

// Res
#include <Engine/Shader.h>
#include <Engine/Mesh.h>
#include <Engine/ResMgr.h>

// Component
#include <Engine/Transform.h>
#include <Engine/MeshRender.h>
#include <Engine/Collider2D.h>
#include <Engine/Animator2D.h>
#include <Engine/Light2D.h>
#include <Engine/Camera.h>
#include <Engine/TileMap2D.h>

#include <Engine/Layer.h>

// Script
#include <Script/BackGroundScript.h>
#include <Script/PlayerScript.h>
#include <Script/ItemScript.h>
#include <Script/InventoryScript.h>
#include <Script/TileScript.h>

bool CToolApp::TestSceneCreate()
{
	CFontMgr::GetInst()->AddFont(L"Andy"); // 0번.

	return true;

	CScene* pScene = CSceneMgr::GetInst()->GetScene();

	// Texture
	wstring strPath = L"/Texture/BackGround/Forest/";
	Ptr<CTexture> pTexForestMt = CResMgr::GetInst()->Load<CTexture>(strPath + L"ForestMt.png", strPath + L"ForestMt.png");
	Ptr<CTexture> pTexTree0 = CResMgr::GetInst()->Load<CTexture>(strPath + L"ForestTree0.png", strPath + L"ForestTree0.png");
	Ptr<CTexture> pTexSky = CResMgr::GetInst()->Load<CTexture>(strPath + L"ForestSky.png", strPath + L"ForestSky.png");
	Ptr<CTexture> pTexTree1 = CResMgr::GetInst()->Load<CTexture>(strPath + L"ForestTree1.png", strPath + L"ForestTree1.png");
	Ptr<CTexture> pTexPlayer = CResMgr::GetInst()->Load<CTexture>(L"/Texture/Player_Normal.png", L"/Texture/Player_Normal.png");


	

	/*
	ForestMt ForestTree0 ForestSky ForestTree1 PlayerSprite 교체 예정.

	wstring strPath = L"Texture/BackGround/Forest/";
	Ptr<CTexture> pTexForestMt = CResMgr::GetInst()->FindRes<CTexture>(strPath + L"ForestMt.png");
	Ptr<CTexture> pTexTree0 = CResMgr::GetInst()->FindRes<CTexture>(strPath + L"ForestTree0.png");
	Ptr<CTexture> pTexSky = CResMgr::GetInst()->FindRes<CTexture>(strPath + L"ForestSky.png");
	Ptr<CTexture> pTexTree = CResMgr::GetInst()->FindRes<CTexture>(strPath + L"ForestTree1.png");
	Ptr<CTexture> pTexPlayer = CResMgr::GetInst()->FindRes<CTexture>(L"Texture/Player_Normal.png");

	*/

	CLayer* pLayerTerrain = pScene->SetLayerName(L"Terrain", 1);
	CLayer* pLayerDefault = pScene->SetLayerName(L"Default", 4);
	CLayer* pLayerItem = pScene->SetLayerName(L"Item", 5);
	CLayer* pLayerTorch = pScene->SetLayerName(L"Torch", 6);
	CLayer* pLayerEffect = pScene->SetLayerName(L"Effect", 30);

	// Layer Collision 
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Torch", L"Default");

	Resolution tRes = CCore::GetInst()->GetRes();

	Ptr<CMesh> pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");


	// 카메라를 넣어준다.
	CGameObject* pCamera = new CGameObject; pCamera->SetName(L"Camera");
	pCamera->AddComponent(new CCamera);
	pCamera->AddComponent(new CTransform);
	pCamera->GetCamera()->SetLayerCheckAll();

	pCamera->GetTransform()->SetLocalPos(RS_WIDTH / 2.f, RS_HEIGHT / -2.f, 1.f);

	pLayerDefault->AddGameObject(pCamera);
	//pCamera->GetCamera()->SetStageSize(Vec2(RS_WIDTH * 2.f, RS_HEIGHT * 2.f)); // 카메라 제한

	// Player
	CGameObject* pPlayer = new CGameObject; pPlayer->SetName(L"Player");
	pPlayer->AddComponent(new CTransform);
	pPlayer->AddComponent(new CMeshRender);
	pPlayer->AddComponent(new CAnimator2D);
	Ptr<CTexture> pTexture = pTexPlayer;

	// 40 * 56   세로 20개.
	pPlayer->GetTransform()->SetLocalPos(RS_WIDTH / 2.f, RS_HEIGHT / -2.f, 100.f);
	pPlayer->GetTransform()->SetLocalScale(40.f, 56.f, 1.f); // 2배.
	pPlayer->GetTransform()->SetLocalRot(0.f, 0.f, 0.f);

	pPlayer->GetMeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Standard2DMtrl")->Clone());
	pPlayer->GetMeshRender()->SetMesh(pRectMesh);

	pPlayer->GetAnimator2D()->AddAnimation(L"PlayerIdle", pTexture, Vec2(0.f, 56.f * 0.f), Vec2(40.f, 56.f), 1);
	pPlayer->GetAnimator2D()->AddAnimation(L"PlayerTool", pTexture, Vec2(0.f, 56.f * 1.f), Vec2(40.f, 56.f), 4,
		1.f, ANIMATION_OPTION::AO_ONCE_RETURN);
	pPlayer->GetAnimator2D()->AddAnimation(L"PlayerJump", pTexture, Vec2(0.f, 56.f * 5.f), Vec2(40.f, 56.f), 1,
		1.f, ANIMATION_OPTION::AO_ONCE_RETURN);
	pPlayer->GetAnimator2D()->AddAnimation(L"PlayerWalk", pTexture, Vec2(0.f, 56.f * 6.f), Vec2(40.f, 56.f), 9);
	pPlayer->GetAnimator2D()->AddAnimation(L"PlayerSword", pTexture, Vec2(0.f, 56.f * 15.f), Vec2(40.f, 56.f), 5,
		1.f, ANIMATION_OPTION::AO_ONCE_RETURN);

	CPlayerScript* pPlyScript = new CPlayerScript;
	pPlayer->AddComponent(pPlyScript);

	pLayerDefault->AddGameObject(pPlayer);

	pPlayer->AddComponent(new CCollider2D);
	pPlayer->GetCollider2D()->SetCollider2DType(COLLIDER2D_TYPE::RECT);
	pPlayer->GetCollider2D()->SetScale(1.f, 1.f, 1.f);
	pPlayer->GetCollider2D()->SetName(L"PlayerCollider");


	//Ptr<CTexture> pTorch = CResMgr::GetInst()->Load<CTexture>(L"Torch", L"Texture/Torch.png");

	//CGameObject* pLight = new CGameObject; pLight->SetName(L"Light0");
	//pLight->AddComponent(new CTransform);
	//pLight->AddComponent(new CMeshRender);
	//pLight->AddComponent(new CLight2D);
	//pLight->AddComponent(new CCollider2D);

	//pLight->GetTransform()->SetLocalPos(pPlayer->GetTransform()->GetLocalPos());
	//pLight->GetTransform()->SetLocalScale(pTorch->GetLength());
	//pLight->GetTransform()->SetLocalRot(0.f, 0.f, 0.f);

	//Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Standard2DMtrl")->Clone();
	//pMtrl->SetData(SHADER_PARAM::TEX_0, &pTorch);

	//pLight->GetMeshRender()->SetMesh(pRectMesh);
	//pLight->GetMeshRender()->SetMaterial(pMtrl);

	//pLight->GetLight2D()->SetLightColor(1.f, 1.f, 1.f);
	//pLight->GetLight2D()->SetLightRange(300.f);
	//pLight->GetLight2D()->SetLightType(LIGHT2D_TYPE::POINT);

	//pLight->GetCollider2D()->SetCollider2DType(COLLIDER2D_TYPE::RECT);
	//pLight->GetCollider2D()->SetOffset(0.f, 0.f, 0.f);
	//pLight->GetCollider2D()->SetScale(1.f, 1.f, 1.f);

	//pLayerTorch->AddGameObject(pLight);

	//pLight = pLight->Clone();

	//pLight->GetTransform()->SetLocalPos(pPlayer->GetTransform()->GetLocalPos() + Vec3(400.f, 0.f, 0.f));
	////pLight->GetTransform()->SetLocalScale(pTorch->GetLength());
	////pLight->GetTransform()->SetLocalRot(0.f, 0.f, 0.f);

	//pLayerTorch->AddGameObject(pLight);



	LayerCreate();

	ItemCreate();


	pScene->Awake();
	pScene->Start();
	CEventMgr::GetInst()->Update();

	return true;
}

void CToolApp::LayerCreate()
{

	wstring strPath = L"/Texture/BackGround/Forest/";
	Ptr<CTexture> pTexForestMt = CResMgr::GetInst()->FindRes<CTexture>(strPath + L"ForestMt.png");
	Ptr<CTexture> pTexTree0 = CResMgr::GetInst()->FindRes<CTexture>(strPath + L"ForestTree0.png");
	Ptr<CTexture> pTexSky = CResMgr::GetInst()->FindRes<CTexture>(strPath + L"ForestSky.png");
	Ptr<CTexture> pTexTree1 = CResMgr::GetInst()->FindRes<CTexture>(strPath + L"ForestTree1.png");
	Ptr<CTexture> pTexPlayer = CResMgr::GetInst()->FindRes<CTexture>(L"Texture/Player_Normal.png");

	CScene* pScene = CSceneMgr::GetInst()->GetScene();


	CLayer* pLayerBack = pScene->SetLayerName(L"BackGround", 0);

	// 백그라운드 : Blue Sky
	float fCoord = 0.f;

	CGameObject*		pBackGround = new CGameObject; pBackGround->SetName(L"BlueSky");
	CBackGroundScript*  pScript = nullptr;
	CGameObject* pCamera = pScene->FindParentObj(L"Camera");
	Ptr<CTexture> pTexture = pTexSky;
	Ptr<CMesh> pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");


	pBackGround->AddComponent(new CTransform);
	pBackGround->AddComponent(new CMeshRender);

	pBackGround->GetTransform()->SetLocalPos(0.f, 0.f, 999.f);
	pBackGround->GetTransform()->SetLocalScale(RS_WIDTH * 2.f, RS_HEIGHT * 2.f, 1.f);
	pBackGround->GetTransform()->EnableLeftTop();

	pBackGround->GetMeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BackGroundMtrl")->Clone());
	pBackGround->GetMeshRender()->SetMesh(pRectMesh);
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, &pTexture);
	fCoord = 0.f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fCoord);
	fCoord = 1.f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_1, &fCoord);

	Vec2 tColor; // x : 사용여부, y : 가중치
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_0, &tColor);
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_1, &tColor);
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_2, &tColor);

	pLayerBack->AddGameObject(pBackGround);
	/*delete pBackGround;*/

	// ForestMt
	pBackGround = new CGameObject; pBackGround->SetName(L"ForestMt0");
	pScript = new CBackGroundScript;
	pTexture = pTexForestMt;

	pBackGround->AddComponent(new CTransform);
	pBackGround->AddComponent(new CMeshRender);
	pBackGround->AddComponent(pScript);

	pBackGround->GetTransform()->SetLocalPos(0.f, 0.f, 999.f);
	pBackGround->GetTransform()->SetLocalScale(RS_WIDTH, RS_HEIGHT, 1.f);

	pBackGround->GetMeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BackGroundMtrl")->Clone());
	pBackGround->GetMeshRender()->SetMesh(pRectMesh);
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, &pTexture);
	fCoord = 0.3f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fCoord);
	fCoord = 1.f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_1, &fCoord);

	tColor.x = 1.f; tColor.y = 0.9f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_0, &tColor);

	tColor.x = 1.f; tColor.y = 0.9f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_1, &tColor);

	tColor.x = 0.f; tColor.y = 0.8f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_2, &tColor);


	Vec2 tLength = pTexture->GetLength(); // &의 &에서 문제가 생기는 것 같다.

	pScript->SetInfo(Vec2(10.f, 0.f), Vec2(0.f, 0.f), tLength, Vec2(200.f, 0.f), Vec2(650.f, 350.f),
		tLength, pCamera); // 직접 Y 좌표를 플레이어의 이동만큼을 처리하는 듯한 느낌..
	pScript->SetCameraGap(100.f);

	pLayerBack->AddGameObject(pBackGround);


	// ForestMt1
	pBackGround = new CGameObject; pBackGround->SetName(L"ForestMt1");
	pScript = new CBackGroundScript;
	pTexture = pTexForestMt;

	pBackGround->AddComponent(new CTransform);
	pBackGround->AddComponent(new CMeshRender);
	pBackGround->AddComponent(pScript);

	pBackGround->GetTransform()->SetLocalPos(0.f, 0.f, 996.f);
	pBackGround->GetTransform()->SetLocalScale(RS_WIDTH, RS_HEIGHT, 1.f);

	pBackGround->GetMeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BackGroundMtrl")->Clone());
	pBackGround->GetMeshRender()->SetMesh(pRectMesh);
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, &pTexture);
	fCoord = -0.3f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fCoord);
	fCoord = 0.68f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_1, &fCoord);

	tColor.x = 1.f; tColor.y = 1.1f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_0, &tColor);

	tColor.x = 1.f; tColor.y = 1.1f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_1, &tColor);

	tColor.x = 0.f; tColor.y = 1.2f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_2, &tColor);

	tLength = pTexture->GetLength();

	pScript->SetInfo(Vec2(5.f, 0.f), Vec2(0.f, 0.f), tLength, Vec2(100.f, 0.f), Vec2(550.f, 350.f),
		tLength, pCamera); // 직접 Y 좌표를 플레이어의 이동만큼을 처리하는 듯한 느낌..
	pScript->SetCameraGap(200.f);

	pLayerBack->AddGameObject(pBackGround);

	// Tree0
	pBackGround = new CGameObject; pBackGround->SetName(L"ForestTree0");
	pScript = new CBackGroundScript;
	pTexture = pTexTree0;

	pBackGround->AddComponent(new CTransform);
	pBackGround->AddComponent(new CMeshRender);
	pBackGround->AddComponent(pScript);

	pBackGround->GetTransform()->SetLocalPos(0.f, 0.f, 980.f);
	pBackGround->GetTransform()->SetLocalScale(RS_WIDTH * 1.07f, RS_HEIGHT * 1.1f, 1.f);

	pBackGround->GetMeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BackGroundMtrl")->Clone());
	pBackGround->GetMeshRender()->SetMesh(pRectMesh);
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, &pTexture);
	fCoord = 0.f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fCoord);
	fCoord = 1.f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_1, &fCoord);

	tColor.x = 1.f; tColor.y = 0.8f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_0, &tColor);

	tColor.x = 1.f; tColor.y = 1.2f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_1, &tColor);

	tColor.x = 1.f; tColor.y = 1.3f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_2, &tColor);

	tLength = pTexture->GetLength();

	pScript->SetInfo(Vec2(15.f, 0.f), Vec2(0.f, 0.f), tLength, Vec2(100.f, 0.f), Vec2(500.f, 50.f),
		tLength, pCamera);
	pScript->SetCameraGap(250.f);

	pLayerBack->AddGameObject(pBackGround);



	// Tree1
	pBackGround = new CGameObject; pBackGround->SetName(L"ForestTree1");
	pScript = new CBackGroundScript;
	pTexture = pTexTree0;

	pBackGround->AddComponent(new CTransform);
	pBackGround->AddComponent(new CMeshRender);
	pBackGround->AddComponent(pScript);

	pBackGround->GetTransform()->SetLocalPos(0.f, 0.f, 970.f);
	pBackGround->GetTransform()->SetLocalScale(RS_WIDTH * 1.07f, RS_HEIGHT * 1.1f, 1.f);

	pBackGround->GetMeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BackGroundMtrl")->Clone());
	pBackGround->GetMeshRender()->SetMesh(pRectMesh);
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, &pTexture);
	fCoord = 0.f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fCoord);
	fCoord = 1.f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_1, &fCoord);

	tColor.x = 1.f; tColor.y = 0.8f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_0, &tColor);

	tColor.x = 1.f; tColor.y = 1.4f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_1, &tColor);

	tColor.x = 1.f; tColor.y = 1.2f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_2, &tColor);

	tLength = pTexture->GetLength();

	pScript->SetInfo(Vec2(20.f, 0.f), Vec2(0.f, 0.f), tLength, Vec2(200.f, 0.f), Vec2(500.f, 50.f),
		tLength, pCamera);
	pScript->SetCameraGap(400.f);

	pLayerBack->AddGameObject(pBackGround);


	// Tree2
	pBackGround = new CGameObject; pBackGround->SetName(L"ForestTree2");
	pScript = new CBackGroundScript;
	pTexture = pTexTree0;

	pBackGround->AddComponent(new CTransform);
	pBackGround->AddComponent(new CMeshRender);
	pBackGround->AddComponent(pScript);

	pBackGround->GetTransform()->SetLocalPos(0.f, 0.f, 960.f);
	pBackGround->GetTransform()->SetLocalScale(RS_WIDTH * 1.07f, RS_HEIGHT * 1.1f, 1.f);

	pBackGround->GetMeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BackGroundMtrl")->Clone());
	pBackGround->GetMeshRender()->SetMesh(pRectMesh);
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, &pTexture);
	fCoord = 0.f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fCoord);
	fCoord = 1.f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_1, &fCoord);

	tColor.x = 1.f; tColor.y = 0.8f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_0, &tColor);

	tColor.x = 1.f; tColor.y = 1.5f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_1, &tColor);

	tColor.x = 1.f; tColor.y = 1.3f;
	pBackGround->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_2, &tColor);

	tLength = pTexture->GetLength();

	pScript->SetInfo(Vec2(17.f, 0.f), Vec2(0.f, 0.f), tLength, Vec2(100.f, 0.f), Vec2(500.f, 50.f),
		tLength, pCamera);
	pScript->SetCameraGap(500.f);

	pLayerBack->AddGameObject(pBackGround);

	// TileMap Obj
	CGameObject* pTileMap = new CGameObject; pTileMap->SetName(L"TileMap0");
	pTileMap->AddComponent(new CTransform);
	pTileMap->AddComponent(new CMeshRender);
	pTileMap->AddComponent(new CTileMap2D);

	pTileMap->GetTransform()->SetPivot(0.f, 1.f);
	pTileMap->GetTransform()->SetLocalPos(100.f, -100.f, 900.f);
	pTileMap->GetTransform()->SetLocalScale(16.f * 100, 16.f * 40);

	/*
	
// Tile Shader
// g_vec2_0 : TILE X Y SIZE
// g_vec2_1 : TILE IMAGE SIZE (NOT ZERO)
// g_vec2_2 : TILE IMAGE INTERVAL 
// g_vec2_3 : TILEMAP TILE SIZE UV (Static)
// g_tex_0  : TILE TEXTURE
	*/

	pTileMap->GetMeshRender()->SetMesh(pRectMesh);
	pTileMap->GetMeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TileMtrl"));

	tLength = Vec2(16.f, 16.f);
	pTileMap->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_0, &tLength);

	tLength = Vec2(600.f, 1000.f);
	pTileMap->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_1, &tLength);

	tLength = Vec2(18.f, 18.f);
	pTileMap->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_2, &tLength);

	tLength = Vec2(1.f / 100.f, 1.f / 40.f);
	pTileMap->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_3, &tLength);

	// 기본값.


	pLayerBack->AddGameObject(pTileMap);

}

void CToolApp::PlayerCreate()
{
}

void CToolApp::ItemCreate()
{


	CScene* pScene = CSceneMgr::GetInst()->GetScene();

	wstring ItemPath = L"/Texture/Item/";

	// Texture
	Ptr<CTexture> pStoneAxe = CResMgr::GetInst()->Load<CTexture>(ItemPath + L"StoneAxe.png", ItemPath + L"StoneAxe.png");
	Ptr<CTexture> pCooperAxe = CResMgr::GetInst()->Load<CTexture>(ItemPath + L"CopperAxe.png", ItemPath + L"CopperAxe.png");




	CLayer* pLayer = pScene->SetLayerName(L"Item", 20);
	CGameObject* pPlayer = pScene->FindParentObj(L"Player");
	CGameObject* pItem = new CGameObject;

	Ptr<CMesh> pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Standard2DMtrl");
	Ptr<CTexture> pTexture = pCooperAxe;
	CItemScript* pItemScript = new CItemScript;
	pItem->SetName(L"CopperAxe");
	pItem->AddComponent(new CTransform);
	pItem->AddComponent(new CMeshRender);
	pItem->AddComponent(pItemScript);

	pItem->GetTransform()->SetLocalPos(RS_WIDTH / 2.f, -RS_HEIGHT / 2.f, 50.f);
	pItem->GetTransform()->SetLocalScale(32.f, 28.f, 1.f);
	pItem->GetTransform()->SetLocalRot(0.f, 0.f, 0.f);
	pItem->GetTransform()->EnableRotOffset();
	pItem->GetTransform()->SetPivot(0.f, 0.f);

	pItem->GetMeshRender()->SetMesh(pRectMesh);
	pItem->GetMeshRender()->SetMaterial(pMtrl->Clone());
	pItem->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, &pTexture);
	pItem->GetMeshRender()->SetEnable(false);

	pItemScript->SetType(ITEM_TYPE::AXE, ITEM::COPPER_AXE);

	pLayer->AddGameObject(pItem);

	// 플레이어 스크립트에 장비를 장착한다.
	CPlayerScript* pPlayerScript = (CPlayerScript*)pPlayer->FindScript(L"PlayerScript");

	pPlayerScript->SetHandleItem(pItemScript);

	CInventoryScript* pInven = new CInventoryScript;
	pPlayer->AddComponent(pInven);

	pPlayerScript->SetInventory(pInven);

	

}

BOOL CToolApp::OnIdle(LONG lCount)
{
	 // 데트타임에 호출되는 함수이다. : Client에서는 직접 메세지 루프를 가지고 DeadTime을 관리했다.
	// 하지만, MFC에서는 이것이 라이브러리 내부에 존재하기 때문에 직접 메세지 루프를 사용할 수 있는 형태가
	// 아니다. 따라서, DeadTime에 호출되는 가상함수인 OnIdle 함수를 재정의하여 여기서 게임의 Logic을 돌려준다.
	// 또한 다른 View들의 처리도 진행해주면 된다.

	CCore::GetInst()->Logic(); // GameView에서 돌고 있는 게임에 대한 처리.

	((CMainFrame*)m_pMainWnd)->Update(); // 그 외 다른 View에 대한 처리.

	// Tool에서 발생시킨 Event를 처리한다.
	CToolEventMgr::GetInst()->Update();

	return CWinApp::OnIdle(0); // 다음 데드타임까지의 대기 시간 zero
}

