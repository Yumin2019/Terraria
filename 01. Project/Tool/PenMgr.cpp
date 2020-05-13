#include "stdafx.h"
#include "PenMgr.h"


CPenMgr::CPenMgr()
{
}


CPenMgr::~CPenMgr()
{
	// Pen을 삭제한다.
	DeleteObject(m_hMagentaPen);

	for (UINT i = 0; i < (UINT)TILE::END; ++i)
	{
		DeleteObject(m_hTilePen[i]);
		DeleteObject(m_hTileBrush[i]);
	}
}

bool CPenMgr::Init()
{
	m_hMagentaPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 255));

	/*
enum class TILE
{
	NONE,			WHITE
	DIRT,			DARK YELLOW	
	STONE,			GRAY
	WOOD,			BROWN0
	WOOD_WALL,		BROWN1
	CATUS,			DARK GREEN
	END
	*/

	m_hTilePen[(UINT)TILE::NONE] = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
	m_hTilePen[(UINT)TILE::DIRT] = CreatePen(PS_SOLID, 3, RGB(128, 128, 0));
	m_hTilePen[(UINT)TILE::STONE] = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	m_hTilePen[(UINT)TILE::WOOD] = CreatePen(PS_SOLID, 3, RGB(136, 0, 21));
	m_hTilePen[(UINT)TILE::WOOD_WALL] = CreatePen(PS_SOLID, 3, RGB(185, 122, 87));
	m_hTilePen[(UINT)TILE::CACTUS] = CreatePen(PS_SOLID, 3, RGB(34, 177, 76));

	m_hTileBrush[(UINT)TILE::NONE] = CreateSolidBrush(RGB(255, 255, 255));
	m_hTileBrush[(UINT)TILE::DIRT] = CreateSolidBrush(RGB(128, 128, 0));
	m_hTileBrush[(UINT)TILE::STONE] = CreateSolidBrush(RGB(0, 0, 0));
	m_hTileBrush[(UINT)TILE::WOOD] = CreateSolidBrush(RGB(136, 0, 21));
	m_hTileBrush[(UINT)TILE::WOOD_WALL] = CreateSolidBrush(RGB(185, 122, 87));
	m_hTileBrush[(UINT)TILE::CACTUS] = CreateSolidBrush(RGB(34, 177, 76));


	return true;
}
