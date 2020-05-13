
#include "stdafx.h"
#include "CollisionMgr.h"

#include "SceneMgr.h"
#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"
#include "Collider2D.h"
#include "Transform.h"
#include "TileMap2D.h"


CCollisionMgr::CCollisionMgr() :
	m_LayerCheck{}
{
}


CCollisionMgr::~CCollisionMgr()
{
}

bool CCollisionMgr::Init()
{
	return true;
}

void CCollisionMgr::Update()
{
	// ���� ����� �����´�.
	CScene* pCurScene = CSceneMgr::GetInst()->GetScene();

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		// ���� ��ȸ�Ѵ�.

		for (int j = 0; j < MAX_LAYER; ++j)
		{
			// ���� ��ȸ�Ѵ�.

			// i�࿡ �ش��ϴ� Layer�� j���� �ش��ϴ� bit �ڸ��� 1�� ��� (�浹ó���� �ϰڴٴ� �ǹ�)
			// Layer�� ���� �浹ó���� �����Ѵ�.

			if (m_LayerCheck[i] & (1 << j))
			{
				// i <= j
				CollisionLayer(pCurScene->GetLayer(i), pCurScene->GetLayer(j));
			}
		}
	}
}

void CCollisionMgr::TileCollision(CGameObject * pTileObj, CGameObject * pObj)
{

	// �浹ü�� ������ ���� ��� return.
	if (!pTileObj->GetCollider2D() || !pObj->GetCollider2D())
		return;

	// �浹�� ���� ���� ��� return.
	if(!IsCollision(pTileObj->GetCollider2D(), pObj->GetCollider2D()))
		return;

	CTransform* pObjTrans = pObj->GetTransform();

	Vec4 tTileRC;

	Vec2 tPos = Vec2(pObjTrans->GetLocalPos());
	Vec2 tPrevPos = Vec2(pObjTrans->GetPrevPos());
	Vec2 tPivot = pObjTrans->GetPivot();
	Vec2 tScale = Vec2(pObjTrans->GetWorldScale() * pObj->GetCollider2D()->GetScale());
	bool bTileCollision = false;
	

	Vec2 tMove = tPos - tPrevPos;

	tPivot.x *= -1.f;
	tPivot.y = (1.f - tPivot.y);

	Vec2 tTrans = tPivot * tScale;


	int iDir = 0;

	if (tMove.x < 0.f)
		iDir = -1;
	else if (tMove.x > 0.f)
		iDir = 1;

	float fResult = 0.f;

	// X �� ���� ó��.
	if (TileCollWidth(pObj, pTileObj->GetTileMap2D(), fResult, tTileRC, iDir))
	{
		// �浹�� �Ͼ��, fResult�� ���س��Ҵ�.

		if (iDir == -1)
		{
			// �������� �̵� :  ������ ���� �浹.
			// ���� ��ġ������ ���� ����� ���Ѵ�.
			
			Vec2 tPrevLT = tPrevPos + tTrans;
			Vec2 tLT = tPos + tTrans;

			// tTileRC. r(z)�� tPrevLT.x ���ٴ� �۰�,
			//			    tLT.x ���ٴ� ũ��.
			if (tTileRC.z < tPrevLT.x && tTileRC.z > tLT.x)
			{
				// �İ�� ��� ó��. World ����.. World ���� ?
				pObjTrans->SetLocalPos(pObjTrans->GetLocalPos() + Vec3(fResult, 0.f, 0.f));
				pObjTrans->SetMoveX(true, -1.f);
				bTileCollision = true;
			}
			
		}
		else if (iDir == 1)
		{
			// ���������� �̵� : ���� ���� �浹.
			// ���� ��ġ�� ������ ����� ���Ѵ�.
			Vec2 tPrevRB = tPrevPos - tTrans;
			Vec2 tPosRB = tPos - tTrans;

			// tTileRC.l(x)�� tPrevRB.x ���� ũ��,
			//				  tPosRB.x  ���� �۴�.

			if (tTileRC.x > tPrevRB.x && tTileRC.x < tPosRB.x)
			{
				pObjTrans->SetLocalPos(pObjTrans->GetLocalPos() + Vec3(fResult, 0.f, 0.f));
				bTileCollision = true;
				pObjTrans->SetMoveX(true, 1.f);
			}
		}
	}

	// Y �̵� ó��.

	iDir = 0;

	if (tMove.y < 0.f)
		iDir = -1;

	else if (tMove.y > 0.f)
		iDir = 1;

	if (TileCollHeight(pObj, pTileObj->GetTileMap2D(), fResult, tTileRC, iDir))
	{
		if (iDir == 1)
		{
			// ���� �ö󰡴� ���.
			// PrevLT �� LT�� y���� ������ �Ǵ��Ѵ�.

			Vec2 tPrevLT = tPrevPos + tTrans;
			Vec2 tLT = tPos + tTrans;

			if (tTileRC.w > tPrevLT.y && tTileRC.w < tLT.y)
			{
				pObjTrans->SetLocalPos(pObjTrans->GetLocalPos() + Vec3(0.f, fResult, 0.f));
				bTileCollision = true;
				pObjTrans->SetMoveY(true, 1.f);
			}

		}
		else if (iDir == -1)
		{
			// �Ʒ��� �������� ���.
			Vec2 tPrevRB = tPrevPos - tTrans;
			Vec2 tPosRB = tPos - tTrans;

			if (tTileRC.y < tPrevRB.y && tTileRC.y > tPosRB.y)
			{
				pObjTrans->SetLocalPos(pObjTrans->GetLocalPos() + Vec3(0.f, fResult, 0.f));
				

				bTileCollision = true;
				pObjTrans->SetMoveY(true, -1.f);
			}
		}
	}
	

	// Post Process
	pObjTrans->SetTileColl(bTileCollision); // �浹�� �Ǿ��ٸ�, true �ƴϸ� false �̰��� Script���� �ν� ����. (���� �����ӿ�)

	if (bTileCollision == true)
		pObjTrans->TileCollision();
}


