#include "stdafx.h"
#include "MouseScript.h"
#include "PlayerScript.h"

#include "PrototypeMgr.h"

#include "ItemScript.h"
#include "UIItemScript.h"
#include "InventoryScript.h"
#include "CraftingScript.h"
#include "SelectScript.h"
#include "TileMapMgr.h"
#include "PoolMgr.h"
#include "ParticleScript.h"
#include "MonsterScript.h"
#include "ItemObjScript.h"

#include <Engine/Device.h>
#include <Engine/CollisionMgr.h>
#include <Engine/TileMap2D.h>

/*
	switch (eItem)
	{
	case ITEM::TORCH:					break;
	case ITEM::WHITE_TORCH:				break;
	case ITEM::VIOLET_TORCH:			break;
	case ITEM::YELLOW_TORCH:			break;
	case ITEM::BLUE_TORCH:				break;
	case ITEM::GREEN_TORCH:				break;
	case ITEM::RED_TORCH:				break;
	case ITEM::ORANGE_TORCH:			break;
	case ITEM::PINK_TORCH:				break;
	case ITEM::ICE_TORCH:				break;
	}
*/


bool CMouseScript::g_bUIColl = false;
CTileMap2D* CMouseScript::g_pTileMap = nullptr;

#define TORCH_START_X 23
#define TORCH_START_Y 14
#define TORCH_UP 0
#define TORCH_RIGHT 1
#define TORCH_LEFT 2

#define WOOD_DOOR_START_X 13
#define WOOD_DOOR_START_Y 24

#define CACTUS_DOOR_START_X 13
#define CACTUS_DOOR_START_Y 27

#define NONE_TILE_X 9
#define NONE_TILE_Y 4

CMouseScript::CMouseScript() :
	CScript((UINT)SCRIPT_TYPE::MOUSESCRIPT),
	m_bShow(true),
	m_bDig(false),
	m_pTileObj(nullptr),
	m_pPlayer(nullptr),
	m_pItemObj(nullptr),
	m_pMouseUIObj(nullptr),
	m_pSelectTile(nullptr),
	m_pSelect(nullptr),
	m_bDigFirst(false),
	m_iColX(50),
	m_iColY(10),
	m_fDigLimitTime(0.2f),
	m_fDigTime(0.f),
	m_fDigSound(0.f),
	m_fDigSoundLimitTime(0.2f / 3.f),
	m_iLayerCheck(0),
	m_iDig(0)
{
}


CMouseScript::~CMouseScript()
{
}


void CMouseScript::SetTileIdx(CTileMap2D * pTileMap, int iX, int iY, ITEM eItem)
{
	int iIdx = iY * 100 + iX;

	switch (eItem)
	{
	case ITEM::DIRT:
	case ITEM::STONE:
	case ITEM::WOOD:
	case ITEM::CACTUS:
		pTileMap->SetTileInfo(iIdx, TileInfo(true, true, eItem, false));
		break;

	case ITEM::CACTUS_WALL:
	case ITEM::STONE_WALL:
	case ITEM::DIRT_WALL:
	case ITEM::WOOD_WALL:
		pTileMap->SetTileInfo(iIdx, TileInfo(true, false, eItem, false));


		break;

	case ITEM::NONE:
		pTileMap->SetTileInfo(iIdx, TileInfo(false, false, eItem));
		break;

	case ITEM::WORKBENCH:
	case ITEM::CACTUS_WORKBENCH:
		assert(false);
		break;
	}

	// 자기 자신. (참고로 Torch는 들어오지 않는다) 무언가 설치.
	ChangeTileIdx(pTileMap, iX, iY);

	// 주변 타일들
	Index arrIdx[4] = { Index(iX - 1, iY), Index(iX, iY - 1), Index(iX + 1, iY), Index(iX, iY + 1) };

	for (int i = 0; i < 4; ++i)
	{
		if (pTileMap->IsTileIdx(arrIdx[i].iX, arrIdx[i].iY))
		{

			switch (GetItemType(pTileMap->GetTileItem(arrIdx[i].iX, arrIdx[i].iY)))
			{
			case ITEM_TYPE::TORCH:
				ChangeTileTorch(pTileMap, arrIdx[i].iX, arrIdx[i].iY, pTileMap->GetTileItem(arrIdx[i].iX, arrIdx[i].iY));

				break;

			default:
				ChangeTileIdx(pTileMap, arrIdx[i].iX, arrIdx[i].iY);

				break;
			}
		}
	}
}

void CMouseScript::SetTorchIdx(CTileMap2D * pTileMap, int iX, int iY, ITEM eItem)
{
	bool bBuild = ChangeTileTorch(pTileMap, iX, iY, eItem);

	if (bBuild == false)
		return;

	Vec2 vCenter = pTileMap->GetTileLT(iX, iY);
	vCenter += Vec2(8.f, -8.f);

	// Torch Tile의 위치에 
	CGameObject* pLightObj = CPrototypeMgr::GetInst()->CreateCloneObj(L"LightObj", L"Light");
	pLightObj->GetTransform()->SetLocalPos(vCenter);

	Vec3 vColor;



	switch (eItem)
	{
	case ITEM::TORCH:
	case ITEM::WHITE_TORCH:
		vColor = Vec3(1.f, 1.f, 1.f); //

		break;

	case ITEM::VIOLET_TORCH:
		vColor = Vec3(185.f / 255.f, 115.f / 255.f, 255.f / 255.f); //


		break;
	case ITEM::YELLOW_TORCH:
		vColor = Vec3(1.f, 1.f, 0.4f); //


		break;
	case ITEM::BLUE_TORCH:
		vColor = Vec3(0.3f, 1.f, 1.f);//


		break;
	case ITEM::GREEN_TORCH:
		vColor = Vec3(0.5f, 1.f, 0.5f); //


		break;
	case ITEM::RED_TORCH:
		vColor = Vec3(1.f, 0.015f, 0.015f); //


		break;
	case ITEM::ORANGE_TORCH:
		vColor = Vec3(1.f, 0.5f, 0.f); //


		break;
	case ITEM::PINK_TORCH:
		vColor = Vec3(1.f, 0.4f, 1.f);//

		break;

	case ITEM::ICE_TORCH:
		vColor = Vec3(0.5f, 1.f, 1.f); //

		break;
	
	default:
		assert(false);
		break;
	}

	pLightObj->GetLight2D()->SetLightColor(vColor);

}

