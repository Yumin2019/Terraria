
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
	// 현재 장면을 가져온다.
	CScene* pCurScene = CSceneMgr::GetInst()->GetScene();

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		// 행을 순회한다.

		for (int j = 0; j < MAX_LAYER; ++j)
		{
			// 열을 순회한다.

			// i행에 해당하는 Layer와 j열에 해당하는 bit 자리가 1인 경우 (충돌처리를 하겠다는 의미)
			// Layer에 대한 충돌처리를 진행한다.

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

	// 충돌체를 가지지 않은 경우 return.
	if (!pTileObj->GetCollider2D() || !pObj->GetCollider2D())
		return;

	// 충돌이 되지 않은 경우 return.
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

	// X 에 대한 처리.
	if (TileCollWidth(pObj, pTileObj->GetTileMap2D(), fResult, tTileRC, iDir))
	{
		// 충돌이 일어났고, fResult를 구해놓았다.

		if (iDir == -1)
		{
			// 왼쪽으로 이동 :  오른쪽 벽과 충돌.
			// 현재 위치에서의 왼쪽 상단을 구한다.
			
			Vec2 tPrevLT = tPrevPos + tTrans;
			Vec2 tLT = tPos + tTrans;

			// tTileRC. r(z)이 tPrevLT.x 보다는 작고,
			//			    tLT.x 보다는 크다.
			if (tTileRC.z < tPrevLT.x && tTileRC.z > tLT.x)
			{
				// 파고든 경우 처리. World 겠지.. World 겠지 ?
				pObjTrans->SetLocalPos(pObjTrans->GetLocalPos() + Vec3(fResult, 0.f, 0.f));
				pObjTrans->SetMoveX(true, -1.f);
				bTileCollision = true;
			}
			
		}
		else if (iDir == 1)
		{
			// 오른쪽으로 이동 : 왼쪽 벽과 충돌.
			// 현재 위치의 오른쪽 상단을 구한다.
			Vec2 tPrevRB = tPrevPos - tTrans;
			Vec2 tPosRB = tPos - tTrans;

			// tTileRC.l(x)가 tPrevRB.x 보다 크고,
			//				  tPosRB.x  보다 작다.

			if (tTileRC.x > tPrevRB.x && tTileRC.x < tPosRB.x)
			{
				pObjTrans->SetLocalPos(pObjTrans->GetLocalPos() + Vec3(fResult, 0.f, 0.f));
				bTileCollision = true;
				pObjTrans->SetMoveX(true, 1.f);
			}
		}
	}

	// Y 이동 처리.

	iDir = 0;

	if (tMove.y < 0.f)
		iDir = -1;

	else if (tMove.y > 0.f)
		iDir = 1;

	if (TileCollHeight(pObj, pTileObj->GetTileMap2D(), fResult, tTileRC, iDir))
	{
		if (iDir == 1)
		{
			// 위로 올라가는 경우.
			// PrevLT 와 LT의 y값을 가지고 판단한다.

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
			// 아래로 내려가는 경우.
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
	pObjTrans->SetTileColl(bTileCollision); // 충돌이 되었다면, true 아니면 false 이것을 Script에서 인식 가능. (다음 프레임에)

	if (bTileCollision == true)
		pObjTrans->TileCollision();
}


bool CCollisionMgr::TileCollHeight(CGameObject * pObj, CTileMap2D* pTileMap, float & fResult, Vec4 & tTileRC, int iDir)
{
	// 아래로 떨어졌다면, 타일의 위쪽을 

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
		// 아래로 떨어지는 경우. 
		// 하단 직선을 구함.
		tStart = Vec2(tLT.x, tLT.y - tScale.y);
		tEnd = Vec2(tLT.x + tScale.x, tStart.y);

	}
	else
	{
		// 위로 올라가는 경우
		// 상단 직선을 구함.
		tStart = Vec2(tLT.x, tLT.y);
		tEnd = Vec2(tStart.x + tScale.x, tStart.y);
	}

	// 3가지 정보.
	// y 위치에 해당하는 인덱스 (start, end 동일)
	// 왼쪽에서 뽑을 수 있는 x 인덱스 start
	// 오른쪽에서 뽑을 수 있는 x 인덱스 end

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
			// 위 / 아래 이동을 가지고 찾아낸 선분에 해당하는 Tile을 확인하니...
			// 그곳에 벽이 있었답니다. 

			Vec2 tTileLT = pTileMap->GetTileLT(x, iY);
			Vec2 tTileSize = pTileMap->GetTileSize();

			// l t r b
			tTileRC.x = tTileLT.x;
			tTileRC.y = tTileLT.y;
			tTileRC.z = tTileLT.x + tTileSize.x;
			tTileRC.w = tTileLT.y - tTileSize.y;

			if (iDir == 1)
			{
				// 위로 이동하는 경우.
				// RESULT는 음수가 나와야 한다.
				fResult = tTileRC.w - tStart.y - 0.1f;
			}
			else
			{
				// 아래로 이동하는 경우
				// RESULT는 양수가 나와야 한다.
				fResult = tTileRC.y - tStart.y + 0.1f;
			}

			return true;
		}
	}

	return false;
}

