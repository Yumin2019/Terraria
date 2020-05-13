#pragma once
#include <Engine/Script.h>

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

	_tagBundleTile() :
		eItem(ITEM::NONE)
	{}

}BundleTile, *PBundleTile;

class CBundleTileScript :
	public CScript
{
public:
	CBundleTileScript();
	virtual ~CBundleTileScript();
	CLONE(CBundleTileScript);

private:
	list<BundleTile>		m_BundleList;

public:
	list<BundleTile>& GetBundleList() { return m_BundleList; } // List반환 함수.
	void AddBundleTile(ITEM eItem, const vector<Index>& vecIdx); // BundleTile추가 함수.

public:
	virtual void Update() {}
};