void CMouseScript::ChangeTileIdx(CTileMap2D * pTileMap, int iX, int iY)
{
	int iTileTypeX = 0;
	int iTileTypeY = 0;
	int iIdx = iY * 100 + iX;
	ITEM eItem = pTileMap->GetTileItem(iIdx);
	bool bWall = false;

	switch (eItem)
	{
	case ITEM::DIRT:
		iTileTypeX = DIRT_TILE_START_X;
		iTileTypeY = DIRT_TILE_START_Y;
		break;

	case ITEM::STONE:

		iTileTypeX = STONE_TILE_START_X;
		iTileTypeY = STONE_TILE_START_Y;
		break;

	case ITEM::WOOD:

		iTileTypeX = WOOD_TILE_START_X;
		iTileTypeY = WOOD_TILE_START_Y;
		break;

	case ITEM::CACTUS:

		iTileTypeX = CACTUS_TILE_START_X;
		iTileTypeY = CACTUS_TILE_START_Y;
		break;

// WALL TILE
	case ITEM::WOOD_WALL:
		bWall = true;
		iTileTypeX = WOOD_WALL_TILE_START_X;
		iTileTypeY = WOOD_WALL_TILE_START_Y;
		break;

	case ITEM::STONE_WALL:
		bWall = true;
		iTileTypeX = STONE_WALL_TILE_START_X;
		iTileTypeY = STONE_WALL_TILE_START_Y;
		break;

	case ITEM::CACTUS_WALL:
		bWall = true;
		iTileTypeX = CACTUS_WALL_TILE_START_X;
		iTileTypeY = CACTUS_WALL_TILE_START_Y;
		break;

	case ITEM::DIRT_WALL:
		bWall = true;
		iTileTypeX = DIRT_WALL_TILE_START_X;
		iTileTypeY = DIRT_WALL_TILE_START_Y;
		break;



	case ITEM::NONE:
		pTileMap->SetIdx(iIdx, NONE_TILE_X, NONE_TILE_Y);
		return;

	case ITEM::WORKBENCH:
	case ITEM::CACTUS_WORKBENCH:
		return;
	}


	// LTRB 순서이다.
	int arrIdx[4] = { (iY) * 100 + (iX - 1) ,
					  (iY - 1) * 100 + (iX),
					  (iY) * 100 + (iX + 1) ,
					  (iY + 1) * 100 + (iX) };

	bool arrBlock[4] = {};
	int iBlockNum = 0;


	if (bWall)
	{
		// 벽의 경우에는 빈 타일이 아니라면 무조건 개수를 센다.
		for (int i = 0; i < 4; ++i)
		{
			if (pTileMap->IsTileIdx(arrIdx[i]) && 
				pTileMap->GetTileInfo(arrIdx[i]).m_eItem != ITEM::NONE)
			{
				arrBlock[i] = true;
				++iBlockNum;
			}

		}

		// 벽 타일에 대해서 처리한다.

		switch (iBlockNum)
		{
		case 0:
		case 4:
			pTileMap->SetIdx(iIdx, iTileTypeX + 1, iTileTypeY + 1);
			break;

		case 1:

			if (arrBlock[0])
			{
				pTileMap->SetIdx(iIdx, iTileTypeX + 3, iTileTypeY + 0);
			}
			else if (arrBlock[1])
			{
				pTileMap->SetIdx(iIdx, iTileTypeX + 3, iTileTypeY + 3);

			}
			else if (arrBlock[2])
			{
				pTileMap->SetIdx(iIdx, iTileTypeX + 0, iTileTypeY + 0);

			}
			else
			{
				pTileMap->SetIdx(iIdx, iTileTypeX + 3, iTileTypeY + 0);
			}

			break;

		case 2:
			//  1
			// 0■2
			//  3

			if (arrBlock[0])
			{
				// 0번이 블록인데... n번이 블록이다.

				if (arrBlock[1])
				{
					// 1번. 
					pTileMap->SetIdx(iIdx, iTileTypeX + 3, iTileTypeY + 3);

				}
				else if (arrBlock[2])
				{
					// 2번. 
					pTileMap->SetIdx(iIdx, iTileTypeX + 1, iTileTypeY + 1);

				}
				else
				{
					// 3번. 1 3
					pTileMap->SetIdx(iIdx, iTileTypeX + 3, iTileTypeY + 0);

				}
			}
			else
			{
				if (arrBlock[1] && arrBlock[3])
				{
					pTileMap->SetIdx(iIdx, iTileTypeX + 1, iTileTypeY + 1);

				}
				else if (arrBlock[1] && arrBlock[2])
				{
					pTileMap->SetIdx(iIdx, iTileTypeX + 0, iTileTypeY + 3);

				}
				else if (arrBlock[2] && arrBlock[3])
				{
					pTileMap->SetIdx(iIdx, iTileTypeX + 0, iTileTypeY + 0);
				}
			}

			break;

		case 3:

			if (arrBlock[0] == false)
			{
				// L이 빈 경우. 
				pTileMap->SetIdx(iIdx, iTileTypeX + 0, iTileTypeY + 1);

			}
			else if (arrBlock[1] == false)
			{
				// T가 빈 경우.
				pTileMap->SetIdx(iIdx, iTileTypeX + 1, iTileTypeY + 0);
			}
			else if (arrBlock[2] == false)
			{
				// R이 빈 경우. 
				pTileMap->SetIdx(iIdx, iTileTypeX + 3, iTileTypeY + 1);

			}
			else
			{
				// B가 빈 경우 
				pTileMap->SetIdx(iIdx, iTileTypeX + 1, iTileTypeY + 3);

			}


			break;

	
		}
	}
	else
	{
		// 벽이 아닌 기본 Tile에 대해서 처리한다.

		for (int i = 0; i < 4; ++i)
		{
			if (pTileMap->IsTileIdx(arrIdx[i]) && pTileMap->GetTileInfo(arrIdx[i]).m_bBlock)
			{
				arrBlock[i] = true;
				++iBlockNum;
			}

			// 인덱스가 넘어갔으면 반드시 false.
			// 블럭인지에 따라서 해당 자리에 flag를 저장.
		}

		// 주변에 블럭이 없다.
		switch (iBlockNum)
		{
		case 0:
			// 9 3
			pTileMap->SetIdx(iIdx, iTileTypeX + 9, iTileTypeY + 3);
			break;

		case 1:

			if (arrBlock[0])
			{
				// L 이 블럭. 12 0
				pTileMap->SetIdx(iIdx, iTileTypeX + 12, iTileTypeY + 0);

			}
			else if (arrBlock[1])
			{
				// T이 블럭 6 3
				pTileMap->SetIdx(iIdx, iTileTypeX + 6, iTileTypeY + 3);

			}
			else if (arrBlock[2])
			{
				// R 이 블럭 9 0
				pTileMap->SetIdx(iIdx, iTileTypeX + 9, iTileTypeY + 0);

			}
			else
			{
				// B가 블럭. 6 0
				pTileMap->SetIdx(iIdx, iTileTypeX + 6, iTileTypeY + 0);
			}


			break;

		case 2:
			//  1
			// 0■2
			//  3

			if (arrBlock[0])
			{
				// 0번이 블록인데... n번이 블록이다.

				if (arrBlock[1])
				{
					// 1번. 1 4
					pTileMap->SetIdx(iIdx, iTileTypeX + 1, iTileTypeY + 4);

				}
				else if (arrBlock[2])
				{
					// 2번. 6 4
					pTileMap->SetIdx(iIdx, iTileTypeX + 6, iTileTypeY + 4);

				}
				else
				{
					// 3번. 1 3
					pTileMap->SetIdx(iIdx, iTileTypeX + 1, iTileTypeY + 3);

				}
			}
			else
			{
				if (arrBlock[1] && arrBlock[3])
				{
					pTileMap->SetIdx(iIdx, iTileTypeX + 5, iTileTypeY + 0);

				}
				else if (arrBlock[1] && arrBlock[2])
				{
					// 0 4
					pTileMap->SetIdx(iIdx, iTileTypeX + 0, iTileTypeY + 4);

				}
				else if (arrBlock[2] && arrBlock[3])
				{
					pTileMap->SetIdx(iIdx, iTileTypeX + 0, iTileTypeY + 3);
					// 0 3
				}
			}

			break;

		case 3:

			if (arrBlock[0] == false)
			{
				// L이 빈 경우. 4 5
				pTileMap->SetIdx(iIdx, iTileTypeX + 0, iTileTypeY + 0);

			}
			else if (arrBlock[1] == false)
			{
				// T가 빈 경우.0 11
				pTileMap->SetIdx(iIdx, iTileTypeX + 1, iTileTypeY + 0);
			}
			else if (arrBlock[2] == false)
			{
				// R이 빈 경우. 5 5
				pTileMap->SetIdx(iIdx, iTileTypeX + 4, iTileTypeY + 0);

			}
			else
			{
				// B가 빈 경우 0 12
				pTileMap->SetIdx(iIdx, iTileTypeX + 1, iTileTypeY + 2);

			}


			break;

		case 4:
			pTileMap->SetIdx(iIdx, iTileTypeX + 6, iTileTypeY + 1);

			break;
		}
	}
	
}

