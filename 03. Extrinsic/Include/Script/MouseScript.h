#pragma once

/*
Mouse�� ���� ó���� �ϴ� Script�̴�. �� Script������ UI���� �浹 or Ÿ�Ͽ� ���� �浹�� ����Ѵ�.
*/

#include <Engine/Script.h>


class CMouseScript :
	public CScript
{
public:
	CMouseScript();
	virtual ~CMouseScript();
	CLONE(CMouseScript);

public:
	static CTileMap2D*		g_pTileMap;


private:
	HWND	m_hWnd;
	Resolution m_tRes;
	Vec2	m_tStartPos;
	POINT  m_tPoint;

	class CPlayerScript*		m_pPlayer;
	class CSelectScript*		m_pSelect;
	CGameObject*				m_pCamera;
	CGameObject*				m_pSelectTile;

	bool	m_bShow;

	vector<CGameObject*>	m_vecTileObj;

	CGameObject*			m_pTileObj; // �浹���� Ÿ�� ������Ʈ & idx
	int		m_iColX;
	int		m_iColY;

	float				m_fDigTime;
	float				m_fDigLimitTime;

	// DigTime ����
	float				m_fDigSound;
	float				m_fDigSoundLimitTime;
	int					m_iDig;

	bool				m_bDigFirst;

	// Object �浹�� Font ����.

	wstring				m_strCollision;
	Vec2				m_tUIPos;
	CGameObject*		m_pMouseUIObj;

	// Mouse�� UIObj�� ���� (���콺�� �����ϴ� ����)
	CGameObject*		m_pItemObj;

	// Ÿ���� ��ġ�Ҷ� ���콺�� ��ġ�� �˻��� Layer�� �����Ѵ�.
	UINT				m_iLayerCheck;

	bool				m_bDig; // ĳ�� ������ ����.

private:
	void SetTileIdx(CTileMap2D* pTileMap, int iX, int iY, ITEM eItem); 
	// �ε����� �־��ָ�... �̰��� �����Ѵ�. Ÿ�� ������ �������ְ�... ���� �ε��� ���� + ���� Ÿ�ϱ���.

	void SetTorchIdx(CTileMap2D* pTileMap, int iX, int iY, ITEM eItem);  // (���۴��)
	// �ε����� �־��ָ�, TileInfo + ��ġ������ �����Ѵ�.

	void ChangeTileIdx(CTileMap2D* pTileMap, int iX, int iY);
	bool ChangeTileTorch(CTileMap2D* pTileMap, int iX, int iY, ITEM eItem); 
	// ��ġ ���� �Լ��̴�. �ֺ� Ÿ�Ͽ� ���� �����ε����� �ٲ��ִ� ó���� �Ѵ�.
	// + �ֺ� Ÿ���� ���� ��� Object�� ����������.

	void DigOneBlockItem(CTileMap2D* pTileMap, int iX, int iY, ITEM eItem, int iParticle = 5);
	// OneBlock�������� ĵ ��쿡 ���ܾ��� ��ƼŬ�� ó���ϰ�, �ش� �ڸ��� ����ִ� ó��.
	// ������ ���� + �ֺ� Ÿ�Ͽ� ���ؼ� �Լ� ȣ����� ó���Ѵ�.

public:
	void SetEquipItem(CGameObject* pItemObj) { m_pItemObj = pItemObj; }
	CGameObject*	GetEquipItem() const { return m_pItemObj; }

public:
	static bool			g_bUIColl;

public:
	void SetCollStr(const wstring& strColl) { m_strCollision = strColl; }
	const wstring& GetCollisionStr() const { return m_strCollision; }
	Vec2 GetOutputPos() const { return Vec2(m_tPoint.x + 10.f, m_tPoint.y + 10.f); }

	void SelectItem(ITEM eItem);

	void CreateItem(ITEM eItem, CTileMap2D* pTileMap, int iX, int iY);
	int GetPointX() const { return m_tPoint.x; }
	int GetPointY() const { return m_tPoint.y; }

private:
	void TileCollision();

public:
	virtual void Awake();
	virtual void Update();

	virtual void Render();

	virtual void OnCollision(class CCollider2D* pOther);

	void SetDigTime(float fLimit) {
		m_fDigLimitTime = fLimit; 

		m_iDig = 0;
		m_fDigTime = 0.f;
		m_fDigSound = 0.f;
		m_fDigSoundLimitTime = m_fDigLimitTime / 3.f;
	}
	float GetDigLimitTime() const { return m_fDigLimitTime; }
};