bool CCollisionMgr::TileCollHeight(CGameObject * pObj, CTileMap2D* pTileMap, float & fResult, Vec4 & tTileRC, int iDir)
{
	// �Ʒ��� �������ٸ�, Ÿ���� ������ 

	if (iDir == 0)
		return false;

	CTransform* pTrans = pObj->GetTransform();
	Vec3 tPos = pTrans->GetWorldPos();
	Vec2 tPivot = pTrans->GetPivot();
	Vec3 tScale = pTrans->GetWorldScale() * pObj->GetCollider2D()->GetScale();

	tPivot.x *= -1.f;
	tPivot.y = (1.f - tPivot.y);

	Vec3 tTrans = Vec3(tPivot.x, tPivot.y, 1.f) * tScale;
	Vec3 tLT = tPos + tTrans;

	Vec2 tStart, tEnd;

	if (iDir == -1)
	{
		// �Ʒ��� �������� ���. 
		// �ϴ� ������ ����.
		tStart = Vec2(tLT.x, tLT.y - tScale.y);
		tEnd = Vec2(tLT.x + tScale.x, tStart.y);

	}
	else
	{
		// ���� �ö󰡴� ���
		// ��� ������ ����.
		tStart = Vec2(tLT.x, tLT.y);
		tEnd = Vec2(tStart.x + tScale.x, tStart.y);
	}

	// 3���� ����.
	// y ��ġ�� �ش��ϴ� �ε��� (start, end ����)
	// ���ʿ��� ���� �� �ִ� x �ε��� start
	// �����ʿ��� ���� �� �ִ� x �ε��� end

	int iY = pTileMap->GetTileIdxY(tStart.y);
	int iStartX = pTileMap->GetTileIdxX(tStart.x);
	int iEndX = pTileMap->GetTileIdxX(tEnd.x);

	if (iY < 0 || iY >= 40)
		return false;

	if (iStartX < 0)
		iStartX = 0;
	
	if (iStartX >= 100)
		return false;

	else if (iEndX < 0)
		return false;
		
	if (iEndX >= 100)
		iEndX = 99;

	for(int x = iStartX; x <= iEndX; ++x)
	{
		int idx = iY * 100 + x;
		if (pTileMap->IsTileBlock(idx))
		{
			// �� / �Ʒ� �̵��� ������ ã�Ƴ� ���п� �ش��ϴ� Tile�� Ȯ���ϴ�...
			// �װ��� ���� �־���ϴ�. 

			Vec2 tTileLT = pTileMap->GetTileLT(x, iY);
			Vec2 tTileSize = pTileMap->GetTileSize();

			// l t r b
			tTileRC.x = tTileLT.x;
			tTileRC.y = tTileLT.y;
			tTileRC.z = tTileLT.x + tTileSize.x;
			tTileRC.w = tTileLT.y - tTileSize.y;

			if (iDir == 1)
			{
				// ���� �̵��ϴ� ���.
				// RESULT�� ������ ���;� �Ѵ�.
				fResult = tTileRC.w - tStart.y - 0.1f;
			}
			else
			{
				// �Ʒ��� �̵��ϴ� ���
				// RESULT�� ����� ���;� �Ѵ�.
				fResult = tTileRC.y - tStart.y + 0.1f;
			}

			return true;
		}
	}

	return false;
}

