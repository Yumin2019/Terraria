#pragma once
#include <Engine/Script.h>

class CTileMapScript :
	public CScript
{
public:
	CTileMapScript();
	virtual ~CTileMapScript();
	CLONE(CTileMapScript);

private:
	TileBuffer	m_tTileBufferArr[4000];
	Vec2		m_tTileSize;

public:
	// Index를 바꾸는 기능.
	void SetIdx(int iIdx, int iTileIdxX, int iTileIdxY);
	const Vec3& GetTileLT(int iX, int iY);
	
	// x, y 위치를 넣어주면 해당 Tile을 반환하는 함수를 만들자.
	int GetTileIdxX(float x);
	int GetTileIdxY(float y);


public:
	virtual void Awake();
	virtual void Update() {}
	virtual void UpdateData();
	virtual bool SaveToScene(FILE* pFile);
	virtual bool LoadFromScene(FILE* pFile);
};