bool CMouseScript::ChangeTileTorch(CTileMap2D * pTileMap, int iX, int iY, ITEM eItem)
{
	// TileMap, iX, iY

	int iIdx = iY * 100 + iX;

	int iYAdd = 0;

	switch (eItem)
	{
	case ITEM::TORCH:			iYAdd = 0;			break;
	case ITEM::WHITE_TORCH:		iYAdd = 5;			break;
	case ITEM::VIOLET_TORCH:	iYAdd = 4;			break;
	case ITEM::YELLOW_TORCH:	iYAdd = 6;			break;
	case ITEM::BLUE_TORCH:		iYAdd = 1;			break;
	case ITEM::GREEN_TORCH:		iYAdd = 3;			break;
	case ITEM::RED_TORCH:		iYAdd = 2;			break;
	case ITEM::ORANGE_TORCH:	iYAdd = 10;			break;
	case ITEM::PINK_TORCH:		iYAdd = 15;			break;
	case ITEM::ICE_TORCH:		iYAdd = 1;			break;
	}

	// 주변 타일들 LTRB
	int arrIdx[4] =
	{
		iIdx - 1, iIdx - 100, iIdx + 1, iIdx + 100
	};

	bool bBlock[4] = {};
	int iBlockNum = 0;

	for (int i = 0; i < 4; ++i)
	{
		if (pTileMap->IsTileIdx(arrIdx[i]) &&
			pTileMap->IsTileBlock(arrIdx[i]))
		{
			// 제대로 된 인덱스 + 설치할 수 있는 블럭.
			bBlock[i] = true;
			++iBlockNum;
		}
	}

	// 블럭의 개수로 나눠서 처리하자.

	switch (iBlockNum)
	{
	case 0:

		// 주변 타일을 지우다가 자신만 남은 경우 : 아이템으로 생성해버리자.
		DigOneBlockItem(pTileMap, iX, iY, eItem);

		return false;

	case 1:

		// 설치불가(dig = false) or 블럭 캔 처리(dig = true)
		if (bBlock[1])
		{
			if (m_bDig)
				DigOneBlockItem(pTileMap, iX, iY, eItem); // 다른 블럭을 캤다. 영향을 받고... 사라진다.
			else
				CInventoryScript::AddMtrlCount(eItem); // 설치를 하다가 잘못된 위치에 설치.

			return false;
		}

		// L T R B

		if (bBlock[0]) // L
		{
			// 오른쪽을 보도록.
			pTileMap->SetIdx(iIdx, TORCH_START_X + TORCH_RIGHT, TORCH_START_Y + iYAdd);
		}
		else if (bBlock[2]) // R
		{
			pTileMap->SetIdx(iIdx, TORCH_START_X + TORCH_LEFT, TORCH_START_Y + iYAdd);

		}
		else if (bBlock[3]) // B
		{
			pTileMap->SetIdx(iIdx, TORCH_START_X + TORCH_UP, TORCH_START_Y + iYAdd);
		}
		break;

	case 2:
		//   1
		// 0ㅁ2
		//   3

		// Top을 포함하느냐 안 하느냐로 나눈다.
		if (bBlock[1])
		{
			if (bBlock[0])
			{
				pTileMap->SetIdx(iIdx, TORCH_START_X + TORCH_RIGHT, TORCH_START_Y + iYAdd);
			}
			else if (bBlock[2])
			{
				pTileMap->SetIdx(iIdx, TORCH_START_X + TORCH_LEFT, TORCH_START_Y + iYAdd);
			}
			else
			{
				pTileMap->SetIdx(iIdx, TORCH_START_X + TORCH_UP, TORCH_START_Y + iYAdd);
			}
		}
		else
		{
			if (bBlock[3])
			{
				// 03, 23 모두 UP
				pTileMap->SetIdx(iIdx, TORCH_START_X + TORCH_UP, TORCH_START_Y + iYAdd);
			}
			else
			{
				// L R 오른쪽 보도록.
				pTileMap->SetIdx(iIdx, TORCH_START_X + TORCH_RIGHT, TORCH_START_Y + iYAdd);

			}
		}

		break;

	case 3:

		if (bBlock[3] == false)
		{
			// 바닥이 뚫린 경우 : 오른쪽을 보도록.
			pTileMap->SetIdx(iIdx, TORCH_START_X + TORCH_RIGHT, TORCH_START_Y + iYAdd);
		}
		else
		{
			// 바닥이 뚫린 경우를 제외하고는 모두 Up
			pTileMap->SetIdx(iIdx, TORCH_START_X + TORCH_UP, TORCH_START_Y + iYAdd);
		}

		break;

	case 4:
		pTileMap->SetIdx(iIdx, TORCH_START_X + TORCH_UP, TORCH_START_Y + iYAdd);

		break;
	}

	pTileMap->SetTileInfo(iIdx, TileInfo(true, false, eItem));

	return true;
}

