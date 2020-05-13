// GameView.cpp: 구현 파일
//

#pragma once

#include "stdafx.h"
#include "Tool.h"
#include "GameView.h"
#include "ButtonView.h"

#include <Engine/Core.h>
#include <Engine/Device.h>

#include <Engine/GameObject.h>
#include <Engine/SceneMgr.h>
#include <Engine/Camera.h>
#include <Engine/Transform.h>
#include <Engine/TileMap2D.h>

#include "ToolCamScript.h"

#include "MainFrame.h"
#include "InfoView.h"

// CGameView

IMPLEMENT_DYNCREATE(CGameView, CView)

CGameView::CGameView() :
	m_pToolCam(nullptr),
	m_iIdxX(0),
	m_iIdxY(0),
	m_eDragType(DT_END),
	m_bViewInput(false)
{

}

CGameView::~CGameView()
{
	SAFE_DELETE(m_pToolCam);
}

bool CGameView::Init()
{
	m_pToolCam = new CGameObject;
	m_pToolCam->AddComponent(new CTransform);
	m_pToolCam->AddComponent(new CCamera);
	m_pToolCam->AddComponent(new CToolCamScript);
	m_pToolCam->GetCamera()->SetLayerCheckAll();

	m_pToolCam->GetTransform()->SetLocalPos(640.f, -360.f, 1.f);


	return true;
}

void CGameView::Update()
{
	// Update함수는 들어오니, mode를 보고, 처리를 하지 않도록 한다.
	if (CCore::GetInst()->GetSceneMode() == SCENE_MODE::PLAY)
		return;

	// Tool Camera에 대한 처리를 진행한다.

	if (m_bViewInput)
	{
		// Key 처리.
		m_pToolCam->Update();
	}

	m_pToolCam->LateUpdate();
	m_pToolCam->FinalUpdate();

	// ToolCamera 를 이용해 찍는다.
	m_pToolCam->GetCamera()->Render();

	// fps
	CTimeMgr::GetInst()->Render();

	// present
	CDevice::GetInst()->Present();
}

BEGIN_MESSAGE_MAP(CGameView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CGameView 그리기

void CGameView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CGameView 진단

#ifdef _DEBUG
void CGameView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif
#endif //_DEBUG

// CGameView 메시지 처리기
BOOL CGameView::PreCreatWindow(CREATESTRUCT & cs)
{
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;

	// 해당 자리의 비트가 아니라면, 그대로 남고 해당 자리는 무조건 0으로 기록된다.

	return CView::PreCreateWindow(cs);
}

#include "ButtonDlg.h"
#include "TileDlg.h"

#include <Script/TileScript.h>

#include <Engine/GameObject.h>
#include <Engine/Transform.h>
#include <Engine/MeshRender.h>
#include <Engine/Scene.h>
#include <Engine/Layer.h>
#include <Engine/SceneMgr.h>
#include <Script/ScriptMgr.h>

void CGameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bViewInput)
		return;

	// 타일 Dlg가 켜져 있지 않은 경우. 켜져 있는 경우.^

	// LButton을 눌러서 Object를 TargetObj를 선택하는 기능을 만든다.

	vector<CGameObject*> vecFind;
	Vec3 tPickPos = CSceneMgr::GetInst()->FindObjByPoint(point, vecFind, m_pToolCam->GetCamera());
	
	if (!CButtonDlg::g_bShowTile)
	{
	
		// 누른 위치의 점을 변환하여 Game의 Pos로 계산하고, 이것과 충돌이 되는 
		// 오브젝트를 가져온다.
		// 찾은 경우에 가장 위에 있는 오브젝트를 선택하고,
		// 아니라면 nullptr

		if (!vecFind.empty())
			((CInfoView*)(MAINFRAME->GetInfoView()))->SetTarget(vecFind[0]);
		else
			((CInfoView*)(MAINFRAME->GetInfoView()))->SetTarget(nullptr);

	}
	
	else if(!vecFind.empty())
	{

		// 클릭한 것이 TileMapScript를 가지고 있는지 확인해야 한다.
		// 상식적으로 가장 앞에 있는 것이 Tile일 것이다.
		// Tool에서 Script를 아는 것은 당연하지 않나 ? 
		// 아니야 어느 정도는 하드 코딩을 해야만 만들 수 있다.
		// 상용 엔진은 아니니까.

		// 교체 Component로.
		
		CTileMap2D* pTileMap = vecFind[0]->GetTileMap2D();

		if (pTileMap)
		{
			// TileMap 클릭 + TileDlg가 출력이 되고 있다.
			// 이 상태에서 선택한 인덱스를 구해야 한다. (클릭이라는 조건하에 인덱스는 벗어나지 않음)

			if (vecFind[0]->GetTransform()->GetPivot() != Vec2(0.f, 1.f))
			{
				// Pivot을 고려하지 않고, 왼쪽 상단이 아니면 처리하지 말자.
				MessageBox(nullptr, L"TileMap Offset Not 0 1", MB_OK);
				return;
			}

			// 왼쪽 상단을 기준으로 Position
			Vec3 tPos = vecFind[0]->GetTransform()->GetLocalPos();
			Vec3 tRelativePos = tPickPos - tPos; // PickPos 는 Tile에서의 클릭 위치이다. 이것을 토대로 상대적인 위치를 구한다.
			Vec3 tTileSize = vecFind[0]->GetTransform()->GetLocalScale() / Vec3(100.f, -40.f, 1.f); // World 에서 Tile하나의 크기.
			Vec3 tIdx = tRelativePos / tTileSize;
			int iIdx = (int)tIdx.y * 100 + (int)tIdx.x;

			if (iIdx < 0 || iIdx >= 4000)
				return;

			// TileDlg를 가져온다.
			CButtonDlg* pButtonDlg = ((CButtonView*)MAINFRAME->GetButtonView())->GetButtonDlg();
			CTileDlg* pTileDlg = pButtonDlg->GetTileDlg();

			// 해당 Idx에 타일을 셋팅한다. (Idx셋팅)
			pTileMap->SetIdx(iIdx, pTileDlg->GetTileIdxX(), pTileDlg->GetTileIdxY());
		}


	}

	CView::OnLButtonDown(nFlags, point);

	
}