bool CCollisionMgr::TileCollWidth(CGameObject * pObj, CTileMap2D* pTileMap, float & fResult, Vec4 & tTileRC, int iDir)
{
	// �������� �̵��� �ߴٸ�, Ÿ���� ������ ��� �浹�ߴ��� Ȯ���ϰ�,
	// ���������� �̵��� �ߴٸ�, Ÿ���� ���ʸ��� �浹�ߴ��� Ȯ���Ѵ�.

	if (iDir == 0)
		return false;

	CTransform* pTrans = pObj->GetTransform();
	Vec3 tPos = pTrans->GetWorldPos();
	Vec2 tPivot = pTrans->GetPivot();
	Vec3 tScale = pTrans->GetWorldScale() * pObj->GetCollider2D()->GetScale();

	// ���� ���� ���Ѵ�.
	// pivot�� ���� ���� LT�� ���ϱ� ���� �������� �ϴ� ������ ���Ѵ�.
	tPivot.x *= -1.f;
	tPivot.y = (1.f - tPivot.y);

	Vec3 tTrans = Vec3(tPivot.x, tPivot.y, 1.f) * tScale;
	Vec3 tLT = tPos + tTrans;

	Vec2 tStart, tEnd;


	if (iDir == -1)
	{
		// �������� �̵��ϴ� ���
		// ���� ��ܺ��� �ϴ��� ��ġ�� ���Ѵ�.
		tStart = Vec2(tLT.x, tLT.y);
		tEnd = Vec2(tLT.x, tLT.y - tScale.y);

	}
	else
	{
		// ���������� �̵��ϴ� ���
		// ������ ��ܺ��� �ϴ��� ��ġ�� ���Ѵ�.
		tStart = Vec2(tLT.x + tScale.x, tLT.y);
		tEnd = Vec2(tStart.x, tLT.y - tScale.y);
	}

	// 3���� ����.
	// x ��ġ�� �ش��ϴ� �ε��� (start, end ����)
	// ��ܿ��� ���� Y Idx Start.y
	// �ϴܿ��� ���� Y Idx End.y

	int iX = pTileMap->GetTileIdxX(tStart.x);
	int iStartY = pTileMap->GetTileIdxY(tStart.y);
	int iEndY = pTileMap->GetTileIdxY(tEnd.y);

	if (iX < 0 || iX >= 100)
		return false;

	if (iStartY < 0)
		iStartY = 0;
	
	if(iStartY >= 40)
		return false;

	if (iEndY < 0) 
		return false;

	if (iEndY >= 40)
		iEndY = 39;

	// �߸��� ������ ������ ��� false ó��.

	for (int y = iStartY; y <= iEndY; ++y)
	{
		int idx = y * 100 + iX;
		if (pTileMap->IsTileBlock(idx))
		{
			// ���� / ������ �̵��� ������ ã�Ƴ� ���п� �ش��ϴ� Tile�� Ȯ���ϴ�...
			// �װ��� ���� �־���ϴ�. 

			Vec2 tTileLT = pTileMap->GetTileLT(iX, y);
			Vec2 tTileSize = pTileMap->GetTileSize();

			// l t r b
			tTileRC.x = tTileLT.x;
			tTileRC.y = tTileLT.y;
			tTileRC.z = tTileLT.x + tTileSize.x;
			tTileRC.w = tTileLT.y - tTileSize.y;

			if (iDir == 1)
			{
				// ���������� �̵��ϴ� ���.
				// RESULT�� ������ ���;� �Ѵ�.
				fResult = tTileRC.x - tStart.x - 0.1f;
			}
			else
			{
				// �������� �̵��ϴ� ���.
				// RESULT�� ����� ���;� �Ѵ�.
				fResult = tTileRC.z - tStart.x + 0.1f;
			}

			
			return true;
		}
	}

	return false;
}

bool CCollisionMgr::OnCollisionLayer(int iLayerIdx)
{
	if (iLayerIdx < 0 || iLayerIdx >= MAX_LAYER)
		assert(false);

	CScene* pCurScene = CSceneMgr::GetInst()->GetScene();

	CLayer* pLayer = pCurScene->GetLayer(iLayerIdx);

	const vector<CGameObject*>& vecObj = pLayer->GetLayerObjVec();
	// Layer Render ���Ŀ� Tool���� ã�ư���. 
	// ���� Vec�� ������Ʈ�� �׻� ����.

	for (size_t i = 0; i < vecObj.size(); ++i)
	{
		CCollider2D* pCollider = vecObj[i]->GetCollider2D();
		if (pCollider)
		{
			if (pCollider->GetCollisionCount())
			{
				MessageBox(nullptr, L"Object Collision", nullptr, MB_OK);
				return true;
			}
		}
	}

	return false;
}