void CMouseScript::DigOneBlockItem(CTileMap2D * pTileMap, int iX, int iY, ITEM eItem, int iParticle)
{
	// One Block Tile

	Vec2 tCenter = pTileMap->GetTileLT(iX, iY);
	tCenter += Vec2(8.f, -8.f);

	if (iParticle != 0)
	{
		vector<CParticleScript*> vecParticle;
		CPoolMgr::GetInst()->OrderParticle(iParticle, vecParticle);

		Vec3 tCenterPos = Vec3(tCenter.x, tCenter.y, 60.f);

		float fMinRad = 0.f; // XMConvertToRadians(180.f/* + 70.f*/);
		float fMaxRad = XM_2PI; // XMConvertToRadians(360.f /*- 70.f*/);

		for (size_t i = 0; i < vecParticle.size(); ++i)
		{
			vecParticle[i]->Init(tCenterPos, 30.f, eItem, fMinRad, fMaxRad, 2.2f, 220.f, 8.f);
		}

	}
	
	// 한 칸 오브젝트.
	SetTileIdx(pTileMap, iX, iY, ITEM::NONE);

	CreateItem(eItem, pTileMap, iX, iY);

	// 토치의 경우에는 캔다고 해서 주변의 영향을 주지 않는다.
	if (GetItemType(eItem) != ITEM_TYPE::TORCH)
		CTileMapMgr::GetInst()->SetTileDisplay(pTileMap, iX, iY, false);
	else
	{
		// Item이 Torch인 경우에는... 해당 위치에 있던 광원을 없애야 한다.
		const vector<CGameObject*>& vecObj = CSceneMgr::GetInst()->GetScene()->FindLayer(L"Light")->GetParentObjVec();

		auto iter = vecObj.begin();
		auto iterEnd = vecObj.end();

		for (; iter != iterEnd; ++iter)
		{
			Vec3 vPos = (*iter)->GetTransform()->GetLocalPos();
			if (vPos.x == tCenter.x && vPos.y == tCenter.y)
			{
				DeleteObject((*iter));
				break;
			}
		}
	}
}

void CMouseScript::SelectItem(ITEM eItem)
{
	// 여기 아이템 flag 받아서, SelectObject의 크기를 변경하는 부분.
	switch (eItem)
	{
	case ITEM::DIRT:
	case ITEM::STONE:
	case ITEM::WOOD:
	case ITEM::WOOD_WALL:
	case ITEM::CACTUS:
	case ITEM::NONE:
	default:
		m_pSelect->ChangeSelectType(SELECT_TYPE::ONE);

		break;

	case ITEM::WORKBENCH:
	case ITEM::CACTUS_WORKBENCH:
		m_pSelect->ChangeSelectType(SELECT_TYPE::TWO_W);

		break;

	case ITEM::WOOD_DOOR:
	case ITEM::CACTUS_DOOR:
		m_pSelect->ChangeSelectType(SELECT_TYPE::TWO_W_THREE_H);

		break;


	/*case ITEM::CACTUS_SWORD:
		break;
	case ITEM::CACTUS_PICKAXE:
		break;
	case ITEM::CACTUS_AXE:
		break;
	case ITEM::COPPER_SWORD:
		break;
	case ITEM::COPPER_AXE:

		break;
	case ITEM::COPPER_PICKAXE:
		break;*/
	}

}

void CMouseScript::CreateItem(ITEM eItem, CTileMap2D* pTileMap, int iX, int iY)
{

	wstring strName = ITEM_NAME[(UINT)eItem];
	strName += L"Obj";

	CGameObject* pObj = CPrototypeMgr::GetInst()->CreateCloneObj(strName, L"ItemObj");

	if (pObj)
	{
		// 셋팅 !
		Vec2 tPos = pTileMap->GetTileLT(iX, iY);
		tPos += Vec2(8.f, -8.f);

		pObj->GetTransform()->SetLocalPos(tPos);
	}
	else
	{
		assert(false);
	}
}

