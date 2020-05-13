#include "stdafx.h"
#include "Core.h"
#include "Device.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "PathMgr.h"
#include "ResMgr.h"
#include "SceneMgr.h"
#include "RenderMgr.h"
#include "CollisionMgr.h"
#include "EventMgr.h"

#include "FontMgr.h"
#include <time.h>

#define TILE_NUMX 100
#define TILE_NUMY 40

CCore::CCore() :
	m_hWnd(nullptr) 
	
{
	// Handle은 윈도우를 가리키기 위한 값으로 32bit 변수이다. BRUSH와 PEN 처럼 다른 핸들의 경우에는 중복된 값을 가질 수
	// 있고, 같은 핸들의 경우에는 중복된 값을 가지지 않는다.

	srand((unsigned int)time(0));
}


CCore::~CCore()
{
	/*
	static 변수를 이용한 Sigleton 이라서, 프로그램 종료시 메모리에서 소멸.
	*/

	/*
	소멸 순서가 정해져야 하는 Single Ton 
	SceneMgr(오브젝트 삭제시 스마트 포인터에 의한 Release) -> ResMgr
	*/

	DESTROY_SINGLE(CSceneMgr);
	DESTROY_SINGLE(CResMgr);
}

bool CCore::Init(HWND hWnd, const Resolution & tResolution, bool bWindow)
{
	m_hWnd = hWnd;
	ShowWindow(m_hWnd, true);

	m_tRes = tResolution;
	// 생성을 하고 윈도우의 사이즈를 변경한다. 따라서, 사이즈를 바꾼 이후에 윈도우를 보여주도록 한다.

	// Device  정보를 초기화해준다.
	if (!GET_SINGLE(CDevice)->Init(hWnd, tResolution, bWindow))
		return false;


	// Constant Buffer 생성
	GET_SINGLE(CDevice)->CreateConstBuffer(L"TransformMatrix", sizeof(Transform), 0); // 행렬 정보
	GET_SINGLE(CDevice)->CreateConstBuffer(L"MaterialParameter", sizeof(MtrlParam), 1); // 재질 정보
	GET_SINGLE(CDevice)->CreateConstBuffer(L"Anim2D", sizeof(Ani2D), 2); // Animation 정보

	// Light2DInfo * 100 과 16byte의 사이즈의 상수버퍼를 만들어둔다.
	GET_SINGLE(CDevice)->CreateConstBuffer(L"Light2D", sizeof(Light2DInfo) * 100 + 16, 3); 

	//  BackGround 상수버퍼.
	GET_SINGLE(CDevice)->CreateConstBuffer(L"BackGround", sizeof(BackGround), 4);

	// Tile 상수버퍼
	GET_SINGLE(CDevice)->CreateConstBuffer(L"TileBuffer", sizeof(TileBuffer) * TILE_NUMX * TILE_NUMY, 5);

	// Panel
	GET_SINGLE(CDevice)->CreateConstBuffer(L"PanelBuffer", sizeof(_tagPanelBuffer) * 50, 6);

	// Global Value
	GET_SINGLE(CDevice)->CreateConstBuffer(L"GlobalValue", sizeof(GlobalValue), 7);

	g_tValue.g_vResolution.x = tResolution.fWidth;
	g_tValue.g_vResolution.y = tResolution.fHeight;
	g_tValue.g_iMorning = 1;
	g_tValue.g_fRatio = 0.f;

	g_tTransform.matWorld = XMMatrixIdentity();
	g_tTransform.matView = XMMatrixIdentity();
	g_tTransform.matProj = XMMatrixIdentity();



	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(914);

	// Init
	GET_SINGLE(CFontMgr)->Init();
	GET_SINGLE(CRenderMgr)->Init();
	CPathMgr::Init();
	GET_SINGLE(CKeyMgr)->Init();
	GET_SINGLE(CTimeMgr)->Init();
	GET_SINGLE(CResMgr)->Init();
	GET_SINGLE(CCollisionMgr)->Init();
	GET_SINGLE(CSceneMgr)->Init();


	return true;
}

void CCore::Logic()
{
	CTimeMgr::GetInst()->Update();

	CKeyMgr::GetInst()->Update();

	// 다음 프레임에서 이벤트 삭제. : 처음 Awake, Start 시점의 Event는 무시된다.
	CEventMgr::GetInst()->Clear();
	{
		// Event 발생.
		(CSceneMgr::GetInst()->*m_pUpdateFunc)();
		(CRenderMgr::GetInst()->*m_pRenderFunc)();
	}
	// Event 실행 및 Tool 연동.
	CEventMgr::GetInst()->Update();
}

void CCore::SetSceneMode(SCENE_MODE eMode)
{
	m_eMode = eMode;
	if (m_eMode == SCENE_MODE::PLAY)
	{
		m_pUpdateFunc = &CSceneMgr::Update;
		m_pRenderFunc = &CRenderMgr::Render;
	}
	else
	{
		m_pUpdateFunc = &CSceneMgr::UpdateTool;
		m_pRenderFunc = &CRenderMgr::RenderTool;
	}
}

void CCore::ChangeWindowSize(HWND hWnd)
{
	RECT tRect = { 0, 0, (int)m_tRes.fWidth, (int)m_tRes.fHeight };

	AdjustWindowRect(&tRect, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(m_hWnd, nullptr, 0, 0, tRect.right - tRect.left, tRect.bottom - tRect.top, 0);

}



