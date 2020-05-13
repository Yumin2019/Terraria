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
	// Handle�� �����츦 ����Ű�� ���� ������ 32bit �����̴�. BRUSH�� PEN ó�� �ٸ� �ڵ��� ��쿡�� �ߺ��� ���� ���� ��
	// �ְ�, ���� �ڵ��� ��쿡�� �ߺ��� ���� ������ �ʴ´�.

	srand((unsigned int)time(0));
}


CCore::~CCore()
{
	/*
	static ������ �̿��� Sigleton �̶�, ���α׷� ����� �޸𸮿��� �Ҹ�.
	*/

	/*
	�Ҹ� ������ �������� �ϴ� Single Ton 
	SceneMgr(������Ʈ ������ ����Ʈ �����Ϳ� ���� Release) -> ResMgr
	*/

	DESTROY_SINGLE(CSceneMgr);
	DESTROY_SINGLE(CResMgr);
}

bool CCore::Init(HWND hWnd, const Resolution & tResolution, bool bWindow)
{
	m_hWnd = hWnd;
	ShowWindow(m_hWnd, true);

	m_tRes = tResolution;
	// ������ �ϰ� �������� ����� �����Ѵ�. ����, ����� �ٲ� ���Ŀ� �����츦 �����ֵ��� �Ѵ�.

	// Device  ������ �ʱ�ȭ���ش�.
	if (!GET_SINGLE(CDevice)->Init(hWnd, tResolution, bWindow))
		return false;


	// Constant Buffer ����
	GET_SINGLE(CDevice)->CreateConstBuffer(L"TransformMatrix", sizeof(Transform), 0); // ��� ����
	GET_SINGLE(CDevice)->CreateConstBuffer(L"MaterialParameter", sizeof(MtrlParam), 1); // ���� ����
	GET_SINGLE(CDevice)->CreateConstBuffer(L"Anim2D", sizeof(Ani2D), 2); // Animation ����

	// Light2DInfo * 100 �� 16byte�� �������� ������۸� �����д�.
	GET_SINGLE(CDevice)->CreateConstBuffer(L"Light2D", sizeof(Light2DInfo) * 100 + 16, 3); 

	//  BackGround �������.
	GET_SINGLE(CDevice)->CreateConstBuffer(L"BackGround", sizeof(BackGround), 4);

	// Tile �������
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

	// ���� �����ӿ��� �̺�Ʈ ����. : ó�� Awake, Start ������ Event�� ���õȴ�.
	CEventMgr::GetInst()->Clear();
	{
		// Event �߻�.
		(CSceneMgr::GetInst()->*m_pUpdateFunc)();
		(CRenderMgr::GetInst()->*m_pRenderFunc)();
	}
	// Event ���� �� Tool ����.
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



