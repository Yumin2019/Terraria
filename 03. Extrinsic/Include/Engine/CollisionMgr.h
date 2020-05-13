#pragma once


typedef union _tagColID
{
	struct {
		UINT iColID1;
		UINT iColID2;
	};

	DWORD_PTR ID; // 64bit ���� 8byte ������
}ColID, *PColID;

//  8 Byte ������ �̿��Ͽ�, ���� 4byte�� A Collider�� ���� 4Byte������ B Collider�� ��Ƶ� ���̴�.
// ����, �̰��� ���� �̿��� �� �ֵ��� ����ü�� ����Ͽ� �����Ѵ�.


class CGameObject;
class CCollider2D;
class CLayer;

class CTileMap2D;

class CCollisionMgr
{
	SINGLE(CCollisionMgr);

private:
	UINT					m_LayerCheck[MAX_LAYER]; 
	// 32���� Layer�� ������ �����ϰ�, �ű⼭ 32���� bit�� ���� ��Ƽ�
	// �� ����� ���� Layer������ �浹�� �����Ѵ�.
	// �浹�� �ϱ� ���Ѵٸ� ��� ���� ���߾� �ش� �ڸ��� 1�� �����д�.

	map<DWORD_PTR, bool>	m_mapCol;
	// �浹�� ���¸� Ȯ���ϱ� ����  map�̴�.
	// �浹 ���°� ���� ó���� ��쿡�� �ڵ����� �߰��ϰ�, ���¸� �ٲ��ִ� ó�� ���� �Ѵ�.
	// ���⼭ DWORD_PTR�� ��쿡�� ���� 4BYTE�� ���� 4BYTE�� �̿��ؼ�, �� �浹ü�� ǥ���ϰ� �ȴ�.
	// �̰��� Ű���̰�, BOOL�� �浹 ���¿� ���� ���θ� ����Ѵ�.

public:
	bool Init();
	void Update();

	void TileCollision(CGameObject* pTileObj, CGameObject* pObj);
	bool TileCollHeight(CGameObject* pObj, CTileMap2D* pTileMap, float& fResult, Vec4& tTileRC, int iDir);
	bool TileCollWidth(CGameObject* pObj, CTileMap2D* pTileMap, float& fResult, Vec4& tTileRC, int iDir);

	bool OnCollisionLayer(int iLayerIdx);
	// �浹 ���� ���� ������Ʈ�� �ش� ���̾ �����ϴ� ��� true

	void CheckCollisionLayer(const wstring& strLayerName1, const wstring& strLayerName2);
	// Layer�� �̸��� �־��ָ�, �̸��� ���� �ε����� ���ؼ�, �浹�� ���� ó���� �صд�.

	void UnCheckCollisionLayer(const wstring& strLayerName1, const wstring& strLayerName2);
	// Layer�� �̸��� �־��ָ�, �̸��� ���� �ε����� ���ؼ�, �浹�� ���� ó���� �����Ѵ�.

	void CheckCollisionLayer(int iLayerIdx1, int iLayerIdx2); 
	// Layer�� �ε����� �־��ָ� �� �ε����� ���� �浹�� �ϱ� ���� ó���� �صд�.

	void UnCheckCollisionLayer(int iLayerIdx1, int iLayerIdx2);
	// Layer�� �ε����� �־��ָ� �� �ε����� ���� �浹�� �ϱ� ���� ó���� �����Ѵ�.

	UINT* GetLayerCheck() { return m_LayerCheck; }

private:
	void CollisionLayer(const CLayer* pLayer1, const CLayer* pLayer2);

	bool CollisionRectToRect(CCollider2D* pCollider1, CCollider2D* pCollider2);
	bool CollisionRectToCircle(CCollider2D* pCollider1, CCollider2D* pCollider2);
	bool CollisionCircleToCircle(CCollider2D* pCollider1, CCollider2D* pCollider2);

public:
	bool IsCollision(CCollider2D* pCollider1, CCollider2D* pCollider2);

};