void CMouseScript::TileCollision()
{
	// 이곳에서 Tile과 관련된 처리를 한다.
	// 일단 Mouse의 위치를 토대로 현재 충돌중인 TileObj를 구한다.

	// Mouse의 위치가 바뀐 경우 : TileObj Update

	bool bCollision = false;
	for (size_t i = 0; i < m_vecTileObj.size(); ++i)
	{
		if (CCollisionMgr::GetInst()->IsCollision(GetCollider2D(), m_vecTileObj[i]->GetCollider2D()))
		{
			bCollision = true;
			m_pTileObj = m_vecTileObj[i];
			break;
		}
	}

	if (bCollision == false)
	{
		m_pSelectTile->GetTransform()->SetLocalPos(0.f, 0.f);
		m_pTileObj = nullptr;
		g_pTileMap = nullptr;
		return;
	}

	// 해당 타일의 어느 인덱스를 가리키고 있는지 판단한다. -1도 나올 가능성이 없지는 않음.
	CTileMap2D* pTileMap = m_pTileObj->GetTileMap2D();
	g_pTileMap = pTileMap;

	int iX = pTileMap->GetTileIdxX(GetTransform()->GetLocalPos().x);
	int iY = pTileMap->GetTileIdxY(GetTransform()->GetLocalPos().y);

	if (iX == -1 || iY == -1)
		return;

	int iIdx = iY * 100 + iX; // 현재 인덱스.

	if (iX != m_iColX || iY != m_iColY)
	{
		// m_iColX, m_iColY 이전 인덱스

		// Selected Tile에 대한 처리를 하자.
		Vec2 tTileLT = pTileMap->GetTileLT(iX, iY);
		m_pSelectTile->GetTransform()->SetLocalPos(tTileLT);

		// 이전 위치에 타일이 있었다면... Exit.
		m_pSelect->OnCollisionTileExit(pTileMap, m_iColX, m_iColY);


		// 충돌 인덱스가 달라졌다면, 초를 다시 센다. 
		m_fDigTime = 0.f;
		m_fDigSound = 0.f;
		m_iDig = 0;
		m_iColX = iX;
		m_iColY = iY;
		m_bDigFirst = true;

		// 새로운 위치에 타일이 있다면... Enter
		m_pSelect->OnCollisionTileEnter(pTileMap, m_iColX, m_iColY);

	}
	else
	{

		float fDist = Vec2::Distance(Vec2(GetTransform()->GetLocalPos()), Vec2(m_pPlayer->GetTransform()->GetLocalPos()));

		// 인덱스가 같다.
		if (KEYPRESS(KEY_TYPE::KEY_LBTN)  && // 키를 누르고 있어야 하고,
			m_pPlayer->IsDigable(pTileMap->GetTileItem(iIdx)) && // 해당 블럭을 파는 도구가 알맞아야 하며
			!m_pPlayer->IsFall() && // 플레이어가 떨어지는 상태면 안 되고,
			fDist <= 100.f &&// 거리는 100이하여야 한다.
			pTileMap->IsTileDigable(iIdx) // 캘 수 있는 타일
			)
		{
			// 캐다

			// 시간을 세주자.
			float fTime = DT;

			m_fDigTime += fTime;

			if (m_fDigTime >= m_fDigLimitTime)
			{
				m_bDig = true;

				if (m_bDigFirst)
				{
					CResMgr::GetInst()->Play(L"Dig_0", CHANNEL_TYPE::DIG, EFFECT_VOLUME);
					m_bDigFirst = false;
				}

				// Layer : ItemObj
				// Pos : 없어진 Tile의 자리 중앙.
				ITEM eItem = pTileMap->GetTileInfo(iIdx).m_eItem;

				// 해당 자리의 블럭을 없애는 처리.

				if (eItem == ITEM::WORKBENCH || 
					eItem == ITEM::CACTUS_WORKBENCH || 
					eItem == ITEM::WOOD_DOOR ||
					eItem == ITEM::CACTUS_DOOR)
				{
					// 두 칸 오브젝트.
					// 캔 위치를 토대로 TileBundle을 가져온다.
					list<BundleTile>::iterator iter = CTileMapMgr::GetInst()->GetBundleTileIter(pTileMap, eItem, iX, iY);

					for (size_t i = 0; i < iter->vecIdx.size(); ++i)
					{
						pTileMap->SetTileInfo(iter->vecIdx[i].iY * 100 + iter->vecIdx[i].iX, TileInfo(false, false, ITEM::NONE));
						pTileMap->SetIdx(iter->vecIdx[i].iY * 100 + iter->vecIdx[i].iX, NONE_TILE_X, NONE_TILE_Y);
					}

					CreateItem(eItem, pTileMap, iX, iY);

					for (size_t i = 0; i < iter->vecIdx.size(); ++i)
					{
						CTileMapMgr::GetInst()->SetTileDisplay(pTileMap, iter->vecIdx[i].iX, iter->vecIdx[i].iY, false);
					}

					// Tile Bundle을 지운다.
					CTileMapMgr::GetInst()->GetBundleList(pTileMap).erase(iter);

				}
				// 자연적인 Cactus에 대해서만 처리한다.
				else if (pTileMap->IsTileNatural(iIdx) && (eItem == ITEM::CACTUS || eItem == ITEM::WOOD))
				{
					CResMgr::GetInst()->Play(L"Grass", CHANNEL_TYPE::EFFECT_0, EFFECT_VOLUME);

					// 만약 아이템에 대한 처리가 연결된 덩어리로 이루어 지는 경우에 사용.
					list<BundleTile>::iterator iter = CTileMapMgr::GetInst()->GetBundleTileIter(pTileMap, eItem, iX, iY);

					// 아이템의 수.
					int iCount = (int)iter->vecIdx.size();

					int iObjCount = iCount / 3;
					if (iCount % 3 != 0)
						++iObjCount;

					// ItemObjScript를 Pool에서 가져온다. 
					vector<CItemObjScript*> vecObj;
					CPoolMgr::GetInst()->OrderObj(GetListItem(eItem), iObjCount, vecObj);

					// 해당 자리의 타일을 없애는 처리. tile -> none
					for (int i = 0; i < iCount; ++i)
					{
						pTileMap->SetTileInfo(iter->vecIdx[i].iY * 100 + iter->vecIdx[i].iX, TileInfo(false, false, ITEM::NONE));
						pTileMap->SetIdx(iter->vecIdx[i].iY * 100 + iter->vecIdx[i].iX, NONE_TILE_X, NONE_TILE_Y);
					}

					// 영향을 받는 범위의 빛을 다시 계산.
					for (int i = 0; i < iCount; ++i)
						CTileMapMgr::GetInst()->SetTileDisplay(pTileMap, iter->vecIdx[i].iX, iter->vecIdx[i].iY, false);

					// 파티클 생성.
					vector<CParticleScript*> vecParticle;
					CPoolMgr::GetInst()->OrderParticle(iObjCount * 3, vecParticle);


					for (int i = 0; i < iObjCount; ++i)
					{
						// ItemObjScript를 통해서 ObjCount만큼에 iCount를 적절히 분배한다.
						Vec2 tPos = pTileMap->GetTileLT(iter->vecIdx[i].iX, iter->vecIdx[i].iY);
						tPos += Vec2(8.f, -8.f);

						for (int iParticle = 0; iParticle < 3; ++iParticle)
						{
							vecParticle[i * 3 + iParticle]->Init(Vec3(tPos.x, tPos.y, 60.f),
								30.f, eItem, 0.f, XM_2PI, 2.2f, 220.f, 8.f);
						}

						vecObj[i]->GetTransform()->SetLocalPos(tPos);
						vecObj[i]->Start();

						if (iCount >= 3)
						{
							vecObj[i]->SetCount(3);
							iCount -= 3;
						}
						else
						{
							vecObj[i]->SetCount(iCount);
							break;
						}

					}
					
					// 3으로 나누어 떨어지지 않는 경우에는 3보다 작은 수가 들어가 있다.
					// 3으로 나누어 떨어지는 경우는 0이 들어가 있다.
					// 아무튼 0 1 2가 아니면 오류상황이다.
					if (iCount >= 3)
						assert(false);
					
					



					// Tile Bundle을 지운다.
					CTileMapMgr::GetInst()->GetBundleList(pTileMap).erase(iter);

					

				}
				else
				{
					// TileMap, iX, iY, eItem
					DigOneBlockItem(pTileMap, iX, iY, eItem);

				}
				// 해당 자리의 참조 인덱스를 어떻게 해야 한다.
				// 이 자리에서 블럭을 캤다는 소리이다.
				m_pSelectTile->GetCollider2D()->OnCollisionExit(nullptr);
				m_bDig = false;
			}

			// DigSound 처리
			m_fDigSound += fTime;

			if (m_fDigSound >= m_fDigSoundLimitTime)
			{
				m_fDigSound -= m_fDigSoundLimitTime;
				++m_iDig;

				if(m_iDig < 3)
					CResMgr::GetInst()->Play(L"Dig_" + to_wstring(m_iDig), CHANNEL_TYPE::DIG, EFFECT_VOLUME);
			}
			


		}

		// 설치.

		else if (KEYDOWN(KEY_TYPE::KEY_LBTN) &&
			(pTileMap->GetTileItem(iIdx) == ITEM::NONE) &&	 // 해당 블럭은 NONE 이라면..
			!m_pPlayer->IsFall() &&							 // 플레이어가 떨어지는 상태면 안 되고,
			fDist <= 100.f		&&						 // 거리는 100이하여야 한다.
			!g_bUIColl &&							 // UI 충돌은 아니다.
			m_pPlayer->GetHandleItem())										
			{
				CItemScript* pCurItem = m_pPlayer->GetHandleItem();
				CUIItemScript* pUIItem = (CUIItemScript*)pCurItem->GetUIObj()->FindScript((UINT)SCRIPT_TYPE::UIITEMSCRIPT);

				bool bType = false;
				
				switch (pCurItem->GetType())
				{
					case ITEM_TYPE::BLOCK:
					case ITEM_TYPE::TORCH:
					case ITEM_TYPE::WALL:
					case ITEM_TYPE::DOOR:
					case ITEM_TYPE::BENCH:

						bType = true;
						break;
				}

				if (bType &&
					CInventoryScript::GetMtrlCount(pUIItem->GetItem()) != 0)
				{
					// 블럭을 들고 있고, 해당 개수가 존재한다.
					// 클릭한 곳이, Object가 존재하지 않아서, 충돌이 일어나지 않는지 확인한다.
					// SelectTile Object가 충돌인 경우 : 설치를 못하는 것이다.

					// 블럭의 좌우 상하 중에 하나라도 블럭이 있어야 설치 가능.
					bool bEnable = false;

					// 추가 설치 조건.
					if (m_pSelect->GetSelectType() == SELECT_TYPE::ONE)
					{
						for (int i = 0; i < 4; ++i)
						{
							int x = 0;
							int y = 0;

							switch (i)
							{
							case 0: x = 0;	y = 1; break;
							case 1: x = 0;	y = -1; break;
							case 2: x = 1;	y = 0; break;
							case 3: x = -1;	y = 0; break;
							}

							int iCurIdx = (iY + y) * 100 + (iX + x);

							if (iCurIdx >= 0 && iCurIdx <= 3999)
							{
								if (pTileMap->GetTileItem(iCurIdx) != ITEM::NONE)
								{
									bEnable = true;
									break;
								}
							}
						}
					}
					else if (m_pSelect->GetSelectType() == SELECT_TYPE::TWO_W)
					{
						// Table. 아래 두 인덱스의 Tile이... NONE이 아니다.
						bEnable = true;

						for (int i = 0; i < 2; ++i)
						{
							int iCurIdx = (iY + 1) * 100 + (iX + i);

							if (iCurIdx >= 0 && iCurIdx <= 3999)
							{
								if (pTileMap->GetTileItem(iCurIdx) == ITEM::NONE)
								{
									bEnable = false;
									break;
								}
							}
						}
					}
					else if (m_pSelect->GetSelectType() == SELECT_TYPE::TWO_W_THREE_H)
					{
						// DOOR. 아래 두 인덱스의 Tile이... NONE이 아니다.
						bEnable = true;

						for (int i = 0; i < 2; ++i)
						{
							int iCurIdx = (iY + 3) * 100 + (iX + i);

							if (iCurIdx >= 0 && iCurIdx <= 3999)
							{
								if (pTileMap->GetTileItem(iCurIdx) == ITEM::NONE)
								{
									bEnable = false;
									break;
								}
							}
						}
					}
			

				

					// 해당 아이템의 개수를 하나 줄여준다.
					if ((m_pSelectTile->GetCollider2D()->GetCollisionCount() == 0) && bEnable)
					{
						// 어떤 블럭인지에 따라서, 아이템이 달라진다.
						
						switch (pCurItem->GetItem())
						{
						case ITEM::NONE:
						
							break;

						case ITEM::WOOD_WALL:
						case ITEM::CACTUS_WALL:
						case ITEM::STONE_WALL:
						case ITEM::DIRT_WALL:

						case ITEM::DIRT:
						case ITEM::STONE:
						case ITEM::WOOD:
						case ITEM::CACTUS:

							SetTileIdx(pTileMap, iX, iY, pCurItem->GetItem());
							CTileMapMgr::GetInst()->SetTileDisplay(pTileMap, iX, iY, true);
							break;

							// 횃불아이템
						case ITEM::TORCH:
						case ITEM::VIOLET_TORCH:
						case ITEM::YELLOW_TORCH:
						case ITEM::BLUE_TORCH:
						case ITEM::GREEN_TORCH:
						case ITEM::RED_TORCH:
						case ITEM::ORANGE_TORCH:
						case ITEM::WHITE_TORCH:
						case ITEM::PINK_TORCH:
						case ITEM::ICE_TORCH:
							
							SetTorchIdx(pTileMap, iX, iY, pCurItem->GetItem());
							break;


			

							// BundleTile Object
						case ITEM::WORKBENCH:
							pTileMap->SetTileInfo(iIdx, TileInfo(true, true, pCurItem->GetItem()));
							pTileMap->SetIdx(iIdx, 26, 12);

							pTileMap->SetTileInfo(iIdx + 1, TileInfo(true, true, pCurItem->GetItem()));
							pTileMap->SetIdx(iIdx + 1, 27, 12);

							{
								BundleTile tTile;
								tTile.eItem = pCurItem->GetItem();
								tTile.vecIdx.push_back(Index(iX, iY));
								tTile.vecIdx.push_back(Index(iX + 1, iY));

								CTileMapMgr::GetInst()->AddBundleTile(pTileMap, tTile);
							}

							CTileMapMgr::GetInst()->SetTileDisplay(pTileMap, iX, iY, true);
							CTileMapMgr::GetInst()->SetTileDisplay(pTileMap, iX + 1, iY, true);

							
							break;

						case ITEM::CACTUS_WORKBENCH:
							pTileMap->SetTileInfo(iIdx, TileInfo(true, true, pCurItem->GetItem()));
							pTileMap->SetIdx(iIdx, 26, 13);

							pTileMap->SetTileInfo(iIdx + 1, TileInfo(true, true, pCurItem->GetItem()));
							pTileMap->SetIdx(iIdx + 1, 27, 13);

							{
								BundleTile tTile;
								tTile.eItem = pCurItem->GetItem();
								tTile.vecIdx.push_back(Index(iX, iY));
								tTile.vecIdx.push_back(Index(iX + 1, iY));

								CTileMapMgr::GetInst()->AddBundleTile(pTileMap, tTile);
							}


							CTileMapMgr::GetInst()->SetTileDisplay(pTileMap, iX, iY, true);
							CTileMapMgr::GetInst()->SetTileDisplay(pTileMap, iX + 1, iY, true);

							break;

						case ITEM::WOOD_DOOR:

							// 인덱스 넘어가서 문제 생기지는 않겠지...
							// 아니 애초에 바닥이 있어야 설치가 되니까 조건처리는 된거지.
						{
							BundleTile tTile;
							tTile.eItem = pCurItem->GetItem();
							int iDirAdd = m_pPlayer->GetDir();

							if (iDirAdd == -1)
								iDirAdd = 0;

							tTile.iDir = iDirAdd;

							int idx = 0;
							for (int y = 0; y < 3; ++y)
							{
								for (int x = 0; x < 2; ++x)
								{
									idx = (iY + y) * 100 + (iX + x);
									pTileMap->SetTileInfo(idx, TileInfo(false, false, pCurItem->GetItem()));

									tTile.vecIdx.push_back(Index((iX + x), (iY + y)));
								}

								idx = (iY + y) * 100 + (iX + iDirAdd);
								pTileMap->SetIdx(idx, WOOD_DOOR_START_X + 2, WOOD_DOOR_START_Y + y);
								pTileMap->SetTileInfo(idx, TileInfo(true, true, pCurItem->GetItem()));

							}

							CTileMapMgr::GetInst()->AddBundleTile(pTileMap, tTile);

							for (int y = 0; y < 3; ++y)
							{
								for (int x = 0; x < 2; ++x)
								{
									CTileMapMgr::GetInst()->SetTileDisplay(pTileMap, iX + x, iY + y, true);
								}
							}

						}

							break;

						case ITEM::CACTUS_DOOR:


						{
							BundleTile tTile;
							tTile.eItem = pCurItem->GetItem();
							int iDirAdd = m_pPlayer->GetDir();

							if (iDirAdd == -1)
								iDirAdd = 0;

							tTile.iDir = iDirAdd;

							int idx = 0;
							for (int y = 0; y < 3; ++y)
							{
								for (int x = 0; x < 2; ++x)
								{
									idx = (iY + y) * 100 + (iX + x);
									pTileMap->SetTileInfo(idx, TileInfo(false, false, pCurItem->GetItem()));

									tTile.vecIdx.push_back(Index((iX + x), (iY + y)));
								}

								idx = (iY + y) * 100 + (iX + iDirAdd);
								pTileMap->SetIdx(idx, CACTUS_DOOR_START_X + 2, CACTUS_DOOR_START_Y + y);
								pTileMap->SetTileInfo(idx, TileInfo(true, true, pCurItem->GetItem()));

							}

							CTileMapMgr::GetInst()->AddBundleTile(pTileMap, tTile);

							for (int y = 0; y < 3; ++y)
							{
								for (int x = 0; x < 2; ++x)
								{
									CTileMapMgr::GetInst()->SetTileDisplay(pTileMap, iX + x, iY + y, true);
								}
							}

						}

							break;
						}
					
						CInventoryScript::SubMtrlCount(pCurItem->GetItem());
						CResMgr::GetInst()->Play(L"Dig_2", CHANNEL_TYPE::EFFECT_0, EFFECT_VOLUME);

						if (CInventoryScript::GetMtrlCount(pCurItem->GetItem()) == 0)
						{
							// 손에 들고 있던 아이템을 모두 사용한 경우이다. (내부에서 없애는 처리)
							m_pPlayer->GetInven()->ClearItem(pCurItem);
							SelectItem(ITEM::NONE);
						}

						// 아이템을 설치한 경우. 새로운 타일이 위치함.
						m_pSelectTile->GetCollider2D()->OnCollisionEnter(nullptr);
					}

				}
				
			}

			// Door Open에 대한 처리
			else if (KEYDOWN(KEY_TYPE::KEY_RBTN) &&		// RBtn
					(GetItemType(pTileMap->GetTileItem(iIdx)) == ITEM_TYPE::DOOR)) // 누른 위치가 Door
			{
				ITEM eItem = pTileMap->GetTileItem(iIdx);
				// BundleTile의 정보를 토대로.. Door의 위치를 확인한다.

				list<BundleTile>::iterator iter = CTileMapMgr::GetInst()->GetBundleTileIter(pTileMap, eItem, iX, iY);
				bool bOpen = true;

				for (int i = 0; i < 2; ++i)
				{
					if (pTileMap->IsTileBlock(iter->vecIdx[i].iY * 100 + iter->vecIdx[i].iX))
					{
						bOpen = false;
						break;
					}
				}

				int iTileTypeX = 0;
				int iTileTypeY = 0;

				if (eItem == ITEM::WOOD_DOOR)
				{
					iTileTypeX = WOOD_DOOR_START_X;
					iTileTypeY = WOOD_DOOR_START_Y;
				}
				else
				{
					iTileTypeX = CACTUS_DOOR_START_X;
					iTileTypeY = CACTUS_DOOR_START_Y;
				}


				if (bOpen)
				{
					// open -> close
					// 0이면 짝수에 문
					// 1이면 홀수에 문
					CResMgr::GetInst()->Play(L"Door_Closed", CHANNEL_TYPE::EFFECT_0, EFFECT_VOLUME);

					if (iter->iDir == 0)
					{
						// 왼쪽 : 짝수번째에 문이 존재.
						for (int i = 0; i <= 4; i += 2)
						{
							int iIdx = iter->vecIdx[i].iY * 100 + iter->vecIdx[i].iX;
							pTileMap->SetTileInfo(iIdx, true, true, eItem);
							pTileMap->SetIdx(iIdx, iTileTypeX + 2, iTileTypeY + i / 2);
						}

						//		  홀수번째에 문이 없다.
						for (int i = 1; i <= 5; i += 2)
						{
							int iIdx = iter->vecIdx[i].iY * 100 + iter->vecIdx[i].iX;
							pTileMap->SetTileInfo(iIdx, false, false, eItem);
							pTileMap->SetIdx(iIdx, NONE_TILE_X, NONE_TILE_Y);

						}


						
					}
					else
					{

						//			짝수번째에 문이 없다.
						for (int i = 0; i <= 4; i += 2)
						{
							int iIdx = iter->vecIdx[i].iY * 100 + iter->vecIdx[i].iX;
							pTileMap->SetTileInfo(iIdx, false, false, eItem);
							pTileMap->SetIdx(iIdx, NONE_TILE_X, NONE_TILE_Y);

						}

						// 오른쪽 : 홀수번째에 문이 존재
						for (int i = 1; i <= 5; i += 2)
						{
							int iIdx = iter->vecIdx[i].iY * 100 + iter->vecIdx[i].iX;
							pTileMap->SetTileInfo(iIdx, true, true, eItem);
							pTileMap->SetIdx(iIdx, iTileTypeX + 2, iTileTypeY + i / 2);

						}
					}

					


				}
				else
				{
					// close -> open
					CResMgr::GetInst()->Play(L"Door_Opened", CHANNEL_TYPE::EFFECT_0, EFFECT_VOLUME);

					for (int y = 0; y < 3; ++y)
					{
						for (int x = 0; x < 2; ++x)
						{
							int iNumIdx = y * 2 + x; // 0 ~ 5
							int iIdx = iter->vecIdx[iNumIdx].iY * 100 + iter->vecIdx[iNumIdx].iX;
							pTileMap->SetTileInfo(iIdx, true, false, eItem);
							pTileMap->SetIdx(iIdx, iTileTypeX + x, iTileTypeY + y);
						}
					}
				}

			}


	}

	// 해당 인덱스가 충돌이다.
	// 같은 Tile을 몇 초 동안 처리하면, 사라지도록 .... 구현해보자.
	

}