void CCollisionMgr::CheckCollisionLayer(const wstring & strLayerName1, const wstring & strLayerName2)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetScene();

	// Layer�� �̸��� �־��ָ�, �� Layer�� ���ؼ� �浹ó���� �ϱ� ���ؼ�, �ش� Layer�� �̸��� ���� ��� ���� ��Ƽ�,
	// LayerArray�� �ڸ���Ʈ�� 1���� �־��ִ� ó���� �ϰ� �ִ�.

	CLayer* pLayer1 = pCurScene->FindLayer(strLayerName1);
	CLayer* pLayer2 = pCurScene->FindLayer(strLayerName2);

	// �� ���̶�, nullptr�� ������ ���. Ű ���� �߸� �־��� ��쿡�� assert�� ��ȯ.
	if (pLayer1 == nullptr || pLayer2 == nullptr)
		assert(false);

	int iIdx1 = pLayer1->GetLayerIdx();
	int iIdx2 = pLayer2->GetLayerIdx();

	int iMin = min(iIdx1, iIdx2);
	int iMax = max(iIdx1, iIdx2);

	// Layer�� ������ ���� �۰�, ���� ũ��.

	m_LayerCheck[iMin] |= (1 << iMax);
	// ���� iMin = iMax = 1 �̶��, 
	// 1���� 2��° (1�� �ش��ϴ� bit)�� �����ϰ� �ȴ�.

	// �̷��� �� ��Ʈ�� �̵��ؼ�, �� �ڸ��� OR������ ���ؼ� 1�� �����д�.

}

void CCollisionMgr::UnCheckCollisionLayer(const wstring & strLayerName1, const wstring & strLayerName2)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetScene();

	CLayer* pLayer1 = pCurScene->FindLayer(strLayerName1);
	CLayer* pLayer2 = pCurScene->FindLayer(strLayerName2);

	if (pLayer1 == nullptr || pLayer2 == nullptr)
		assert(false);

	int iIdx1 = pLayer1->GetLayerIdx();
	int iIdx2 = pLayer2->GetLayerIdx();

	int iMin = min(iIdx1, iIdx2);
	int iMax = max(iIdx1, iIdx2);


	m_LayerCheck[iMin] &= ~(1 << iMax); // ������ ��Ʈ�� �״�� �ش� �ڸ��� �ݵ�� 0
}

void CCollisionMgr::CheckCollisionLayer(int iLayerIdx1, int iLayerIdx2)
{
	// Layer�� �ε����� �־��ָ� �� �� �ε����� ����, ��Ʈ�� �ٲ��ش�.

	CScene* pCurScene = CSceneMgr::GetInst()->GetScene();

	// �־��� ���̾� �ε����� ������ �غ���, ��� �� ���̶� �� ��� assert
	if (!pCurScene->GetLayer(iLayerIdx1) || !pCurScene->GetLayer(iLayerIdx2))
		assert(false);

	// ���� �ε����� 0 ~ 31���̰� �ƴ� ��쿡�� �⺻������ assertó���� �ȴ�.


	// �ε����� ������ ����, �� �ε����� �ش��ϴ� Layer�� ����� ���縦 �ϴ� ��쿡�� �浹ó���� true�� ���ش�.

	int iMin = min(iLayerIdx1, iLayerIdx2);
	int iMax = max(iLayerIdx1, iLayerIdx2);

	m_LayerCheck[iMin] |= (1 << iMax);
}

void CCollisionMgr::UnCheckCollisionLayer(int iLayerIdx1, int iLayerIdx2)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetScene();

	if (!pCurScene->GetLayer(iLayerIdx1) || !pCurScene->GetLayer(iLayerIdx2))
		assert(false);

	int iMin = min(iLayerIdx1, iLayerIdx2);
	int iMax = max(iLayerIdx1, iLayerIdx2);

	m_LayerCheck[iMin] &= ~(1 << iMax);
}


