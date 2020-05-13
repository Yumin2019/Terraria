#include "stdafx.h"
#include "TileMapMgr.h"
#include <queue>
#include <Engine/SceneMgr.h>

#include <Engine/Scene.h>
#include <Engine/Layer.h>
#include <Engine/GameObject.h>
#include <Engine/TileMap2D.h>


CTileMapMgr::CTileMapMgr() :
	m_arrVisited{}
{
}


CTileMapMgr::~CTileMapMgr()
{
}

void CTileMapMgr::AddBundleTileBFS(CTileMap2D * pTileMap,
	int iStartX, int iStartY)
{
	// 해당 TileMap의 정보에서... iStartX와 iStartY에 해당하는 ITEM TAG를 
	// 가진 타일들을 찾아서 묶어주는 역할이다. (덩어리로 되어있는 것들)

	// 해당 아이템.
	ITEM eItem = pTileMap->GetTileItem(iStartY * 100 + iStartX);

	queue<Index> queue;
	queue.push(Index(iStartX, iStartY));
	VisitedIndex(iStartX, iStartY);

	// 번들 정보를 채워서 넘기기 위한 용도이다.
	BundleTile tBundle;
	tBundle.eItem = eItem;

	while (!queue.empty())
	{
		Index tIndex = queue.front();
		queue.pop();
		// BundleList에 해당 정보를 추가.
		tBundle.vecIdx.push_back(tIndex);

		// 좌우상하 인덱스를 queue에 추가한다.
		// 인덱스가 제대로 된 범위인지 체크해야 하고, 
		// 그곳을 방문했는지 체크해야 하고,
		//	방문하지 않은 경우라면, ITEM이 맞는지 확인해야 한다.

		ITEM eCompare = pTileMap->GetTileItem(tIndex.iX - 1, tIndex.iY);

		// 왼쪽.
		if (CheckIndex(tIndex.iX - 1, tIndex.iY) &&
			!CheckVisited(tIndex.iX - 1, tIndex.iY) && 
			eItem == eCompare)
		{
			queue.push(Index(tIndex.iX - 1, tIndex.iY));
			VisitedIndex(tIndex.iX - 1, tIndex.iY);
		}
		// 오른쪽, 위, 아래 ...

		eCompare = pTileMap->GetTileItem(tIndex.iX + 1, tIndex.iY);

		// 오른쪽
		if (CheckIndex(tIndex.iX + 1, tIndex.iY) &&
			!CheckVisited(tIndex.iX + 1, tIndex.iY) &&
			eItem == eCompare)
		{
			queue.push(Index(tIndex.iX + 1, tIndex.iY));
			VisitedIndex(tIndex.iX + 1, tIndex.iY);
		}

		// 위.
		eCompare = pTileMap->GetTileItem(tIndex.iX, tIndex.iY - 1);
		if (CheckIndex(tIndex.iX, tIndex.iY - 1) &&
			!CheckVisited(tIndex.iX, tIndex.iY - 1) &&
			eItem == eCompare)
		{
			queue.push(Index(tIndex.iX, tIndex.iY - 1));
			VisitedIndex(tIndex.iX, tIndex.iY - 1);
		}

		// 아래
		eCompare = pTileMap->GetTileItem(tIndex.iX, tIndex.iY + 1);
		if (CheckIndex(tIndex.iX, tIndex.iY + 1) &&
			!CheckVisited(tIndex.iX, tIndex.iY + 1) &&
			eItem == eCompare)
		{
			queue.push(Index(tIndex.iX, tIndex.iY + 1));
			VisitedIndex(tIndex.iX, tIndex.iY + 1);
		}

	}

	// 이렇게.. 해서, BundleList에 원하는 정보가 추가가 되었다.
	// 이것을 추가해주자.
	GetBundleList(pTileMap).push_back(tBundle);

	if (tBundle.vecIdx.empty())
		assert(false);

}

bool CTileMapMgr::Init()
{
	// 모든 Layer의 오브젝트를 순회하면서... TileMap 컴포넌트를 가진 경우에는 정보를 추가한다.

	CScene* pScene = CSceneMgr::GetInst()->GetScene();
	m_vecBundleInfo.clear();

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObj = pScene->GetLayer(i)->GetParentObjVec(); // 자식일리는 없지..

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			if (vecObj[j]->GetTileMap2D() != nullptr)
			{
				m_vecBundleInfo.push_back(BundleInfo(vecObj[j]->GetTileMap2D()));
			}
		}
	}


	// 광원처리를 위해서 TileBuffer로 넘길 값을 셋팅한다.
	for (size_t i = 0; i < m_vecBundleInfo.size(); ++i)
	{
		CTileMap2D* pTileMap = m_vecBundleInfo[i].pTileMap;

		for (int iY = 0; iY < 40; ++iY)
		{
			for (int iX = 0; iX < 100; ++iX)
			{
				pTileMap->SetTileDisplay(iX, iY);
			}
		}
	}






	// Tile을 순회하면서, 추가적인 List작업을 한다.
	for (size_t i = 0; i < m_vecBundleInfo.size(); ++i)
	{
		VisitedArrReset();
	
		for (int iY = 0; iY < 40; ++iY)
		{
			for (int iX = 0; iX < 100; ++iX)
			{
				int iIdx = iY * 100 + iX;

				// 이미 방문한 곳인 경우. 넘긴다.
				if (m_arrVisited[iIdx] == true)
					continue;

				// TileMap으로부터, 우리가 원하는 타일의 정보를 찾고... 연결 하는 작업을 거친다.
				switch (m_vecBundleInfo[i].pTileMap->GetTileItem(iIdx))
				{
					// 묶음 처리를 하고 싶은 Item을 추가로 적는다.
				case ITEM::CACTUS:
				case ITEM::WOOD:
					AddBundleTileBFS(m_vecBundleInfo[i].pTileMap, iX, iY);
					break;
				}

				// 방문처리.
				m_arrVisited[iIdx] = true;
			}
		}
	}




	return true;
}