bool CCollisionMgr::TileCollWidth(CGameObject * pObj, CTileMap2D* pTileMap, float & fResult, Vec4 & tTileRC, int iDir)
{
	// 왼쪽으로 이동을 했다면, 타일의 오른쪽 면과 충돌했는지 확인하고,
	// 오른쪽으로 이동을 했다면, 타일의 왼쪽면이 충돌했는지 확인한다.

	if (iDir == 0)
		return false;

	CTransform* pTrans = pObj->GetTransform();
	Vec3 tPos = pTrans->GetWorldPos();
	Vec2 tPivot = pTrans->GetPivot();
	Vec3 tScale = pTrans->GetWorldScale() * pObj->GetCollider2D()->GetScale();

	// 왼쪽 위를 구한다.
	// pivot의 값을 토대로 LT를 구하기 위해 움직여야 하는 비율을 구한다.
	tPivot.x *= -1.f;
	tPivot.y = (1.f - tPivot.y);

	Vec3 tTrans = Vec3(tPivot.x, tPivot.y, 1.f) * tScale;
	Vec3 tLT = tPos + tTrans;

	Vec2 tStart, tEnd;


	if (iDir == -1)
	{
		// 왼쪽으로 이동하는 경우
		// 왼쪽 상단부터 하단의 위치를 구한다.
		tStart = Vec2(tLT.x, tLT.y);
		tEnd = Vec2(tLT.x, tLT.y - tScale.y);

	}
	else
	{
		// 오른쪽으로 이동하는 경우
		// 오른쪽 상단부터 하단의 위치를 구한다.
		tStart = Vec2(tLT.x + tScale.x, tLT.y);
		tEnd = Vec2(tStart.x, tLT.y - tScale.y);
	}

	// 3가지 정보.
	// x 위치에 해당하는 인덱스 (start, end 동일)
	// 상단에서 뽑을 Y Idx Start.y
	// 하단에서 뽑을 Y Idx End.y

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

	// 잘못된 범위가 나오는 경우 false 처리.

	for (int y = iStartY; y <= iEndY; ++y)
	{
		int idx = y * 100 + iX;
		if (pTileMap->IsTileBlock(idx))
		{
			// 왼쪽 / 오른쪽 이동을 가지고 찾아낸 선분에 해당하는 Tile을 확인하니...
			// 그곳에 벽이 있었답니다. 

			Vec2 tTileLT = pTileMap->GetTileLT(iX, y);
			Vec2 tTileSize = pTileMap->GetTileSize();

			// l t r b
			tTileRC.x = tTileLT.x;
			tTileRC.y = tTileLT.y;
			tTileRC.z = tTileLT.x + tTileSize.x;
			tTileRC.w = tTileLT.y - tTileSize.y;

			if (iDir == 1)
			{
				// 오른쪽으로 이동하는 경우.
				// RESULT는 음수가 나와야 한다.
				fResult = tTileRC.x - tStart.x - 0.1f;
			}
			else
			{
				// 왼쪽으로 이동하는 경우.
				// RESULT는 양수가 나와야 한다.
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
	// Layer Render 이후에 Tool에서 찾아간다. 
	// 따라서 Vec에 오브젝트가 항상 존재.

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

	// Layer의 이름을 넣어주면, 두 Layer에 대해서 충돌처리를 하기 위해서, 해당 Layer의 이름을 토대로 행과 열을 잡아서,
	// LayerArray의 자리비트에 1값을 넣어주는 처리를 하고 있다.

	CLayer* pLayer1 = pCurScene->FindLayer(strLayerName1);
	CLayer* pLayer2 = pCurScene->FindLayer(strLayerName2);

	// 한 쪽이라도, nullptr가 나오는 경우. 키 값을 잘못 넣어준 경우에는 assert를 반환.
	if (pLayer1 == nullptr || pLayer2 == nullptr)
		assert(false);

	int iIdx1 = pLayer1->GetLayerIdx();
	int iIdx2 = pLayer2->GetLayerIdx();

	int iMin = min(iIdx1, iIdx2);
	int iMax = max(iIdx1, iIdx2);

	// Layer의 순서는 행이 작고, 열이 크다.

	m_LayerCheck[iMin] |= (1 << iMax);
	// 만약 iMin = iMax = 1 이라면, 
	// 1행의 2번째 (1에 해당하는 bit)에 접근하게 된다.

	// 이렇게 각 비트로 이동해서, 그 자리를 OR연산을 통해서 1로 만들어둔다.

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


	m_LayerCheck[iMin] &= ~(1 << iMax); // 나머지 비트는 그대로 해당 자리는 반드시 0
}

void CCollisionMgr::CheckCollisionLayer(int iLayerIdx1, int iLayerIdx2)
{
	// Layer의 인덱스를 넣어주면 이 두 인덱스를 토대로, 비트를 바꿔준다.

	CScene* pCurScene = CSceneMgr::GetInst()->GetScene();

	// 넣어준 레이어 인덱스에 접근을 해보니, 어느 한 쪽이라도 빈 경우 assert
	if (!pCurScene->GetLayer(iLayerIdx1) || !pCurScene->GetLayer(iLayerIdx2))
		assert(false);

	// 만약 인덱스가 0 ~ 31사이가 아닌 경우에도 기본적으로 assert처리가 된다.


	// 인덱스에 문제가 없고, 두 인덱스에 해당하는 Layer가 제대로 존재를 하는 경우에는 충돌처리를 true로 해준다.

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
	// 두 레이어에 해당하는 오브젝트를 얻어온다.
	const vector<CGameObject*>& vecObj1 = pLayer1->GetLayerObjVec();
	const vector<CGameObject*>& vecObj2 = pLayer2->GetLayerObjVec();

	if (pLayer1->GetLayerIdx() != TILE_LAYER_IDX && pLayer2->GetLayerIdx() != TILE_LAYER_IDX)
	{
		map<DWORD_PTR, bool>::iterator iter;

		for (size_t i = 0; i < vecObj1.size(); ++i)
		{
			CCollider2D* pCollider1 = vecObj1[i]->GetCollider2D();

			// 충돌체를 가지고 있지 않은 경우에 다음 오브젝트..
			if (pCollider1 == nullptr)
				continue;

			// 현재 구조상, 충돌관계에 있던 물체를 비활성화하거나, 
			// Collider2D Component를 비활성화 했다고 해도, 
			// 자신과 충돌한 물체를 알고 있지 않아서 Exit처리를 해주기 어렵다.
			// 따라서, 물체 / Collider2D 가 비활성화라고 하더라도 충돌처리는 진행한다.
			// 그러면 자동으로 Exit까지 호출이 될 것이다. (이 조건에서 반드시 충돌은 false로 되도록)

			size_t j = 0;

			// 같은 Layer를 처리하고 있는 경우
			if (pLayer1 == pLayer2)
				j = i + 1;

			for (; j < vecObj2.size(); ++j)
			{
				CCollider2D* pCollider2 = vecObj2[j]->GetCollider2D();

				// A물체는 충돌체가 있는데 B물체가 충돌체를 가지지 않은 경우라면, 다른 물체... continue;
				if (pCollider2 == nullptr)
					continue;



				ColID id;
				id.iColID1 = pCollider1->GetColID();
				id.iColID2 = pCollider2->GetColID();
				iter = m_mapCol.find(id.ID); // 4byte 4byte를 넣어서, 만든 것을 키 값으로 넣어준다.

				bool IsActive = true;

				// 어느 한쪽의 오브젝트가 죽은 경우에 예외 처리를 해준다.
				if ((pCollider1->GetObj()->IsActive() == false) || (pCollider2->GetObj()->IsActive() == false))
					IsActive = false;

				if (IsCollision(pCollider1, pCollider2))
				{
					// 충돌이 일어난 경우


					// 두 충돌체에 해당하는 정보가 아예 없는 상태이다. 
					if (iter == m_mapCol.end())
					{
						// 추가하고, 충돌상태를 true로 한다.

						if (IsActive == false)
							continue;
						// 죽은 상태에서 다른 오브젝트와 충돌한 경우에는 처음부터
						// Counting하지 않고 아예처리를 하지 않음.

						m_mapCol.insert(make_pair((DWORD_PTR)id.ID, true));

						// 처음 충돌.
						pCollider1->OnCollisionEnter(pCollider2);
						pCollider2->OnCollisionEnter(pCollider1);

					}

					// 정보는 들어가 있는 경우
					else if (iter->second == false)
					{
						// 지금 충돌인데, 이전에는 충돌이 아닌 경우 (처음 충돌)

						if (IsActive == false)
							continue;

						iter->second = true;

						pCollider1->OnCollisionEnter(pCollider2);
						pCollider2->OnCollisionEnter(pCollider1);
					}
					else
					{
						// true인 경우. 충돌중이다.

						if (IsActive == false)
						{
							// 한쪽이 사라질 것이기 때문에, 충돌을 false 처리하고, Counting을 감소한다.
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
					// 충돌이 일어나지 않은 경우

					if (iter == m_mapCol.end())
					{
						// 정보가 들어가 있지 않으니까, 추가를 해야 하나 ? 
						// 근데, 어짜피, 충돌이 되는 순간 자동으로 추가가 되는 부분인데, 굳이 충돌이 되지 않은 상황까지
						// 고려해서 정보를 넣을 필요가 있는지는 잘 모르겠다. -> 필요가 없다.
					}
					else
					{
						// 정보가 들어가 있는 경우.

						if (iter->second == true)
						{
							// 이전에 충돌이 되었고, 지금은 충돌이 아닌 경우이다. false 처리를 해준다.
							iter->second = false;

							// 충돌이 끝난 것이다.
							pCollider1->OnCollisionExit(pCollider2);
							pCollider2->OnCollisionExit(pCollider1);
						}
						//else 이전에 충돌이 되지 않았고, 지금도 충돌이 아니다 => 아무 것도 아니다.

					}
				}

			}
		}


	}
	else
	{
		// 어느 한쪽은 Tile인 경우이다.
		// Tile Layer와 TileLayer가 충돌하는 경우는 존재할 수 없다고 하자

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
	if (!pCollider1->IsEnable() || !pCollider2->IsEnable() // 충돌체가 비활성
		|| !pCollider1->GetObj()->IsEnable() || !pCollider2->GetObj()->IsEnable()) // 오브젝트가 비활성
		return false; // 반드시 false

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

	// 사각형 Mesh에서 정점의 정보를 가져온다.
	static Vec3 arrLocal[4] = {
		Vec3(-0.5f, 0.5f, 0.f),
		Vec3(0.5f, 0.5f, 0.f),
		Vec3(0.5f, -0.5f, 0.f),
		Vec3(-0.5f, -0.5f, 0.f)
	};

	const Matrix& matCol1 = pCollider1->GetColliderWorldMat();
	const Matrix& matCol2 = pCollider2->GetColliderWorldMat();

	Vec3 arrCol1[4] = {}; // Colllider1의 정점 World 좌표를 담을 용도이다. 
	Vec3 arrCol2[4] = {}; // Collider2
	Vec3 arrCenter[2] = {}; // 두 충돌체의 Center가 옮겨질 좌표를 담을 용도이다.

	for (UINT i = 0; i < 4; ++i)
	{
		// XMVector3TransformCoord 함수는 , Vec3형을 받아서, 마지막 인자에 1.f를 넣어서,
		// 넣어준 행렬과 곱해준다.

		// 충돌체의 WorldMatrix를 정점의 좌표에 곱해서, 옮겨진 좌표를 구해낸다.
		arrCol1[i] = XMVector3TransformCoord(arrLocal[i], matCol1);
		arrCol2[i] = XMVector3TransformCoord(arrLocal[i], matCol2);

		// 2D 충돌이기 때문에 Z값을 동일하게 맞춰준다. 만약에 Z값이 다르다고 하더라도
		// 직교 투영에서는 Z값의 영향을 받아서 출력이 되지 않는다. 따라서 보이는 그대로 충돌에 대한 처리를 하기 위해서는
		// Z값을 동일하게 해서 처리를 해야 한다.
		arrCol1[i].z = 0.f;
		arrCol2[i].z = 0.f;
	}

	// Rect의 정점을 각 충돌체의 WorldMatrix 를 곱해서 옮겨진 좌표를 구해준다. arrcol1 arrcol2

	// Center 또한 옮겨주는 처리를 한다. -> Center는 x y z 모두 0.f이다. (그대로 쓰면 된다)
	arrCenter[0] = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol1);
	arrCenter[1] = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol2);

	arrCenter[0].z = 0.f;
	arrCenter[1].z = 0.f;

	// 두 충돌체의 중심에서 중심으로 가는 벡터를 구해낸다.
	Vec3 vCenter = arrCenter[1] - arrCenter[0];

	// 각 충돌체의 벡터를 두 개씩 뽑아낸다.
	Vec3 arrOrginVec[4] = {
		arrCol1[0] - arrCol1[3],
		arrCol1[2] - arrCol1[3],
		arrCol2[0] - arrCol2[3],
		arrCol2[2] - arrCol2[3]
	};

	//  이렇게 구한 충돌체의 벡터들은, 투영벡터이기도 하면서 (정규화 필요), 투영을 시킬 벡터이기도 하다.

	Vec3 arrProjVec[4] = {};

	for (UINT i = 0; i < 4; ++i)
	{
		arrOrginVec[i].Normalize(arrProjVec[i]);
		// 각 충돌체에서 뽑아낸 벡터를 토대로, 정규화를 진행하여, 크기가 1인 투영벡터를 만들어낸다.
	}

	// 투영을 통해서 분리축 테스트
	// vCenter		 : 두 사각형의 중심을 잇는 벡터
	// arrOriginVec	 : 각 사각형의 표면 벡터
	// arrProjVec	 : 사각형의 표면과 평행한 투영축 벡터로 단위벡터이다.

	// 투영축의 벡터(단위벡터)와 사각형의 표면벡터를 내적하면, 해당 벡터를 투영했을 때의 길이를 구해낼 수 있다.

	for (UINT i = 0; i < 4; ++i)
	{
		float fCenter = abs(vCenter.Dot(arrProjVec[i]));
		// 중심을 잇는 벡터와, 투영벡터를 내적하여 이것을 투영시킨 거리를 구한다.
		// 내적의 값이 음수가 나올 가능성도(둔각) 있으므로 절댓값을 구해준다.

		float fAcc = 0.f;
		for (UINT j = 0; j < 4; ++j)
			fAcc += abs(arrOrginVec[j].Dot(arrProjVec[i]));

		// 이렇게 투영축 하나를 잡고 이 투영축의 벡터를 각 표면벡터와 내적하여, 투영된 길이를 구해서,
		// 이것의 합친다. 이 길이의 절반이 두 중심을 잇는 벡터를 투영시킨 길이와 비교하여 처리가 달라지게 된다.

		fAcc /= 2.f;

		if (fCenter > fAcc)
			return false;
		// 4번의 경우 중에서 한번이라도 중심축을 잇는 벡터를 투영시킨 길이가 더 긴 경우에는 충돌이 아니다.
	}


	// 4경우 모두 두 중심축을 잇는 벡터를 투영시킨 길이가 더 작다면, 충돌이다.
	return true;
}

bool CCollisionMgr::CollisionRectToCircle(CCollider2D * pCollider1, CCollider2D * pCollider2)
{
	// 사각형의 원의 충돌의 경우에는 사각형의 회전각도 만큼 다시 공전을 시켜야 한다.

	// 사각형의 Rotation값을 가져온다.
	const Vec3& tRectRot = pCollider1->GetObj()->GetTransform()->GetLocalRot();

	Matrix matRot = XMMatrixRotationX(-tRectRot.x);
	matRot *= XMMatrixRotationY(-tRectRot.y);
	matRot *= XMMatrixRotationZ(-tRectRot.z);

	// World에 있는 물체는 Rotation값이 적용이 되었을 것이다. 
	// 30도의 회전이 이뤄졌다면, -30도... 만큼 회전을 시킨다.


	// 각 충돌체의 Collider를 가져온다.
	const Matrix& matCol1 = pCollider1->GetColliderWorldMat();
	const Matrix& matCol2 = pCollider2->GetColliderWorldMat();

	// 충돌체의 ColWorldMat에 반대 방향의 회전행렬을 곱해준다.

	Matrix matRotRect = matCol1 * matRot;
	Matrix matRotCircle = matCol2 * matRot;

	// 이 행렬은 Translation,  Scale의 값을 고려하지만, Rotation에 대해서는 처리하지 않는 행렬이다.

	// 이 행렬을 통해서 옮겨진 좌표를 구해준다.

	static Vec3 arrCircleLocal[2] = {
		Vec3(0.f, 0.f, 0.f),
		Vec3(0.5f, 0.f, 0.f)
	};


	// 사각형 Mesh에서 정점의 정보를 가져온다.
	static Vec3 arrRectLocal[4] = {
		Vec3(-0.5f, 0.5f, 0.f),
		Vec3(0.5f, 0.5f, 0.f),
		Vec3(0.5f, -0.5f, 0.f),
		Vec3(-0.5f, -0.5f, 0.f)
	};

	Vec3 arrCircle[2] = {};
	Vec3 arrRect[4] = {};

	// 원의 중심과 한 점을 옮긴다.
	arrCircle[0] = XMVector3TransformCoord(arrCircleLocal[0], matRotCircle);
	arrCircle[1] = XMVector3TransformCoord(arrCircleLocal[1], matRotCircle);

	arrCircle[0].z = 0.f;
	arrCircle[1].z = 0.f;

	float fRadius = GetLength(arrCircle[0] - arrCircle[1]); // z = 0 이라서 계산이 되지 않는다.

	for (UINT i = 0; i < 4; ++i)
	{
		arrRect[i] = XMVector3TransformCoord(arrRectLocal[i], matRotRect);
		arrRect[i].z = 0.f;
	}

	// 이제 Rect To Circle 충돌을 진행한다.


	// 0 -- 1
	// |	|
	// 3 -- 2

	// 원의 중심이 상하 좌우에 있는 경우 
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

		// 확장된 사각형 내부에 원의 중심이 들어가 있는 경우 충돌이다.

		if (arrCircle[0].x >= arrRect[0].x && arrCircle[0].x <= arrRect[1].x &&
			arrCircle[0].y <= arrRect[0].y && arrCircle[0].y >= arrRect[3].y)
			return true;


		return false;
	}
	else
	{
		// 원의 중심이 모서리에 있는 경우
		// 사각형의 4개의 정점과 원의 중심 사이의 거리를 비교하여, 반지름의 길이를 가지고 판단한다.

		for (UINT i = 0; i < 4; ++i)
		{
			if (fRadius >= GetLength(arrCircle[0].x, arrCircle[0].y, arrRect[i].x, arrRect[i].y))
				return true;

			// 하나의 경우라도 만족하는 경우 충돌이다.
		}

		// 4개의 경우를 진행했지만 여기서 걸리지 않으면 충돌이 아니다.
	}



	return false;
}

bool CCollisionMgr::CollisionCircleToCircle(CCollider2D * pCollider1, CCollider2D * pCollider2)
{
	// 중심에 대한 World좌표를 구한다.

	const Matrix& matCol1 = pCollider1->GetColliderWorldMat();
	const Matrix& matCol2 = pCollider2->GetColliderWorldMat();

	Vec3 arrCenter[2] = {};
	Vec3 arrRadius[2] = {};

	arrCenter[0] = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol1);
	arrCenter[1] = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol2);

	arrCenter[0].z = 0.f;
	arrCenter[1].z = 0.f;

	// 반지름 옮기기
	arrRadius[0] = XMVector3TransformCoord(Vec3(0.5f, 0.f, 0.f), matCol1);
	arrRadius[1] = XMVector3TransformCoord(Vec3(0.5f, 0.f, 0.f), matCol2);

	arrRadius[0].z = 0.f;
	arrRadius[1].z = 0.f;


	// 두 중심 사이의 거리를 구한다.
	Vec3 vCenter = arrCenter[0] - arrCenter[1];

	float fCenter = sqrt(vCenter.x * vCenter.x +
		vCenter.y * vCenter.y +
		vCenter.z * vCenter.z);

	Vec3 vRadius[2] = {
		arrRadius[0] - arrCenter[0],
		arrRadius[1] - arrCenter[1]
	};

	// 반지름의 길이의 합을 구한다.
	float fRadius = sqrt(vRadius[0].x * vRadius[0].x +
		vRadius[0].y * vRadius[0].y +
		vRadius[0].z * vRadius[0].z) +

		sqrt(vRadius[1].x * vRadius[1].x +
			vRadius[1].y * vRadius[1].y +
			vRadius[1].z * vRadius[1].z);


	// 중심 사이의 거리가 더 크다면 충돌이 아니다.
	if (fRadius < fCenter)
		return false;

	return true;
}