void CCollisionMgr::CollisionLayer(const CLayer * pLayer1, const CLayer * pLayer2)
{
	// �� ���̾ �ش��ϴ� ������Ʈ�� ���´�.
	const vector<CGameObject*>& vecObj1 = pLayer1->GetLayerObjVec();
	const vector<CGameObject*>& vecObj2 = pLayer2->GetLayerObjVec();

	if (pLayer1->GetLayerIdx() != TILE_LAYER_IDX && pLayer2->GetLayerIdx() != TILE_LAYER_IDX)
	{
		map<DWORD_PTR, bool>::iterator iter;

		for (size_t i = 0; i < vecObj1.size(); ++i)
		{
			CCollider2D* pCollider1 = vecObj1[i]->GetCollider2D();

			// �浹ü�� ������ ���� ���� ��쿡 ���� ������Ʈ..
			if (pCollider1 == nullptr)
				continue;

			// ���� ������, �浹���迡 �ִ� ��ü�� ��Ȱ��ȭ�ϰų�, 
			// Collider2D Component�� ��Ȱ��ȭ �ߴٰ� �ص�, 
			// �ڽŰ� �浹�� ��ü�� �˰� ���� �ʾƼ� Exitó���� ���ֱ� ��ƴ�.
			// ����, ��ü / Collider2D �� ��Ȱ��ȭ��� �ϴ��� �浹ó���� �����Ѵ�.
			// �׷��� �ڵ����� Exit���� ȣ���� �� ���̴�. (�� ���ǿ��� �ݵ�� �浹�� false�� �ǵ���)

			size_t j = 0;

			// ���� Layer�� ó���ϰ� �ִ� ���
			if (pLayer1 == pLayer2)
				j = i + 1;

			for (; j < vecObj2.size(); ++j)
			{
				CCollider2D* pCollider2 = vecObj2[j]->GetCollider2D();

				// A��ü�� �浹ü�� �ִµ� B��ü�� �浹ü�� ������ ���� �����, �ٸ� ��ü... continue;
				if (pCollider2 == nullptr)
					continue;



				ColID id;
				id.iColID1 = pCollider1->GetColID();
				id.iColID2 = pCollider2->GetColID();
				iter = m_mapCol.find(id.ID); // 4byte 4byte�� �־, ���� ���� Ű ������ �־��ش�.

				bool IsActive = true;

				// ��� ������ ������Ʈ�� ���� ��쿡 ���� ó���� ���ش�.
				if ((pCollider1->GetObj()->IsActive() == false) || (pCollider2->GetObj()->IsActive() == false))
					IsActive = false;

				if (IsCollision(pCollider1, pCollider2))
				{
					// �浹�� �Ͼ ���


					// �� �浹ü�� �ش��ϴ� ������ �ƿ� ���� �����̴�. 
					if (iter == m_mapCol.end())
					{
						// �߰��ϰ�, �浹���¸� true�� �Ѵ�.

						if (IsActive == false)
							continue;
						// ���� ���¿��� �ٸ� ������Ʈ�� �浹�� ��쿡�� ó������
						// Counting���� �ʰ� �ƿ�ó���� ���� ����.

						m_mapCol.insert(make_pair((DWORD_PTR)id.ID, true));

						// ó�� �浹.
						pCollider1->OnCollisionEnter(pCollider2);
						pCollider2->OnCollisionEnter(pCollider1);

					}

					// ������ �� �ִ� ���
					else if (iter->second == false)
					{
						// ���� �浹�ε�, �������� �浹�� �ƴ� ��� (ó�� �浹)

						if (IsActive == false)
							continue;

						iter->second = true;

						pCollider1->OnCollisionEnter(pCollider2);
						pCollider2->OnCollisionEnter(pCollider1);
					}
					else
					{
						// true�� ���. �浹���̴�.

						if (IsActive == false)
						{
							// ������ ����� ���̱� ������, �浹�� false ó���ϰ�, Counting�� �����Ѵ�.
							pCollider1->OnCollisionExit(pCollider2);
							pCollider2->OnCollisionExit(pCollider1);
							iter->second = false;
						}
						else
						{
							pCollider1->OnCollision(pCollider2);
							pCollider2->OnCollision(pCollider1);
						}
					}

				}
				else
				{
					// �浹�� �Ͼ�� ���� ���

					if (iter == m_mapCol.end())
					{
						// ������ �� ���� �����ϱ�, �߰��� �ؾ� �ϳ� ? 
						// �ٵ�, ��¥��, �浹�� �Ǵ� ���� �ڵ����� �߰��� �Ǵ� �κ��ε�, ���� �浹�� ���� ���� ��Ȳ����
						// ����ؼ� ������ ���� �ʿ䰡 �ִ����� �� �𸣰ڴ�. -> �ʿ䰡 ����.
					}
					else
					{
						// ������ �� �ִ� ���.

						if (iter->second == true)
						{
							// ������ �浹�� �Ǿ���, ������ �浹�� �ƴ� ����̴�. false ó���� ���ش�.
							iter->second = false;

							// �浹�� ���� ���̴�.
							pCollider1->OnCollisionExit(pCollider2);
							pCollider2->OnCollisionExit(pCollider1);
						}
						//else ������ �浹�� ���� �ʾҰ�, ���ݵ� �浹�� �ƴϴ� => �ƹ� �͵� �ƴϴ�.

					}
				}

			}
		}


	}
	else
	{
		// ��� ������ Tile�� ����̴�.
		// Tile Layer�� TileLayer�� �浹�ϴ� ���� ������ �� ���ٰ� ����

		if (pLayer1->GetLayerIdx() == TILE_LAYER_IDX)
		{
			// pLayer1 : TILE LAYER  vecObj1
			// pLayer2 : LAYER		 vecObj2

			for (size_t i = 0; i < vecObj1.size(); ++i)
			{
				for (size_t j = 0; j < vecObj2.size(); ++j)
				{
					TileCollision(vecObj1[i], vecObj2[j]);
				}
			}

		}
		else
		{
			// pLayer1 : LAYER			 vecObj1
			// pLayer2 : TILE LAYER		 vecObj2

			for (size_t i = 0; i < vecObj1.size(); ++i)
			{
				for (size_t j = 0; j < vecObj2.size(); ++j)
				{
					TileCollision(vecObj2[i], vecObj1[j]);
				}
			}
		}


	}
}

