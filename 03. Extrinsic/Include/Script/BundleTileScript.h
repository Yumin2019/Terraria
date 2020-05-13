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
	vector<Index>	vecIdx;		// �� �������� Tile Idx�� ������ �ִ´�.
	ITEM			eItem;		// � ������.

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
	list<BundleTile>& GetBundleList() { return m_BundleList; } // List��ȯ �Լ�.
	void AddBundleTile(ITEM eItem, const vector<Index>& vecIdx); // BundleTile�߰� �Լ�.

public:
	virtual void Update() {}
};

