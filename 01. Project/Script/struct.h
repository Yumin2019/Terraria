#pragma once

// Defines
#define DIRT_TILE_START_X 0
#define DIRT_TILE_START_Y 0

#define WOOD_TILE_START_X 16
#define WOOD_TILE_START_Y 0

#define STONE_TILE_START_X 0
#define STONE_TILE_START_Y 15

#define CACTUS_TILE_START_X 0
#define CACTUS_TILE_START_Y 30

#define DIRT_WALL_TILE_START_X 0
#define DIRT_WALL_TILE_START_Y 45

#define STONE_WALL_TILE_START_X 4
#define STONE_WALL_TILE_START_Y 45

#define WOOD_WALL_TILE_START_X 8
#define WOOD_WALL_TILE_START_Y 45

#define CACTUS_WALL_TILE_START_X 12
#define CACTUS_WALL_TILE_START_Y 45

#define CACTUS_SWORD_DMG_MIN 8
#define CACTUS_SWORD_DMG_MAX 12

#define COPPER_SWORD_DMG_MIN 4
#define COPPER_SWORD_DMG_MAX 7

// bgm�� ����
#define EFFECT_VOLUME 0.3f
#define EFFECT_VOLUME2 0.5f // �Ҹ��� ���� �͵�


#define XY_OFFSET_TEST \
static int iX = 0; \
static int iY = 0; \
if (KEYDOWN(KEY_TYPE::KEY_1)) iX += 1; \
if (KEYDOWN(KEY_TYPE::KEY_2)) iX -= 1;\
if (KEYDOWN(KEY_TYPE::KEY_3)) iY += 1;\
if (KEYDOWN(KEY_TYPE::KEY_4)) iY -= 1;

#define CLONE_DISABLE_SCRIPT(Type) Type* Clone() { assert(false); return nullptr;} \
									Type(const Type& obj) : CScript(0) { assert(false);}



// ���� �ý��ۿ��� ����� ����ü.
typedef struct _tagCrafting
{
	ITEM	m_eResult;
	int		m_iResult; // ����� ����.

	ITEM	m_eItemArr[3]; 
	int		m_iItemArr[3];

	bool	m_bBench; // Bench�� �־�� �ϴ��� �����̴�.

	_tagCrafting() :
		m_iItemArr{}
	{
		m_eItemArr[0] = ITEM::NONE;
		m_eItemArr[1] = ITEM::NONE;
		m_eItemArr[2] = ITEM::NONE;
		m_eResult = ITEM::NONE;
		m_iResult = 1;
		m_bBench = false;
	}

	_tagCrafting(ITEM eResult, int iResult, bool bBench,
		ITEM eItem1, int iCount1,
		ITEM eItem2 = ITEM::NONE, int iCount2 = 0,
		ITEM eItem3 = ITEM::NONE, int iCount3 = 0)
	{
		m_iResult = iResult;
		m_eResult = eResult;
		m_bBench = bBench;
		m_eItemArr[0] = eItem1;
		m_eItemArr[1] = eItem2;
		m_eItemArr[2] = eItem3;

		m_iItemArr[0] = iCount1;
		m_iItemArr[1] = iCount2;
		m_iItemArr[2] = iCount3;
	}



}Crafting, *PCrafting;


typedef struct _tagInvenSlot
{
	// �κ��丮�� ���Կ� ���� ó���� �ϱ� ���� ����ü�̴�.

	int iIdx;
	class CItemScript* pItemScript;

	_tagInvenSlot() :
		iIdx(0),
		pItemScript(nullptr)
	{}

	_tagInvenSlot(int iIdx, CItemScript* pScript) :
		iIdx(iIdx),
		pItemScript(pScript)
	{}

}InvenSlot, *PInvenSlot;