void CMouseScript::Awake()
{
	// 초기값.
	SetDigTime(0.6f);

	m_hWnd = CDevice::GetInst()->GetHandle();
	
	// UI Camera가 0 0의 위치에 있다는 가정하에...

	m_tRes = CDevice::GetInst()->GetResolution();


	vector<CGameObject*> vecObj;
	CSceneMgr::GetInst()->FindObjByTag(L"MainCamera", vecObj);

	if (vecObj.empty())
		assert(false);

	m_pCamera = vecObj[0];

	// TileMap Obj를 담는다.
	CLayer* pLayer = CSceneMgr::GetInst()->GetScene()->FindLayer(L"Tile");

	if (pLayer == nullptr)
		assert(false);

	m_vecTileObj = pLayer->GetParentObjVec(); // vector 복사.

	for (size_t i = 0; i < m_vecTileObj.size(); ++i)
	{
		// Tile Layer의 Obj에 TileMap2D Component를 가지고 있지 않은 오브젝트가 있다.
		if (m_vecTileObj[i]->GetTileMap2D() == nullptr)
			assert(false);
	}

	if (GetCollider2D() == nullptr)
		assert(false);

	vecObj.clear();
	CSceneMgr::GetInst()->FindObjByTag(L"Player", vecObj);

	if (vecObj.empty())
		assert(false);

	m_pPlayer = (CPlayerScript*)vecObj[0]->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT);

	if (m_pPlayer == nullptr)
		assert(false);

	vecObj.clear();
	CSceneMgr::GetInst()->FindObjByTag(L"MouseUIObj", vecObj);

	if (vecObj.empty())
		assert(false);

	m_pMouseUIObj = vecObj[0];


	vecObj.clear();
	CSceneMgr::GetInst()->FindObjByTag(L"SelectedTile", vecObj);

	if (vecObj.empty())
		assert(false);

	m_pSelectTile = vecObj[0];
	m_pSelectTile->GetCollider2D()->SetRender(true);

	m_pSelect = (CSelectScript*)m_pSelectTile->FindScript((UINT)SCRIPT_TYPE::SELECTSCRIPT);

	if (m_pSelect == nullptr)
		assert(false);

	// Tile을 설치하는 경우에 체크할 오브젝트가 들은 Layer를 설정해둔다.
	CScene* pScene = CSceneMgr::GetInst()->GetScene();
	CLayer* pLayer1 = pScene->FindLayer(L"Default");
	CLayer* pLayer2 = pScene->FindLayer(L"Monster");

	m_iLayerCheck |= (1 << pLayer1->GetLayerIdx());
	m_iLayerCheck |= (1 << pLayer2->GetLayerIdx());


}