bool CCollisionMgr::IsCollision(CCollider2D * pCollider1, CCollider2D * pCollider2)
{
	if (!pCollider1->IsEnable() || !pCollider2->IsEnable() // �浹ü�� ��Ȱ��
		|| !pCollider1->GetObj()->IsEnable() || !pCollider2->GetObj()->IsEnable()) // ������Ʈ�� ��Ȱ��
		return false; // �ݵ�� false

	switch (pCollider1->GetColliderType())
	{
	case COLLIDER2D_TYPE::RECT:

		switch (pCollider2->GetColliderType())
		{
		case COLLIDER2D_TYPE::RECT:
			return CollisionRectToRect(pCollider1, pCollider2);

		case COLLIDER2D_TYPE::CIRCLE:
			return CollisionRectToCircle(pCollider1, pCollider2);
		}

		break;

	case COLLIDER2D_TYPE::CIRCLE:

		switch (pCollider2->GetColliderType())
		{
		case COLLIDER2D_TYPE::RECT:
			return CollisionRectToCircle(pCollider2, pCollider1);

		case COLLIDER2D_TYPE::CIRCLE:
			return CollisionCircleToCircle(pCollider1, pCollider2);
		}

		break;
	}

	return false;
}

bool CCollisionMgr::CollisionRectToRect(CCollider2D * pCollider1, CCollider2D * pCollider2)
{

	// 0 -- 1
	// |	|
	// 3 -- 2

	// �簢�� Mesh���� ������ ������ �����´�.
	static Vec3 arrLocal[4] = {
		Vec3(-0.5f, 0.5f, 0.f),
		Vec3(0.5f, 0.5f, 0.f),
		Vec3(0.5f, -0.5f, 0.f),
		Vec3(-0.5f, -0.5f, 0.f)
	};

	const Matrix& matCol1 = pCollider1->GetColliderWorldMat();
	const Matrix& matCol2 = pCollider2->GetColliderWorldMat();

	Vec3 arrCol1[4] = {}; // Colllider1�� ���� World ��ǥ�� ���� �뵵�̴�. 
	Vec3 arrCol2[4] = {}; // Collider2
	Vec3 arrCenter[2] = {}; // �� �浹ü�� Center�� �Ű��� ��ǥ�� ���� �뵵�̴�.

	for (UINT i = 0; i < 4; ++i)
	{
		// XMVector3TransformCoord �Լ��� , Vec3���� �޾Ƽ�, ������ ���ڿ� 1.f�� �־,
		// �־��� ��İ� �����ش�.

		// �浹ü�� WorldMatrix�� ������ ��ǥ�� ���ؼ�, �Ű��� ��ǥ�� ���س���.
		arrCol1[i] = XMVector3TransformCoord(arrLocal[i], matCol1);
		arrCol2[i] = XMVector3TransformCoord(arrLocal[i], matCol2);

		// 2D �浹�̱� ������ Z���� �����ϰ� �����ش�. ���࿡ Z���� �ٸ��ٰ� �ϴ���
		// ���� ���������� Z���� ������ �޾Ƽ� ����� ���� �ʴ´�. ���� ���̴� �״�� �浹�� ���� ó���� �ϱ� ���ؼ���
		// Z���� �����ϰ� �ؼ� ó���� �ؾ� �Ѵ�.
		arrCol1[i].z = 0.f;
		arrCol2[i].z = 0.f;
	}

	// Rect�� ������ �� �浹ü�� WorldMatrix �� ���ؼ� �Ű��� ��ǥ�� �����ش�. arrcol1 arrcol2

	// Center ���� �Ű��ִ� ó���� �Ѵ�. -> Center�� x y z ��� 0.f�̴�. (�״�� ���� �ȴ�)
	arrCenter[0] = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol1);
	arrCenter[1] = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol2);

	arrCenter[0].z = 0.f;
	arrCenter[1].z = 0.f;

	// �� �浹ü�� �߽ɿ��� �߽����� ���� ���͸� ���س���.
	Vec3 vCenter = arrCenter[1] - arrCenter[0];

	// �� �浹ü�� ���͸� �� ���� �̾Ƴ���.
	Vec3 arrOrginVec[4] = {
		arrCol1[0] - arrCol1[3],
		arrCol1[2] - arrCol1[3],
		arrCol2[0] - arrCol2[3],
		arrCol2[2] - arrCol2[3]
	};

	//  �̷��� ���� �浹ü�� ���͵���, ���������̱⵵ �ϸ鼭 (����ȭ �ʿ�), ������ ��ų �����̱⵵ �ϴ�.

	Vec3 arrProjVec[4] = {};

	for (UINT i = 0; i < 4; ++i)
	{
		arrOrginVec[i].Normalize(arrProjVec[i]);
		// �� �浹ü���� �̾Ƴ� ���͸� ����, ����ȭ�� �����Ͽ�, ũ�Ⱑ 1�� �������͸� ������.
	}

	// ������ ���ؼ� �и��� �׽�Ʈ
	// vCenter		 : �� �簢���� �߽��� �մ� ����
	// arrOriginVec	 : �� �簢���� ǥ�� ����
	// arrProjVec	 : �簢���� ǥ��� ������ ������ ���ͷ� ���������̴�.

	// �������� ����(��������)�� �簢���� ǥ�麤�͸� �����ϸ�, �ش� ���͸� �������� ���� ���̸� ���س� �� �ִ�.

	for (UINT i = 0; i < 4; ++i)
	{
		float fCenter = abs(vCenter.Dot(arrProjVec[i]));
		// �߽��� �մ� ���Ϳ�, �������͸� �����Ͽ� �̰��� ������Ų �Ÿ��� ���Ѵ�.
		// ������ ���� ������ ���� ���ɼ���(�а�) �����Ƿ� ������ �����ش�.

		float fAcc = 0.f;
		for (UINT j = 0; j < 4; ++j)
			fAcc += abs(arrOrginVec[j].Dot(arrProjVec[i]));

		// �̷��� ������ �ϳ��� ��� �� �������� ���͸� �� ǥ�麤�Ϳ� �����Ͽ�, ������ ���̸� ���ؼ�,
		// �̰��� ��ģ��. �� ������ ������ �� �߽��� �մ� ���͸� ������Ų ���̿� ���Ͽ� ó���� �޶����� �ȴ�.

		fAcc /= 2.f;

		if (fCenter > fAcc)
			return false;
		// 4���� ��� �߿��� �ѹ��̶� �߽����� �մ� ���͸� ������Ų ���̰� �� �� ��쿡�� �浹�� �ƴϴ�.
	}


	// 4��� ��� �� �߽����� �մ� ���͸� ������Ų ���̰� �� �۴ٸ�, �浹�̴�.
	return true;
}