void CTileMapMgr::AddBundleTile(CTileMap2D * pTileMap, const BundleTile & tBundleTile)
{
	// 이 타일맵에... BundleTile 정보를 넣는다..

	GetBundleList(pTileMap).push_back(tBundleTile); // IIndex + Item정보.
}

list<BundleTile>& CTileMapMgr::GetBundleList(CTileMap2D * pTileMap)
{
	for (size_t i = 0; i < m_vecBundleInfo.size(); ++i)
	{
		if (m_vecBundleInfo[i].pTileMap == pTileMap)
		{
			return m_vecBundleInfo[i].BundleList;
		}
	}

	// 찾아달라고 했지만, 해당하는 Tilemap이 없는 경우.
	assert(false);
	return m_vecBundleInfo[0].BundleList;
}

BundleTile & CTileMapMgr::GetBundleTile(CTileMap2D * pTileMap, ITEM eItem, int iX, int iY)
{
	list<BundleTile>& BundleList = GetBundleList(pTileMap);

	list<BundleTile>::iterator iter;
	list<BundleTile>::iterator iterEnd = BundleList.end();

	for (iter = BundleList.begin(); iter != iterEnd; ++iter)
	{
		if (iter->eItem == eItem)
		{
			const vector<Index>& vecIdx = iter->vecIdx;

			for (size_t i = 0; i < vecIdx.size(); ++i)
			{
				if (vecIdx[i].iX == iX && vecIdx[i].iY == iY)
				{
					return *iter;
				}
			}
		}
	}

	// 찾을 수 없다.
	assert(false);
	return (*iter);
}

void CTileMapMgr::GetBundleTile(CTileMap2D * pTileMap, ITEM_TYPE eType, vector<BundleTile>& vecBundle)
{
	list<BundleTile>& BundleList = GetBundleList(pTileMap);

	list<BundleTile>::iterator iter;
	list<BundleTile>::iterator iterEnd = BundleList.end();

	for (iter = BundleList.begin(); iter != iterEnd; ++iter)
	{
		if (GetItemType(iter->eItem) == eType)
		{
			vecBundle.push_back((*iter));
		}
	}
}

list<BundleTile>::iterator CTileMapMgr::GetBundleTileIter(CTileMap2D * pTileMap, ITEM eItem, int iX, int iY)
{
	list<BundleTile>& BundleList = GetBundleList(pTileMap);

	list<BundleTile>::iterator iter;
	list<BundleTile>::iterator iterEnd = BundleList.end();

	for (iter = BundleList.begin(); iter != iterEnd; ++iter)
	{
		if (iter->eItem == eItem)
		{
			const vector<Index>& vecIdx = iter->vecIdx;

			for (size_t i = 0; i < vecIdx.size(); ++i)
			{
				if (vecIdx[i].iX == iX && vecIdx[i].iY == iY)
				{
					return iter;
				}
			}
		}
	}

	// 찾을 수 없다.
	assert(false);
	return iter;
}

void CTileMapMgr::SetTileDisplay(CTileMap2D * pTileMap, int iX, int iY, bool bBuild)
{
	// iX와 iY의 영향력에 해당하는 부분을 모두 호출하여 Display를 다시 구하는 함수이다.
	// bBuild는 자기 위치에 대한 처리를 할지 말지 여부이다.

	if (bBuild)
		pTileMap->SetTileDisplay(iX, iY);

	for (int iWallNum = 1; iWallNum <= 4; ++iWallNum)
	{
		int iLeftTop = (iY - iWallNum) * 100 + iX - iWallNum;
		int iRightTop = (iY - iWallNum) * 100 + iX + iWallNum;
		int iLeftBottom = (iY + iWallNum) * 100 + iX - iWallNum;
		int iRightBottom = (iY + iWallNum) * 100 + iX + iWallNum;

		pTileMap->SetTileDisplay(iLeftTop);
		pTileMap->SetTileDisplay(iRightTop);
		pTileMap->SetTileDisplay(iLeftBottom);
		pTileMap->SetTileDisplay(iRightBottom);

		// Top 
		for (int iStart = iLeftTop + 1; iStart < iRightTop; ++iStart)
			pTileMap->SetTileDisplay(iStart);

		// bottom 
		for (int iStart = iLeftBottom + 1; iStart < iRightBottom; ++iStart)
			pTileMap->SetTileDisplay(iStart);

		// Left
		for (int iStart = iLeftTop + 100; iStart < iLeftBottom; iStart += 100)
			pTileMap->SetTileDisplay(iStart);

		// Right
		for (int iStart = iRightTop + 100; iStart < iRightBottom; iStart += 100)
			pTileMap->SetTileDisplay(iStart);

	}

}

