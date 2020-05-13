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

	// �ڱ� �ڽ�. (����� Torch�� ������ �ʴ´�) ���� ��ġ.
	ChangeTileIdx(pTileMap, iX, iY);

	// �ֺ� Ÿ�ϵ�
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

	// Torch Tile�� ��ġ�� 
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


	// LTRB �����̴�.
	int arrIdx[4] = { (iY) * 100 + (iX - 1) ,
					  (iY - 1) * 100 + (iX),
					  (iY) * 100 + (iX + 1) ,
					  (iY + 1) * 100 + (iX) };

	bool arrBlock[4] = {};
	int iBlockNum = 0;


	if (bWall)
	{
		// ���� ��쿡�� �� Ÿ���� �ƴ϶�� ������ ������ ����.
		for (int i = 0; i < 4; ++i)
		{
			if (pTileMap->IsTileIdx(arrIdx[i]) && 
				pTileMap->GetTileInfo(arrIdx[i]).m_eItem != ITEM::NONE)
			{
				arrBlock[i] = true;
				++iBlockNum;
			}

		}

		// �� Ÿ�Ͽ� ���ؼ� ó���Ѵ�.

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
			// 0��2
			//  3

			if (arrBlock[0])
			{
				// 0���� ����ε�... n���� ����̴�.

				if (arrBlock[1])
				{
					// 1��. 
					pTileMap->SetIdx(iIdx, iTileTypeX + 3, iTileTypeY + 3);

				}
				else if (arrBlock[2])
				{
					// 2��. 
					pTileMap->SetIdx(iIdx, iTileTypeX + 1, iTileTypeY + 1);

				}
				else
				{
					// 3��. 1 3
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
				// L�� �� ���. 
				pTileMap->SetIdx(iIdx, iTileTypeX + 0, iTileTypeY + 1);

			}
			else if (arrBlock[1] == false)
			{
				// T�� �� ���.
				pTileMap->SetIdx(iIdx, iTileTypeX + 1, iTileTypeY + 0);
			}
			else if (arrBlock[2] == false)
			{
				// R�� �� ���. 
				pTileMap->SetIdx(iIdx, iTileTypeX + 3, iTileTypeY + 1);

			}
			else
			{
				// B�� �� ��� 
				pTileMap->SetIdx(iIdx, iTileTypeX + 1, iTileTypeY + 3);

			}


			break;

	
		}
	}
	else
	{
		// ���� �ƴ� �⺻ Tile�� ���ؼ� ó���Ѵ�.

		for (int i = 0; i < 4; ++i)
		{
			if (pTileMap->IsTileIdx(arrIdx[i]) && pTileMap->GetTileInfo(arrIdx[i]).m_bBlock)
			{
				arrBlock[i] = true;
				++iBlockNum;
			}

			// �ε����� �Ѿ���� �ݵ�� false.
			// �������� ���� �ش� �ڸ��� flag�� ����.
		}

		// �ֺ��� ���� ����.
		switch (iBlockNum)
		{
		case 0:
			// 9 3
			pTileMap->SetIdx(iIdx, iTileTypeX + 9, iTileTypeY + 3);
			break;

		case 1:

			if (arrBlock[0])
			{
				// L �� ��. 12 0
				pTileMap->SetIdx(iIdx, iTileTypeX + 12, iTileTypeY + 0);

			}
			else if (arrBlock[1])
			{
				// T�� �� 6 3
				pTileMap->SetIdx(iIdx, iTileTypeX + 6, iTileTypeY + 3);

			}
			else if (arrBlock[2])
			{
				// R �� �� 9 0
				pTileMap->SetIdx(iIdx, iTileTypeX + 9, iTileTypeY + 0);

			}
			else
			{
				// B�� ��. 6 0
				pTileMap->SetIdx(iIdx, iTileTypeX + 6, iTileTypeY + 0);
			}


			break;

		case 2:
			//  1
			// 0��2
			//  3

			if (arrBlock[0])
			{
				// 0���� ����ε�... n���� ����̴�.

				if (arrBlock[1])
				{
					// 1��. 1 4
					pTileMap->SetIdx(iIdx, iTileTypeX + 1, iTileTypeY + 4);

				}
				else if (arrBlock[2])
				{
					// 2��. 6 4
					pTileMap->SetIdx(iIdx, iTileTypeX + 6, iTileTypeY + 4);

				}
				else
				{
					// 3��. 1 3
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
				// L�� �� ���. 4 5
				pTileMap->SetIdx(iIdx, iTileTypeX + 0, iTileTypeY + 0);

			}
			else if (arrBlock[1] == false)
			{
				// T�� �� ���.0 11
				pTileMap->SetIdx(iIdx, iTileTypeX + 1, iTileTypeY + 0);
			}
			else if (arrBlock[2] == false)
			{
				// R�� �� ���. 5 5
				pTileMap->SetIdx(iIdx, iTileTypeX + 4, iTileTypeY + 0);

			}
			else
			{
				// B�� �� ��� 0 12
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

	// �ֺ� Ÿ�ϵ� LTRB
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
			// ����� �� �ε��� + ��ġ�� �� �ִ� ��.
			bBlock[i] = true;
			++iBlockNum;
		}
	}

	// ���� ������ ������ ó������.

	switch (iBlockNum)
	{
	case 0:

		// �ֺ� Ÿ���� ����ٰ� �ڽŸ� ���� ��� : ���������� �����ع�����.
		DigOneBlockItem(pTileMap, iX, iY, eItem);

		return false;

	case 1:

		// ��ġ�Ұ�(dig = false) or �� ĵ ó��(dig = true)
		if (bBlock[1])
		{
			if (m_bDig)
				DigOneBlockItem(pTileMap, iX, iY, eItem); // �ٸ� ���� ĺ��. ������ �ް�... �������.
			else
				CInventoryScript::AddMtrlCount(eItem); // ��ġ�� �ϴٰ� �߸��� ��ġ�� ��ġ.

			return false;
		}

		// L T R B

		if (bBlock[0]) // L
		{
			// �������� ������.
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
		// 0��2
		//   3

		// Top�� �����ϴ��� �� �ϴ��ķ� ������.
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
				// 03, 23 ��� UP
				pTileMap->SetIdx(iIdx, TORCH_START_X + TORCH_UP, TORCH_START_Y + iYAdd);
			}
			else
			{
				// L R ������ ������.
				pTileMap->SetIdx(iIdx, TORCH_START_X + TORCH_RIGHT, TORCH_START_Y + iYAdd);

			}
		}

		break;

	case 3:

		if (bBlock[3] == false)
		{
			// �ٴ��� �ո� ��� : �������� ������.
			pTileMap->SetIdx(iIdx, TORCH_START_X + TORCH_RIGHT, TORCH_START_Y + iYAdd);
		}
		else
		{
			// �ٴ��� �ո� ��츦 �����ϰ�� ��� Up
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
	
	// �� ĭ ������Ʈ.
	SetTileIdx(pTileMap, iX, iY, ITEM::NONE);

	CreateItem(eItem, pTileMap, iX, iY);

	// ��ġ�� ��쿡�� ĵ�ٰ� �ؼ� �ֺ��� ������ ���� �ʴ´�.
	if (GetItemType(eItem) != ITEM_TYPE::TORCH)
		CTileMapMgr::GetInst()->SetTileDisplay(pTileMap, iX, iY, false);
	else
	{
		// Item�� Torch�� ��쿡��... �ش� ��ġ�� �ִ� ������ ���־� �Ѵ�.
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
	// ���� ������ flag �޾Ƽ�, SelectObject�� ũ�⸦ �����ϴ� �κ�.
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
		// ���� !
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
	// �̰����� Tile�� ���õ� ó���� �Ѵ�.
	// �ϴ� Mouse�� ��ġ�� ���� ���� �浹���� TileObj�� ���Ѵ�.

	// Mouse�� ��ġ�� �ٲ� ��� : TileObj Update

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

	// �ش� Ÿ���� ��� �ε����� ����Ű�� �ִ��� �Ǵ��Ѵ�. -1�� ���� ���ɼ��� ������ ����.
	CTileMap2D* pTileMap = m_pTileObj->GetTileMap2D();
	g_pTileMap = pTileMap;

	int iX = pTileMap->GetTileIdxX(GetTransform()->GetLocalPos().x);
	int iY = pTileMap->GetTileIdxY(GetTransform()->GetLocalPos().y);

	if (iX == -1 || iY == -1)
		return;

	int iIdx = iY * 100 + iX; // ���� �ε���.

	if (iX != m_iColX || iY != m_iColY)
	{
		// m_iColX, m_iColY ���� �ε���

		// Selected Tile�� ���� ó���� ����.
		Vec2 tTileLT = pTileMap->GetTileLT(iX, iY);
		m_pSelectTile->GetTransform()->SetLocalPos(tTileLT);

		// ���� ��ġ�� Ÿ���� �־��ٸ�... Exit.
		m_pSelect->OnCollisionTileExit(pTileMap, m_iColX, m_iColY);


		// �浹 �ε����� �޶����ٸ�, �ʸ� �ٽ� ����. 
		m_fDigTime = 0.f;
		m_fDigSound = 0.f;
		m_iDig = 0;
		m_iColX = iX;
		m_iColY = iY;
		m_bDigFirst = true;

		// ���ο� ��ġ�� Ÿ���� �ִٸ�... Enter
		m_pSelect->OnCollisionTileEnter(pTileMap, m_iColX, m_iColY);

	}
	else
	{

		float fDist = Vec2::Distance(Vec2(GetTransform()->GetLocalPos()), Vec2(m_pPlayer->GetTransform()->GetLocalPos()));

		// �ε����� ����.
		if (KEYPRESS(KEY_TYPE::KEY_LBTN)  && // Ű�� ������ �־�� �ϰ�,
			m_pPlayer->IsDigable(pTileMap->GetTileItem(iIdx)) && // �ش� ���� �Ĵ� ������ �˸¾ƾ� �ϸ�
			!m_pPlayer->IsFall() && // �÷��̾ �������� ���¸� �� �ǰ�,
			fDist <= 100.f &&// �Ÿ��� 100���Ͽ��� �Ѵ�.
			pTileMap->IsTileDigable(iIdx) // Ķ �� �ִ� Ÿ��
			)
		{
			// ĳ��

			// �ð��� ������.
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
				// Pos : ������ Tile�� �ڸ� �߾�.
				ITEM eItem = pTileMap->GetTileInfo(iIdx).m_eItem;

				// �ش� �ڸ��� ���� ���ִ� ó��.

				if (eItem == ITEM::WORKBENCH || 
					eItem == ITEM::CACTUS_WORKBENCH || 
					eItem == ITEM::WOOD_DOOR ||
					eItem == ITEM::CACTUS_DOOR)
				{
					// �� ĭ ������Ʈ.
					// ĵ ��ġ�� ���� TileBundle�� �����´�.
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

					// Tile Bundle�� �����.
					CTileMapMgr::GetInst()->GetBundleList(pTileMap).erase(iter);

				}
				// �ڿ����� Cactus�� ���ؼ��� ó���Ѵ�.
				else if (pTileMap->IsTileNatural(iIdx) && (eItem == ITEM::CACTUS || eItem == ITEM::WOOD))
				{
					CResMgr::GetInst()->Play(L"Grass", CHANNEL_TYPE::EFFECT_0, EFFECT_VOLUME);

					// ���� �����ۿ� ���� ó���� ����� ����� �̷�� ���� ��쿡 ���.
					list<BundleTile>::iterator iter = CTileMapMgr::GetInst()->GetBundleTileIter(pTileMap, eItem, iX, iY);

					// �������� ��.
					int iCount = (int)iter->vecIdx.size();

					int iObjCount = iCount / 3;
					if (iCount % 3 != 0)
						++iObjCount;

					// ItemObjScript�� Pool���� �����´�. 
					vector<CItemObjScript*> vecObj;
					CPoolMgr::GetInst()->OrderObj(GetListItem(eItem), iObjCount, vecObj);

					// �ش� �ڸ��� Ÿ���� ���ִ� ó��. tile -> none
					for (int i = 0; i < iCount; ++i)
					{
						pTileMap->SetTileInfo(iter->vecIdx[i].iY * 100 + iter->vecIdx[i].iX, TileInfo(false, false, ITEM::NONE));
						pTileMap->SetIdx(iter->vecIdx[i].iY * 100 + iter->vecIdx[i].iX, NONE_TILE_X, NONE_TILE_Y);
					}

					// ������ �޴� ������ ���� �ٽ� ���.
					for (int i = 0; i < iCount; ++i)
						CTileMapMgr::GetInst()->SetTileDisplay(pTileMap, iter->vecIdx[i].iX, iter->vecIdx[i].iY, false);

					// ��ƼŬ ����.
					vector<CParticleScript*> vecParticle;
					CPoolMgr::GetInst()->OrderParticle(iObjCount * 3, vecParticle);


					for (int i = 0; i < iObjCount; ++i)
					{
						// ItemObjScript�� ���ؼ� ObjCount��ŭ�� iCount�� ������ �й��Ѵ�.
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
					
					// 3���� ������ �������� �ʴ� ��쿡�� 3���� ���� ���� �� �ִ�.
					// 3���� ������ �������� ���� 0�� �� �ִ�.
					// �ƹ�ư 0 1 2�� �ƴϸ� ������Ȳ�̴�.
					if (iCount >= 3)
						assert(false);
					
					



					// Tile Bundle�� �����.
					CTileMapMgr::GetInst()->GetBundleList(pTileMap).erase(iter);

					

				}
				else
				{
					// TileMap, iX, iY, eItem
					DigOneBlockItem(pTileMap, iX, iY, eItem);

				}
				// �ش� �ڸ��� ���� �ε����� ��� �ؾ� �Ѵ�.
				// �� �ڸ����� ���� ĺ�ٴ� �Ҹ��̴�.
				m_pSelectTile->GetCollider2D()->OnCollisionExit(nullptr);
				m_bDig = false;
			}

			// DigSound ó��
			m_fDigSound += fTime;

			if (m_fDigSound >= m_fDigSoundLimitTime)
			{
				m_fDigSound -= m_fDigSoundLimitTime;
				++m_iDig;

				if(m_iDig < 3)
					CResMgr::GetInst()->Play(L"Dig_" + to_wstring(m_iDig), CHANNEL_TYPE::DIG, EFFECT_VOLUME);
			}
			


		}

		// ��ġ.

		else if (KEYDOWN(KEY_TYPE::KEY_LBTN) &&
			(pTileMap->GetTileItem(iIdx) == ITEM::NONE) &&	 // �ش� ���� NONE �̶��..
			!m_pPlayer->IsFall() &&							 // �÷��̾ �������� ���¸� �� �ǰ�,
			fDist <= 100.f		&&						 // �Ÿ��� 100���Ͽ��� �Ѵ�.
			!g_bUIColl &&							 // UI �浹�� �ƴϴ�.
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
					// ���� ��� �ְ�, �ش� ������ �����Ѵ�.
					// Ŭ���� ����, Object�� �������� �ʾƼ�, �浹�� �Ͼ�� �ʴ��� Ȯ���Ѵ�.
					// SelectTile Object�� �浹�� ��� : ��ġ�� ���ϴ� ���̴�.

					// ���� �¿� ���� �߿� �ϳ��� ���� �־�� ��ġ ����.
					bool bEnable = false;

					// �߰� ��ġ ����.
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
						// Table. �Ʒ� �� �ε����� Tile��... NONE�� �ƴϴ�.
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
						// DOOR. �Ʒ� �� �ε����� Tile��... NONE�� �ƴϴ�.
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
			

				

					// �ش� �������� ������ �ϳ� �ٿ��ش�.
					if ((m_pSelectTile->GetCollider2D()->GetCollisionCount() == 0) && bEnable)
					{
						// � �������� ����, �������� �޶�����.
						
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

							// ȶ�Ҿ�����
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

							// �ε��� �Ѿ�� ���� �������� �ʰ���...
							// �ƴ� ���ʿ� �ٴ��� �־�� ��ġ�� �Ǵϱ� ����ó���� �Ȱ���.
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
							// �տ� ��� �ִ� �������� ��� ����� ����̴�. (���ο��� ���ִ� ó��)
							m_pPlayer->GetInven()->ClearItem(pCurItem);
							SelectItem(ITEM::NONE);
						}

						// �������� ��ġ�� ���. ���ο� Ÿ���� ��ġ��.
						m_pSelectTile->GetCollider2D()->OnCollisionEnter(nullptr);
					}

				}
				
			}

			// Door Open�� ���� ó��
			else if (KEYDOWN(KEY_TYPE::KEY_RBTN) &&		// RBtn
					(GetItemType(pTileMap->GetTileItem(iIdx)) == ITEM_TYPE::DOOR)) // ���� ��ġ�� Door
			{
				ITEM eItem = pTileMap->GetTileItem(iIdx);
				// BundleTile�� ������ ����.. Door�� ��ġ�� Ȯ���Ѵ�.

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
					// 0�̸� ¦���� ��
					// 1�̸� Ȧ���� ��
					CResMgr::GetInst()->Play(L"Door_Closed", CHANNEL_TYPE::EFFECT_0, EFFECT_VOLUME);

					if (iter->iDir == 0)
					{
						// ���� : ¦����°�� ���� ����.
						for (int i = 0; i <= 4; i += 2)
						{
							int iIdx = iter->vecIdx[i].iY * 100 + iter->vecIdx[i].iX;
							pTileMap->SetTileInfo(iIdx, true, true, eItem);
							pTileMap->SetIdx(iIdx, iTileTypeX + 2, iTileTypeY + i / 2);
						}

						//		  Ȧ����°�� ���� ����.
						for (int i = 1; i <= 5; i += 2)
						{
							int iIdx = iter->vecIdx[i].iY * 100 + iter->vecIdx[i].iX;
							pTileMap->SetTileInfo(iIdx, false, false, eItem);
							pTileMap->SetIdx(iIdx, NONE_TILE_X, NONE_TILE_Y);

						}


						
					}
					else
					{

						//			¦����°�� ���� ����.
						for (int i = 0; i <= 4; i += 2)
						{
							int iIdx = iter->vecIdx[i].iY * 100 + iter->vecIdx[i].iX;
							pTileMap->SetTileInfo(iIdx, false, false, eItem);
							pTileMap->SetIdx(iIdx, NONE_TILE_X, NONE_TILE_Y);

						}

						// ������ : Ȧ����°�� ���� ����
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

	// �ش� �ε����� �浹�̴�.
	// ���� Tile�� �� �� ���� ó���ϸ�, ��������� .... �����غ���.
	

}

void CMouseScript::Awake()
{
	// �ʱⰪ.
	SetDigTime(0.6f);

	m_hWnd = CDevice::GetInst()->GetHandle();
	
	// UI Camera�� 0 0�� ��ġ�� �ִٴ� �����Ͽ�...

	m_tRes = CDevice::GetInst()->GetResolution();


	vector<CGameObject*> vecObj;
	CSceneMgr::GetInst()->FindObjByTag(L"MainCamera", vecObj);

	if (vecObj.empty())
		assert(false);

	m_pCamera = vecObj[0];

	// TileMap Obj�� ��´�.
	CLayer* pLayer = CSceneMgr::GetInst()->GetScene()->FindLayer(L"Tile");

	if (pLayer == nullptr)
		assert(false);

	m_vecTileObj = pLayer->GetParentObjVec(); // vector ����.

	for (size_t i = 0; i < m_vecTileObj.size(); ++i)
	{
		// Tile Layer�� Obj�� TileMap2D Component�� ������ ���� ���� ������Ʈ�� �ִ�.
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

	// Tile�� ��ġ�ϴ� ��쿡 üũ�� ������Ʈ�� ���� Layer�� �����صд�.
	CScene* pScene = CSceneMgr::GetInst()->GetScene();
	CLayer* pLayer1 = pScene->FindLayer(L"Default");
	CLayer* pLayer2 = pScene->FindLayer(L"Monster");

	m_iLayerCheck |= (1 << pLayer1->GetLayerIdx());
	m_iLayerCheck |= (1 << pLayer2->GetLayerIdx());


}

void CMouseScript::Update()
{
	// ���콺�� ��ġ�� �˾ƿ����� �Ѵ�.
	GetCursorPos(&m_tPoint);
	ScreenToClient(m_hWnd, &m_tPoint);

	static Vec2 tLTTrans = Vec2(m_tRes.fWidth / -2.f, m_tRes.fHeight / 2.f);

	Vec3 tStartPos = m_pCamera->GetTransform()->GetLocalPos();
	tStartPos.x += tLTTrans.x;
	tStartPos.y += tLTTrans.y;

	

	Vec3 tPos = Vec3(tStartPos.x + m_tPoint.x, tStartPos.y - m_tPoint.y, 5.f);
	GetTransform()->SetLocalPos(tPos);

	TileCollision();

	// MouseUIObj (UI�� �浹�ϴ� MouseObj)
	m_tUIPos = tLTTrans; // UI ������ġ.
	m_tUIPos.x += m_tPoint.x;
	m_tUIPos.y -= m_tPoint.y;

	m_pMouseUIObj->GetTransform()->SetLocalPos(m_tUIPos);


	if (GetEquipItem())
	{
		// �� ������Ʈ�� UIItem�̴�. (UIī�޶�� ��µǴ� ������Ʈ�̴�.)
		// UI ī�޶� �������� ����� �ϵ�, ���콺�� ����ٴϴ� ��ó�� ǥ���Ѵ�.

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
//// ���� ��ġ.
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

	// �Ƹ�... Mouse�� �浹�ϴµ� Collider2D�� ����ϴ� ���� Player Or Monster�� ���̴�.
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