bool CCollisionMgr::CollisionRectToCircle(CCollider2D * pCollider1, CCollider2D * pCollider2)
{
	// �簢���� ���� �浹�� ��쿡�� �簢���� ȸ������ ��ŭ �ٽ� ������ ���Ѿ� �Ѵ�.

	// �簢���� Rotation���� �����´�.
	const Vec3& tRectRot = pCollider1->GetObj()->GetTransform()->GetLocalRot();

	Matrix matRot = XMMatrixRotationX(-tRectRot.x);
	matRot *= XMMatrixRotationY(-tRectRot.y);
	matRot *= XMMatrixRotationZ(-tRectRot.z);

	// World�� �ִ� ��ü�� Rotation���� ������ �Ǿ��� ���̴�. 
	// 30���� ȸ���� �̷����ٸ�, -30��... ��ŭ ȸ���� ��Ų��.


	// �� �浹ü�� Collider�� �����´�.
	const Matrix& matCol1 = pCollider1->GetColliderWorldMat();
	const Matrix& matCol2 = pCollider2->GetColliderWorldMat();

	// �浹ü�� ColWorldMat�� �ݴ� ������ ȸ������� �����ش�.

	Matrix matRotRect = matCol1 * matRot;
	Matrix matRotCircle = matCol2 * matRot;

	// �� ����� Translation,  Scale�� ���� ���������, Rotation�� ���ؼ��� ó������ �ʴ� ����̴�.

	// �� ����� ���ؼ� �Ű��� ��ǥ�� �����ش�.

	static Vec3 arrCircleLocal[2] = {
		Vec3(0.f, 0.f, 0.f),
		Vec3(0.5f, 0.f, 0.f)
	};


	// �簢�� Mesh���� ������ ������ �����´�.
	static Vec3 arrRectLocal[4] = {
		Vec3(-0.5f, 0.5f, 0.f),
		Vec3(0.5f, 0.5f, 0.f),
		Vec3(0.5f, -0.5f, 0.f),
		Vec3(-0.5f, -0.5f, 0.f)
	};

	Vec3 arrCircle[2] = {};
	Vec3 arrRect[4] = {};

	// ���� �߽ɰ� �� ���� �ű��.
	arrCircle[0] = XMVector3TransformCoord(arrCircleLocal[0], matRotCircle);
	arrCircle[1] = XMVector3TransformCoord(arrCircleLocal[1], matRotCircle);

	arrCircle[0].z = 0.f;
	arrCircle[1].z = 0.f;

	float fRadius = GetLength(arrCircle[0] - arrCircle[1]); // z = 0 �̶� ����� ���� �ʴ´�.

	for (UINT i = 0; i < 4; ++i)
	{
		arrRect[i] = XMVector3TransformCoord(arrRectLocal[i], matRotRect);
		arrRect[i].z = 0.f;
	}

	// ���� Rect To Circle �浹�� �����Ѵ�.


	// 0 -- 1
	// |	|
	// 3 -- 2

	// ���� �߽��� ���� �¿쿡 �ִ� ��� 
	if ((arrCircle[0].x >= arrRect[0].x && arrCircle[0].x <= arrRect[1].x) ||
		(arrCircle[0].y <= arrRect[0].y && arrCircle[0].y >= arrRect[3].y))
	{
		arrRect[0].x -= fRadius;
		arrRect[0].y += fRadius;

		arrRect[1].x += fRadius;
		arrRect[1].y += fRadius;

		arrRect[2].x += fRadius;
		arrRect[2].y -= fRadius;

		arrRect[3].x -= fRadius;
		arrRect[3].y -= fRadius;

		// Ȯ��� �簢�� ���ο� ���� �߽��� �� �ִ� ��� �浹�̴�.

		if (arrCircle[0].x >= arrRect[0].x && arrCircle[0].x <= arrRect[1].x &&
			arrCircle[0].y <= arrRect[0].y && arrCircle[0].y >= arrRect[3].y)
			return true;


		return false;
	}
	else
	{
		// ���� �߽��� �𼭸��� �ִ� ���
		// �簢���� 4���� ������ ���� �߽� ������ �Ÿ��� ���Ͽ�, �������� ���̸� ������ �Ǵ��Ѵ�.

		for (UINT i = 0; i < 4; ++i)
		{
			if (fRadius >= GetLength(arrCircle[0].x, arrCircle[0].y, arrRect[i].x, arrRect[i].y))
				return true;

			// �ϳ��� ���� �����ϴ� ��� �浹�̴�.
		}

		// 4���� ��츦 ���������� ���⼭ �ɸ��� ������ �浹�� �ƴϴ�.
	}



	return false;
}