void CMouseScript::Update()
{
	// 마우스의 위치를 알아오도록 한다.
	GetCursorPos(&m_tPoint);
	ScreenToClient(m_hWnd, &m_tPoint);

	static Vec2 tLTTrans = Vec2(m_tRes.fWidth / -2.f, m_tRes.fHeight / 2.f);

	Vec3 tStartPos = m_pCamera->GetTransform()->GetLocalPos();
	tStartPos.x += tLTTrans.x;
	tStartPos.y += tLTTrans.y;

	

	Vec3 tPos = Vec3(tStartPos.x + m_tPoint.x, tStartPos.y - m_tPoint.y, 5.f);
	GetTransform()->SetLocalPos(tPos);

	TileCollision();

	// MouseUIObj (UI와 충돌하는 MouseObj)
	m_tUIPos = tLTTrans; // UI 시작위치.
	m_tUIPos.x += m_tPoint.x;
	m_tUIPos.y -= m_tPoint.y;

	m_pMouseUIObj->GetTransform()->SetLocalPos(m_tUIPos);


	if (GetEquipItem())
	{
		// 이 오브젝트는 UIItem이다. (UI카메라로 출력되는 오브젝트이다.)
		// UI 카메라를 기준으로 출력을 하되, 마우스를 따라다니는 것처럼 표현한다.

		m_tUIPos.x += 21.f;
		m_tUIPos.y -= 21.f;
		GetEquipItem()->GetTransform()->SetLocalPos(m_tUIPos);

	}

	if (m_tPoint.x < 0 || m_tPoint.y < 0 || 
		m_tPoint.x > m_tRes.fWidth || m_tPoint.y > m_tRes.fHeight)
	{
		ShowCursor(true);
	}
	else 
		ShowCursor(false);

}

