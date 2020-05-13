#pragma once


typedef union _tagColID
{
	struct {
		UINT iColID1;
		UINT iColID2;
	};

	DWORD_PTR ID; // 64bit 기준 8byte 포인터
}ColID, *PColID;

//  8 Byte 변수를 이용하여, 상위 4byte는 A Collider를 하위 4Byte에서는 B Collider를 담아둘 것이다.
// 따라서, 이것을 쉽게 이용할 수 있도록 공용체를 사용하여 구현한다.


class CGameObject;
class CCollider2D;
class CLayer;

class CTileMap2D;

class CCollisionMgr
{
	SINGLE(CCollisionMgr);

private:
	UINT					m_LayerCheck[MAX_LAYER]; 
	// 32개의 Layer를 행으로 지정하고, 거기서 32개의 bit를 열로 삼아서
	// 이 행렬을 토대로 Layer끼리의 충돌을 구현한다.
	// 충돌을 하길 원한다면 행과 열을 맞추어 해당 자리를 1로 만들어둔다.

	map<DWORD_PTR, bool>	m_mapCol;
	// 충돌의 상태를 확인하기 위한  map이다.
	// 충돌 상태가 가장 처음인 경우에는 자동으로 추가하고, 상태를 바꿔주는 처리 등을 한다.
	// 여기서 DWORD_PTR의 경우에는 상위 4BYTE와 하위 4BYTE를 이용해서, 두 충돌체를 표현하게 된다.
	// 이것이 키값이고, BOOL은 충돌 상태에 대한 여부를 기록한다.

public:
	bool Init();
	void Update();

	void TileCollision(CGameObject* pTileObj, CGameObject* pObj);
	bool TileCollHeight(CGameObject* pObj, CTileMap2D* pTileMap, float& fResult, Vec4& tTileRC, int iDir);
	bool TileCollWidth(CGameObject* pObj, CTileMap2D* pTileMap, float& fResult, Vec4& tTileRC, int iDir);

	bool OnCollisionLayer(int iLayerIdx);
	// 충돌 진행 중인 오브젝트가 해당 레이어에 존재하는 경우 true

	void CheckCollisionLayer(const wstring& strLayerName1, const wstring& strLayerName2);
	// Layer의 이름을 넣어주면, 이름을 토대로 인덱스를 구해서, 충돌을 위한 처리를 해둔다.

	void UnCheckCollisionLayer(const wstring& strLayerName1, const wstring& strLayerName2);
	// Layer의 이름을 넣어주면, 이름을 토대로 인덱스를 구해서, 충돌을 위한 처리를 해제한다.

	void CheckCollisionLayer(int iLayerIdx1, int iLayerIdx2); 
	// Layer의 인덱스를 넣어주면 두 인덱스를 토대로 충돌을 하기 위한 처리를 해둔다.

	void UnCheckCollisionLayer(int iLayerIdx1, int iLayerIdx2);
	// Layer의 인덱스를 넣어주면 두 인덱스를 토대로 충돌을 하기 위한 처리를 해제한다.

	UINT* GetLayerCheck() { return m_LayerCheck; }

private:
	void CollisionLayer(const CLayer* pLayer1, const CLayer* pLayer2);

	bool CollisionRectToRect(CCollider2D* pCollider1, CCollider2D* pCollider2);
	bool CollisionRectToCircle(CCollider2D* pCollider1, CCollider2D* pCollider2);
	bool CollisionCircleToCircle(CCollider2D* pCollider1, CCollider2D* pCollider2);

public:
	bool IsCollision(CCollider2D* pCollider1, CCollider2D* pCollider2);

};