bool CCollisionMgr::CollisionCircleToCircle(CCollider2D * pCollider1, CCollider2D * pCollider2)
{
	// �߽ɿ� ���� World��ǥ�� ���Ѵ�.

	const Matrix& matCol1 = pCollider1->GetColliderWorldMat();
	const Matrix& matCol2 = pCollider2->GetColliderWorldMat();

	Vec3 arrCenter[2] = {};
	Vec3 arrRadius[2] = {};

	arrCenter[0] = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol1);
	arrCenter[1] = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol2);

	arrCenter[0].z = 0.f;
	arrCenter[1].z = 0.f;

	// ������ �ű��
	arrRadius[0] = XMVector3TransformCoord(Vec3(0.5f, 0.f, 0.f), matCol1);
	arrRadius[1] = XMVector3TransformCoord(Vec3(0.5f, 0.f, 0.f), matCol2);

	arrRadius[0].z = 0.f;
	arrRadius[1].z = 0.f;


	// �� �߽� ������ �Ÿ��� ���Ѵ�.
	Vec3 vCenter = arrCenter[0] - arrCenter[1];

	float fCenter = sqrt(vCenter.x * vCenter.x +
		vCenter.y * vCenter.y +
		vCenter.z * vCenter.z);

	Vec3 vRadius[2] = {
		arrRadius[0] - arrCenter[0],
		arrRadius[1] - arrCenter[1]
	};

	// �������� ������ ���� ���Ѵ�.
	float fRadius = sqrt(vRadius[0].x * vRadius[0].x +
		vRadius[0].y * vRadius[0].y +
		vRadius[0].z * vRadius[0].z) +

		sqrt(vRadius[1].x * vRadius[1].x +
			vRadius[1].y * vRadius[1].y +
			vRadius[1].z * vRadius[1].z);


	// �߽� ������ �Ÿ��� �� ũ�ٸ� �浹�� �ƴϴ�.
	if (fRadius < fCenter)
		return false;

	return true;
}
