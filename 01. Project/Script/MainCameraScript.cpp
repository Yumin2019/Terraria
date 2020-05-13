#include "stdafx.h"
#include "MainCameraScript.h"

#include <Engine/Scene.h>
#include <Engine/Layer.h>
#include <Engine/SceneMgr.h>
#include <Engine/Device.h>
#include <Engine/TileMap2D.h>

#include "PlayerScript.h"

Vec2 CMainCameraScript::g_vMapStart;
Vec2 CMainCameraScript::g_vMapEnd;

Vec3 CMainCameraScript::g_vMainCamPos;

CMainCameraScript::CMainCameraScript() :
	CScript((UINT)SCRIPT_TYPE::MAINCAMERASCRIPT),
	m_pPlayer(nullptr)
{
}


CMainCameraScript::~CMainCameraScript()
{
}

void CMainCameraScript::Awake()
{
	vector<CGameObject*> vecFind;
	CSceneMgr::GetInst()->FindObjByTag(L"Player", vecFind);

	if (vecFind.empty())
		assert(false);

	m_pPlayer = (CPlayerScript*)vecFind[0]->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT);

	if (m_pPlayer == nullptr)
		assert(false);

	Resolution tRes = CDevice::GetInst()->GetResolution();
	memcpy(&m_vRes, &tRes, sizeof(Vec2));

	vector<CGameObject*> vecTileMap;
	CScene* pScene = CSceneMgr::GetInst()->GetScene();

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecParent = pScene->GetLayer(i)->GetParentObjVec();

		for (int j = 0; j < vecParent.size(); ++j)
		{
			if (vecParent[j]->GetTileMap2D())
				vecTileMap.push_back(vecParent[j]);
		}
	}

	if (!vecTileMap.empty())
	{
		// 원점에서 거리가 가장 먼 것과 거리가 가장 작은 것으로 처리한다.
		float fMin = 9999.f, fMax = -1.f;
		int iMinIdx = 0, iMaxIdx = 0;

		for(int i = 0; i < vecTileMap.size(); ++i)
		{
			float fDist = Vec3::Distance(vecTileMap[i]->GetTransform()->GetLocalPos(), Vec3(100.f, 540.f, 0.f));

			if (fDist < fMin)
			{
				fMin = fDist;
				iMinIdx = i;
			}

			if (fDist > fMax)
			{
				fMax = fDist;
				iMaxIdx = i;
			}
		}

		// LT와 RB을 구한다.
		g_vMapStart = vecTileMap[iMinIdx]->GetTileMap2D()->GetTileLT(0, 0);
		g_vMapEnd = vecTileMap[iMaxIdx]->GetTileMap2D()->GetTileLT(99, 39)
			+ vecTileMap[iMaxIdx]->GetTileMap2D()->GetTileSize();
	}

	m_vRatio = Vec2(0.5f, 0.4f);
}

void CMainCameraScript::LateUpdate()
{
	if (m_pPlayer->GetTransform()->IsMoveX())
		return;

	Vec2 vPlayerPos = Vec2(m_pPlayer->GetTransform()->GetLocalPos());

	// 플레이어의 현재 위치를 중심으로 LT와 RB를 구한다.
	Vec2 vLeftTop = vPlayerPos;
	vLeftTop.x -= m_vRes.x * m_vRatio.x;
	vLeftTop.y += m_vRes.y * m_vRatio.y;

	Vec2 vRightBottom = vLeftTop;
	vRightBottom.x += m_vRes.x;
	vRightBottom.y -= m_vRes.y;

	// 위치 조정.
	if (vLeftTop.x < g_vMapStart.x)
	{
		vLeftTop.x = g_vMapStart.x;
		vRightBottom.x = vLeftTop.x + m_vRes.x;
	}
	else if (vRightBottom.x > g_vMapEnd.x)
	{
		vRightBottom.x = g_vMapEnd.x;
		vLeftTop.x = vRightBottom.x - m_vRes.x;
	}

	if (vLeftTop.y > g_vMapStart.y)
	{
		vLeftTop.y = g_vMapStart.y;
		vRightBottom.y = vLeftTop.y - m_vRes.y;
	}
	else if (vRightBottom.y < g_vMapEnd.y)
	{
		vRightBottom.y = g_vMapEnd.y;
		vLeftTop.y = vRightBottom.y + m_vRes.y;
	}

	// 조정한 위치를 셋팅.
	GetTransform()->SetLocalPos(vLeftTop + Vec2(m_vRes.x / 2.f, -m_vRes.y / 2.f));
	g_vMainCamPos = GetTransform()->GetLocalPos();
}