void CMouseScript::Render()
{
	static wchar_t strPos[50] = {};



#ifdef _DEBUG


	if (KEYDOWN(KEY_TYPE::KEY_0) && KEYDOWN(KEY_TYPE::KEY_LCTRL))
	{
		m_bShow = !m_bShow;
		ShowCursor(m_bShow);
	}


	/*
	wsprintf(strPos, L"Mouse : (%d, %d)", (int)m_tPoint.x, (int)m_tPoint.y);
	CFontMgr::GetInst()->DrawFont(strPos, 600.f, 0.f, 16.f, FONT_RGBA(255, 0, 0, 255));
	*/
	Vec3 tPos = GetTransform()->GetLocalPos();
//
//// 실제 위치.
	wsprintf(strPos, L"Mouse Pos : (%d, %d)", (int)tPos.x, (int)tPos.y);
	CFontMgr::GetInst()->DrawFont(strPos, 600.f, 20.f, 16.f, FONT_RGBA(255, 0, 0, 255));
//	/*
//
//		wsprintf(strPos, L"Show : %d", m_bShow);
//		CFontMgr::GetInst()->DrawFont(strPos, 600.f, 40, 16.f, FONT_RGBA(255, 0, 0, 255));*/
//
//		// collision Tile
//	wsprintf(strPos, L"Col Tile X : %d, Y :%d", (int)m_iColX, (int)m_iColY);
//	CFontMgr::GetInst()->DrawFont(strPos, 600.f, 60.f, 16.f, FONT_RGBA(255, 0, 0, 255));
//

	// Debug
	wsprintf(strPos, L"Mouse UI Pos : (%d, %d)", (int)m_tUIPos.x, (int)m_tUIPos.y);
	CFontMgr::GetInst()->DrawFont(strPos, 600.f, 80.f, 16.f, FONT_RGBA(255, 0, 0, 255));

//	// Dir
//	if (m_pPlayer)
//	{
//		wsprintf(strPos, L"Ply Dir: %d", m_pPlayer->GetDir());
//		CFontMgr::GetInst()->DrawFont(strPos, 600.f, 100.f, 16.f, FONT_RGBA(255, 0, 0, 255));
//	}
//	
	// MousePos (Screen)
	wsprintf(strPos, L"Mouse Pos(WND) : (%d, %d)", (int)m_tPoint.x, (int)m_tPoint.y);
	CFontMgr::GetInst()->DrawFont(strPos, 600.f, 140.f, 16.f, FONT_RGBA(255, 0, 0, 255));

	// Coll Mouse
	wsprintf(strPos, L"Mouse UI Coll : %d", CMouseScript::g_bUIColl);
	CFontMgr::GetInst()->DrawFont(strPos, 600.f, 160.f, 16.f, FONT_RGBA(255, 0, 0, 255));
	
#endif

	CMouseScript::g_bUIColl = false;
}

void CMouseScript::OnCollision(CCollider2D * pOther)
{
	m_strCollision = pOther->GetObj()->GetName();
	static wchar_t strName[30] = {};

	// 아마... Mouse와 충돌하는데 Collider2D를 사용하는 경우는 Player Or Monster일 것이다.
	if (m_pPlayer->GetObj() == pOther->GetObj())
	{
		wsprintf(strName, L" %d/%d", m_pPlayer->GetHP(), m_pPlayer->GetHPMax());
	}
	else
	{
		// Monster
		CMonsterScript* pMonster = nullptr;

		if (m_strCollision[0] == L'Z')
		{
			pMonster = (CMonsterScript*)pOther->GetObj()->FindScript((UINT)SCRIPT_TYPE::ZOMBIESCRIPT);
		}
		else
		{
			pMonster = (CMonsterScript*)pOther->GetObj()->FindScript((UINT)SCRIPT_TYPE::SLIMESCRIPT);
		}

		wsprintf(strName, L" %d/%d", pMonster->GetHP(), pMonster->GetHPMax());
	}

	m_strCollision += strName;

}