void CGameView::OnLButtonUp(UINT nFlags, CPoint point)
{

	CView::OnLButtonUp(nFlags, point);
}


void CGameView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bViewInput)
		return;

	vector<CGameObject*> vecFind;
	Vec3 tPickPos = CSceneMgr::GetInst()->FindObjByPoint(point, vecFind, m_pToolCam->GetCamera());

	if (!vecFind.empty() && CButtonDlg::g_bShowTile)
	{
		
		if (vecFind[0]->GetTileMap2D())
		{
			// TileMap 클릭 + TileDlg가 출력이 되고 있다.
			// 이 상태에서 선택한 인덱스를 구해야 한다. (클릭이라는 조건하에 인덱스는 벗어나지 않음)

			if (vecFind[0]->GetTransform()->GetPivot() != Vec2(0.f, 1.f))
			{
				// Pivot을 고려하지 않고, 왼쪽 상단이 아니면 처리하지 말자.
				MessageBox(nullptr, L"TileMap Offset Not 0 1", MB_OK);
				return;
			}

			// 왼쪽 상단을 기준으로 Position
			Vec3 tPos = vecFind[0]->GetTransform()->GetLocalPos();
			Vec3 tRelativePos = tPickPos - tPos; // PickPos 는 Tile에서의 클릭 위치이다. 이것을 토대로 상대적인 위치를 구한다.
			Vec3 tTileSize = vecFind[0]->GetTransform()->GetLocalScale() / Vec3(100.f, -40.f, 1.f); // World 에서 Tile하나의 크기.
			Vec3 tIdx = tRelativePos / tTileSize;
			int iIdx = (int)tIdx.y * 100 + (int)tIdx.x;


			if (iIdx < 0 || iIdx >= 4000)
				return;

			m_eDragType = DT_BEGIN;
			m_iIdxX = tIdx.x;
			m_iIdxY = tIdx.y;

		}


	}


	CView::OnRButtonDown(nFlags, point);
}


void CGameView::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (!m_bViewInput)
		return;


	if (m_eDragType != DT_BEGIN)
		return;

	// 타일 Dlg가 켜져 있지 않은 경우. 켜져 있는 경우.

	// LButton을 눌러서 Object를 TargetObj를 선택하는 기능을 만든다.

	vector<CGameObject*> vecFind;
	Vec3 tPickPos = CSceneMgr::GetInst()->FindObjByPoint(point, vecFind, m_pToolCam->GetCamera());

	if (!vecFind.empty() && CButtonDlg::g_bShowTile)
	{
		CTileMap2D* pTileMap = vecFind[0]->GetTileMap2D();

		if (pTileMap)
		{
			// TileMap 클릭 + TileDlg가 출력이 되고 있다.
			// 이 상태에서 선택한 인덱스를 구해야 한다. (클릭이라는 조건하에 인덱스는 벗어나지 않음)

			if (vecFind[0]->GetTransform()->GetPivot() != Vec2(0.f, 1.f))
			{
				// Pivot을 고려하지 않고, 왼쪽 상단이 아니면 처리하지 말자.
				MessageBox(nullptr, L"TileMap Offset Not 0 1", MB_OK);
				return;
			}

			// 왼쪽 상단을 기준으로 Position
			Vec3 tPos = vecFind[0]->GetTransform()->GetLocalPos();
			Vec3 tRelativePos = tPickPos - tPos; // PickPos 는 Tile에서의 클릭 위치이다. 이것을 토대로 상대적인 위치를 구한다.
			Vec3 tTileSize = vecFind[0]->GetTransform()->GetLocalScale() / Vec3(100.f, -40.f, 1.f); // World 에서 Tile하나의 크기.
			Vec3 tIdx = tRelativePos / tTileSize;
			int iIdx = (int)tIdx.y * 100 + (int)tIdx.x;

			if (iIdx < 0 || iIdx >= 4000)
				return;

			m_eDragType = DT_END;
			int iEndX = tIdx.x;
			int iEndY = tIdx.y;

			// TileDlg를 가져온다.
			CButtonDlg* pButtonDlg = ((CButtonView*)MAINFRAME->GetButtonView())->GetButtonDlg();
			CTileDlg* pTileDlg = pButtonDlg->GetTileDlg();

			// 시작 인덱스부터 끝 인덱스까지 처리를 해주자.
			int iTileIdxX = pTileDlg->GetTileIdxX();
			int iTileIdxY = pTileDlg->GetTileIdxY();

			for (int y = m_iIdxY; y <= iEndY; ++y)
			{
				for (int x = m_iIdxX; x <= iEndX; ++x)
				{
					int idx = y * 100 + x;
					pTileMap->SetIdx(idx, iTileIdxX, iTileIdxY);
				}
			}


		}


	}


	CView::OnRButtonUp(nFlags, point);
}
