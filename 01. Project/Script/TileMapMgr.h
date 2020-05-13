#pragma once

// TileMap에 관한 처리를 담당하기 위한 Mgr

class CTileMap2D;

typedef struct _tagIndex
{
	int iX;
	int iY;

	_tagIndex() :
		iX(0),
		iY(0)
	{}

	_tagIndex(int iX, int iY) :
		iX(iX),
		iY(iY)
	{}

}Index, *PIndex;

typedef struct _tagBundleTile
{
	vector<Index>	vecIdx;		// 그 아이템의 Tile Idx를 가지고 있는다.
	ITEM			eItem;		// 어떤 아이템.
	int				iDir;

	_tagBundleTile() :
		eItem(ITEM::NONE),
		iDir(1)
	{}

}BundleTile, *PBundleTile;

typedef struct _tagBundleInfo
{
	CTileMap2D*				pTileMap;	// 어느 타일맵인지...
	list<BundleTile>		BundleList; // 어떤 인덱스들이.. 어떠한 ITEM정보를 갖는지.

	_tagBundleInfo(CTileMap2D* pTileMap) :
		pTileMap(pTileMap)
	{}

}BundleInfo, *PBundleInfo;

class CTileMapMgr
{
	SINGLE(CTileMapMgr);

private:
	vector<BundleInfo>		m_vecBundleInfo;
	bool					m_arrVisited[4000];

private:
	void VisitedArrReset() { memset(m_arrVisited, 0, sizeof(bool) * 4000); }
	void AddBundleTileBFS(CTileMap2D* pTileMap, int iStartX, int iStartY);
	bool CheckIndex(int iX, int iY) { return (iX >= 0 && iX < 100 && iY >= 0 && iY < 40); }
	bool CheckVisited(int iX, int iY) { int iIdx = iY * 100 + iX; return m_arrVisited[iIdx]; } 
	void VisitedIndex(int iX, int iY) { int iIdx = iY * 100 + iX; m_arrVisited[iIdx] = true; }

public:
	bool Init();
	void AddBundleTile(CTileMap2D* pTileMap, const BundleTile& tBundleTile); // 넣어준 포인터에 해당하는 TileMap에 접근하여... BundleTile 정보를 넣어준다.
	list<BundleTile>& GetBundleList(CTileMap2D* pTileMap); // 넣어준 포인터에 해당하는 TileMap의 BundleList를 반환한다.
	BundleTile& GetBundleTile(CTileMap2D* pTileMap, ITEM eItem, int iX, int iY); // TileMap에 해당하는 BundleTile정보를 레퍼런스로 반환하는 함수이다. (탐색)
	// TileMap에 해당하는 여러 BundleList중에서... ITEM을 정보로 갖고, iX, iY를 인덱스로 가지고 있는... BundleTile정보를 반환.

	void GetBundleTile(CTileMap2D* pTileMap, ITEM_TYPE eType, vector<BundleTile>& vecBundle);
	// 해당 TileMap에서 Type값이 일치하는 Bundle을 찾아서 반환한다.

	// list<BundleTile>::iterator 로 반환하여 Bundle을 가리키도록 하는 버전이 있어야 한다.
	list<BundleTile>::iterator GetBundleTileIter(CTileMap2D* pTileMap, ITEM eItem, int iX, int iY);

	// iX와 iY의 영향력에 해당하는 부분을 모두 호출하여 Display를 다시 구하는 함수이다.
	void SetTileDisplay(CTileMap2D* pTileMap, int iX, int iY, bool bBuild);
};

