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
	// �ش� TileMap�� ��������... iStartX�� iStartY�� �ش��ϴ� ITEM TAG�� 
	// ���� Ÿ�ϵ��� ã�Ƽ� �����ִ� �����̴�. (����� �Ǿ��ִ� �͵�)

	// �ش� ������.
	ITEM eItem = pTileMap->GetTileItem(iStartY * 100 + iStartX);

	queue<Index> queue;
	queue.push(Index(iStartX, iStartY));
	VisitedIndex(iStartX, iStartY);

	// ���� ������ ä���� �ѱ�� ���� �뵵�̴�.
	BundleTile tBundle;
	tBundle.eItem = eItem;

	while (!queue.empty())
	{
		Index tIndex = queue.front();
		queue.pop();
		// BundleList�� �ش� ������ �߰�.
		tBundle.vecIdx.push_back(tIndex);

		// �¿���� �ε����� queue�� �߰��Ѵ�.
		// �ε����� ����� �� �������� üũ�ؾ� �ϰ�, 
		// �װ��� �湮�ߴ��� üũ�ؾ� �ϰ�,
		//	�湮���� ���� �����, ITEM�� �´��� Ȯ���ؾ� �Ѵ�.

		ITEM eCompare = pTileMap->GetTileItem(tIndex.iX - 1, tIndex.iY);

		// ����.
		if (CheckIndex(tIndex.iX - 1, tIndex.iY) &&
			!CheckVisited(tIndex.iX - 1, tIndex.iY) && 
			eItem == eCompare)
		{
			queue.push(Index(tIndex.iX - 1, tIndex.iY));
			VisitedIndex(tIndex.iX - 1, tIndex.iY);
		}
		// ������, ��, �Ʒ� ...

		eCompare = pTileMap->GetTileItem(tIndex.iX + 1, tIndex.iY);

		// ������
		if (CheckIndex(tIndex.iX + 1, tIndex.iY) &&
			!CheckVisited(tIndex.iX + 1, tIndex.iY) &&
			eItem == eCompare)
		{
			queue.push(Index(tIndex.iX + 1, tIndex.iY));
			VisitedIndex(tIndex.iX + 1, tIndex.iY);
		}

		// ��.
		eCompare = pTileMap->GetTileItem(tIndex.iX, tIndex.iY - 1);
		if (CheckIndex(tIndex.iX, tIndex.iY - 1) &&
			!CheckVisited(tIndex.iX, tIndex.iY - 1) &&
			eItem == eCompare)
		{
			queue.push(Index(tIndex.iX, tIndex.iY - 1));
			VisitedIndex(tIndex.iX, tIndex.iY - 1);
		}

		// �Ʒ�
		eCompare = pTileMap->GetTileItem(tIndex.iX, tIndex.iY + 1);
		if (CheckIndex(tIndex.iX, tIndex.iY + 1) &&
			!CheckVisited(tIndex.iX, tIndex.iY + 1) &&
			eItem == eCompare)
		{
			queue.push(Index(tIndex.iX, tIndex.iY + 1));
			VisitedIndex(tIndex.iX, tIndex.iY + 1);
		}

	}

	// �̷���.. �ؼ�, BundleList�� ���ϴ� ������ �߰��� �Ǿ���.
	// �̰��� �߰�������.
	GetBundleList(pTileMap).push_back(tBundle);

	if (tBundle.vecIdx.empty())
		assert(false);

}

bool CTileMapMgr::Init()
{
	// ��� Layer�� ������Ʈ�� ��ȸ�ϸ鼭... TileMap ������Ʈ�� ���� ��쿡�� ������ �߰��Ѵ�.

	CScene* pScene = CSceneMgr::GetInst()->GetScene();
	m_vecBundleInfo.clear();

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObj = pScene->GetLayer(i)->GetParentObjVec(); // �ڽ��ϸ��� ����..

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			if (vecObj[j]->GetTileMap2D() != nullptr)
			{
				m_vecBundleInfo.push_back(BundleInfo(vecObj[j]->GetTileMap2D()));
			}
		}
	}


	// ����ó���� ���ؼ� TileBuffer�� �ѱ� ���� �����Ѵ�.
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






	// Tile�� ��ȸ�ϸ鼭, �߰����� List�۾��� �Ѵ�.
	for (size_t i = 0; i < m_vecBundleInfo.size(); ++i)
	{
		VisitedArrReset();
	
		for (int iY = 0; iY < 40; ++iY)
		{
			for (int iX = 0; iX < 100; ++iX)
			{
				int iIdx = iY * 100 + iX;

				// �̹� �湮�� ���� ���. �ѱ��.
				if (m_arrVisited[iIdx] == true)
					continue;

				// TileMap���κ���, �츮�� ���ϴ� Ÿ���� ������ ã��... ���� �ϴ� �۾��� ��ģ��.
				switch (m_vecBundleInfo[i].pTileMap->GetTileItem(iIdx))
				{
					// ���� ó���� �ϰ� ���� Item�� �߰��� ���´�.
				case ITEM::CACTUS:
				case ITEM::WOOD:
					AddBundleTileBFS(m_vecBundleInfo[i].pTileMap, iX, iY);
					break;
				}

				// �湮ó��.
				m_arrVisited[iIdx] = true;
			}
		}
	}




	return true;
}

void CTileMapMgr::AddBundleTile(CTileMap2D * pTileMap, const BundleTile & tBundleTile)
{
	// �� Ÿ�ϸʿ�... BundleTile ������ �ִ´�..

	GetBundleList(pTileMap).push_back(tBundleTile); // IIndex + Item����.
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

	// ã�ƴ޶�� ������, �ش��ϴ� Tilemap�� ���� ���.
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

	// ã�� �� ����.
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

	// ã�� �� ����.
	assert(false);
	return iter;
}

void CTileMapMgr::SetTileDisplay(CTileMap2D * pTileMap, int iX, int iY, bool bBuild)
{
	// iX�� iY�� ����¿� �ش��ϴ� �κ��� ��� ȣ���Ͽ� Display�� �ٽ� ���ϴ� �Լ��̴�.
	// bBuild�� �ڱ� ��ġ�� ���� ó���� ���� ���� �����̴�.

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

