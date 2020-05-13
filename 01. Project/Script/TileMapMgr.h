#pragma once

// TileMap�� ���� ó���� ����ϱ� ���� Mgr

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
	vector<Index>	vecIdx;		// �� �������� Tile Idx�� ������ �ִ´�.
	ITEM			eItem;		// � ������.
	int				iDir;

	_tagBundleTile() :
		eItem(ITEM::NONE),
		iDir(1)
	{}

}BundleTile, *PBundleTile;

typedef struct _tagBundleInfo
{
	CTileMap2D*				pTileMap;	// ��� Ÿ�ϸ�����...
	list<BundleTile>		BundleList; // � �ε�������.. ��� ITEM������ ������.

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
	void AddBundleTile(CTileMap2D* pTileMap, const BundleTile& tBundleTile); // �־��� �����Ϳ� �ش��ϴ� TileMap�� �����Ͽ�... BundleTile ������ �־��ش�.
	list<BundleTile>& GetBundleList(CTileMap2D* pTileMap); // �־��� �����Ϳ� �ش��ϴ� TileMap�� BundleList�� ��ȯ�Ѵ�.
	BundleTile& GetBundleTile(CTileMap2D* pTileMap, ITEM eItem, int iX, int iY); // TileMap�� �ش��ϴ� BundleTile������ ���۷����� ��ȯ�ϴ� �Լ��̴�. (Ž��)
	// TileMap�� �ش��ϴ� ���� BundleList�߿���... ITEM�� ������ ����, iX, iY�� �ε����� ������ �ִ�... BundleTile������ ��ȯ.

	void GetBundleTile(CTileMap2D* pTileMap, ITEM_TYPE eType, vector<BundleTile>& vecBundle);
	// �ش� TileMap���� Type���� ��ġ�ϴ� Bundle�� ã�Ƽ� ��ȯ�Ѵ�.

	// list<BundleTile>::iterator �� ��ȯ�Ͽ� Bundle�� ����Ű���� �ϴ� ������ �־�� �Ѵ�.
	list<BundleTile>::iterator GetBundleTileIter(CTileMap2D* pTileMap, ITEM eItem, int iX, int iY);

	// iX�� iY�� ����¿� �ش��ϴ� �κ��� ��� ȣ���Ͽ� Display�� �ٽ� ���ϴ� �Լ��̴�.
	void SetTileDisplay(CTileMap2D* pTileMap, int iX, int iY, bool bBuild);
};

