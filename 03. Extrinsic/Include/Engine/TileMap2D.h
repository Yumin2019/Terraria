#pragma once
#include "Component.h"

class CTileMap2D :
	public CComponent
{
public:
	CTileMap2D();
	virtual ~CTileMap2D();
	CLONE(CTileMap2D);

private:
	TileBuffer	m_tTileBufferArr[4000]; // UV + 참조 인덱스를 담당한다.
	TileInfo	m_tTileInfoArr[4000];
	Vec2		m_tTileSize;


public:
	// Index를 바꾸는 기능.
	void SetIdx(int iIdx, int iTileIdxX, int iTileIdxY);
	Vec2 GetTileLT(int iX, int iY);
	const Vec2& GetTileSize() const { return m_tTileSize; }

	// x, y 위치를 넣어주면 해당 Tile을 반환하는 함수를 만들자.
	int GetTileIdxX(float x);
	int GetTileIdxY(float y);

	TileInfo& GetTileInfo(int iIdx) { return m_tTileInfoArr[iIdx]; }
	TileInfo& GetTileInfo(int iX, int iY) { int iIdx = iY * 100 + iX; return m_tTileInfoArr[iIdx]; }
	TileBuffer& GetTileBuffer(int iIdx) { return m_tTileBufferArr[iIdx]; }
	TileBuffer& GetTileBuffer(int iX, int iY) { int iIdx = iY * 100 + iX; return m_tTileBufferArr[iIdx]; }
	void SetTileInfo(int iIdx, bool bDigable, bool bBlock, ITEM eItem) { m_tTileInfoArr[iIdx] = TileInfo(bDigable, bBlock, eItem); }
	void SetTileInfo(int iIdx, const TileInfo& tTileInfo) { m_tTileInfoArr[iIdx] = tTileInfo; }
	bool IsTileDigable(int iIdx) const { return m_tTileInfoArr[iIdx].m_bDigable; }
	bool IsTileBlock(int iIdx) const { return m_tTileInfoArr[iIdx].m_bBlock; }
	ITEM GetTileItem(int iIdx) const { return m_tTileInfoArr[iIdx].m_eItem; }
	ITEM GetTileItem(int iX, int iY) { int iIdx = iY * 100 + iX; return m_tTileInfoArr[iIdx].m_eItem; }

	bool IsTileNatural(int iIdx) { return m_tTileInfoArr[iIdx].m_bNatural; }
	void SetTileNatural(int iIdx, bool bNature) { m_tTileInfoArr[iIdx].m_bNatural = bNature; }

	bool IsTileIdx(int iX, int iY) { if (iX >= 0 && iX < 100 && iY >= 0 && iY < 40) return true; return false; }
	bool IsTileIdx(int iIdx) { if (iIdx >= 0 && iIdx < 4000) return true; return false; }

	void SetTileDisplay(int iX, int iY); // None타일을 토대로 밝기를 설정해주는 함수이다.
	void SetTileDisplay(int iIdx);
public:
	virtual void Awake();
	virtual void Update() {}
	void UpdateData();
	virtual bool SaveToScene(FILE* pFile);
	virtual bool LoadFromScene(FILE* pFile);
};

