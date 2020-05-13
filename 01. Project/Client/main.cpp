
#include "stdafx.h"
#include "main.h"


// 라이브러리에서 사용하는 헤더파일을 참조한다.
#include <Engine/Game.h>
#include <Engine/Core.h>
#include <Engine/PathMgr.h>
#include <Engine/Scene.h>
#include <Engine/Layer.h>
#include <Engine/GameObject.h>
#include <Engine/Animator2D.h>
#include <Engine/TileMap2D.h>
#include <Engine/EventMgr.h>
#include <Engine/FontMgr.h.>

#include <Script/SaveLoadMgr.h>
#include <Script/PoolMgr.h>
#include <Script/TileMapMgr.h>
#include <Script/UtilMgr.h>
#include <Script/PrototypeMgr.h>

// 모드에 따라서 다른 라이브러리를 참조해야 하므로 전처리기를 사용하여 라이브러리를 연결한다.
#ifdef _DEBUG
#pragma comment(lib, "Engine/Engine_Debug")
#else
#pragma comment(lib, "Engine/Engine")
#endif

#ifdef _DEBUG
#pragma comment(lib, "Script/Script_Debug.lib")
#else
#pragma comment(lib, "Script/Script.lib")
#endif


enum class TILE
{
	NONE,
	DIRT,
	STONE,
	WOOD,
	WOOD_WALL,
	CACTUS,
	END
};

TileInfo	g_tTileInfo[(UINT)TILE::END];
TILE*	    g_pTileOption;

// TILEINFO에서 사용한다.
#define IMAGE_NUM_X 32
#define IMAGE_NUM_Y 54

#define MAX_LOADSTRING 100

void GameInit();
void Destroy();
void TileOptionLoad();


// 전역 변수:
HWND g_hWnd;
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	GameInit();

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
			if (msg.message == WM_QUIT)
				break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		else
		{
			// DeadTime.

			CCore::GetInst()->Logic();
			
		}
    }

    return (int) msg.wParam;
}



void GameInit()
{

	// Engine Init
	if (!CCore::GetInst()->Init(g_hWnd, Resolution{ 1120, 630 }, true))
	{
		assert(false);
		return;
	}

	// Client에 맞게 작업영역을 잡는다.
	CCore::GetInst()->ChangeWindowSize(g_hWnd);

	CFontMgr::GetInst()->AddFont(L"Andy"); // 0번.

	// Scene Load
	wstring strPath = CPathMgr::GetResPath();
	strPath += L"Scene/testScene.scene";
	CSaveLoadMgr::LoadScene(strPath);



	// Script Init
	CScene* pScene = CSceneMgr::GetInst()->GetScene();
	CCore::GetInst()->SetSceneMode(SCENE_MODE::PLAY);

	// 모든 Object Enable = false
	for (size_t i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pScene->GetLayer(i);
		const vector<CGameObject*>& vecObj = pLayer->GetParentObjVec();

		for (size_t j = 0; j < vecObj.size(); ++j)
			vecObj[j]->SetEnable(false);
	}

	// Title Layer만 다시 Enable = true
	CLayer* pTitleLayer = pScene->FindLayer(L"Title");
	const vector<CGameObject*>& vecTitle = pTitleLayer->GetParentObjVec();

	for (size_t i = 0; i < vecTitle.size(); ++i)
		vecTitle[i]->SetEnable(true);

	// 모든 애니메이션 처리 시작
	for (size_t i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pScene->GetLayer(i);
		const vector<CGameObject*>& vecObj = pLayer->GetParentObjVec();

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			if (vecObj[j]->GetAnimator2D())
			{
				vecObj[j]->GetAnimator2D()->SetPause(false);
			}
		}
	}

	TileOptionLoad();

	// TileInfo 셋팅.
	// 옵션값 셋팅하는 부분이다.  
	CLayer* pLayer = pScene->GetLayer(TILE_LAYER_IDX);
	const vector<CGameObject*>& vecObj = pLayer->GetParentObjVec(); // TileObject를 얻어온다.
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

			TILE eTileOption = g_pTileOption[iTileIdx];

			// 이렇게 얻은 Option을 토대로, TileInfo를 얻는다.
			// 이것을 TileMap의 정보로 셋팅한다.

			pTileMap->SetTileInfo(iIdx, g_tTileInfo[(UINT)eTileOption]);

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
}

void TileOptionLoad()
{
	int m_iTileNum = IMAGE_NUM_X * IMAGE_NUM_Y;
	g_pTileOption = new TILE[m_iTileNum];

	for (UINT i = 0; i < m_iTileNum; ++i)
	{
		g_pTileOption[i] = TILE::NONE;
	}

	wstring strPath = CPathMgr::GetResPath();
	strPath += L"Scene/TileOption.ot";

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strPath.c_str(), L"rb");

	if (pFile)
	{
		// 해당 정보를 순회하면서 읽는다.
		fread(g_pTileOption, 4, m_iTileNum, pFile);
		fclose(pFile);
	}


	// Digable, block, DROP ITEM
	g_tTileInfo[(UINT)TILE::NONE] = TileInfo(false, false, ITEM::NONE);
	g_tTileInfo[(UINT)TILE::DIRT] = TileInfo(true, true, ITEM::DIRT);
	g_tTileInfo[(UINT)TILE::STONE] = TileInfo(true, true, ITEM::STONE);
	g_tTileInfo[(UINT)TILE::WOOD] = TileInfo(true, true, ITEM::WOOD);
	g_tTileInfo[(UINT)TILE::WOOD_WALL] = TileInfo(true, false, ITEM::WOOD_WALL);
	g_tTileInfo[(UINT)TILE::CACTUS] = TileInfo(true, false, ITEM::CACTUS);


	for (UINT i = 0; i < (UINT)TILE::END; ++i)
		g_tTileInfo[i].m_bNatural = true;
}

void Destroy()
{
	SAFE_DELETE_ARRAY(g_pTileOption);

	// Prototype -> Core
	DESTROY_SINGLE(CPrototypeMgr);
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = nullptr; // MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, false);
   UpdateWindow(g_hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		Destroy();
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
