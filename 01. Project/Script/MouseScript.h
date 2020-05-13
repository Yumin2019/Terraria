#pragma once

/*
Mouse에 대한 처리를 하는 Script이다. 이 Script에서는 UI와의 충돌 or 타일에 대한 충돌을 담당한다.
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

	CGameObject*			m_pTileObj; // 충돌중인 타일 오브젝트 & idx
	int		m_iColX;
	int		m_iColY;

	float				m_fDigTime;
	float				m_fDigLimitTime;

	// DigTime 분할
	float				m_fDigSound;
	float				m_fDigSoundLimitTime;
	int					m_iDig;

	bool				m_bDigFirst;

	// Object 충돌과 Font 연결.

	wstring				m_strCollision;
	Vec2				m_tUIPos;
	CGameObject*		m_pMouseUIObj;

	// Mouse와 UIObj를 연결 (마우스에 장착하는 느낌)
	CGameObject*		m_pItemObj;

	// 타일을 설치할때 마우스의 위치와 검사할 Layer를 지정한다.
	UINT				m_iLayerCheck;

	bool				m_bDig; // 캐는 중인지 여부.

private:
	void SetTileIdx(CTileMap2D* pTileMap, int iX, int iY, ITEM eItem); 
	// 인덱스를 넣어주면... 이곳을 셋팅한다. 타일 정보를 셋팅해주고... 참조 인덱스 설정 + 주위 타일까지.

	void SetTorchIdx(CTileMap2D* pTileMap, int iX, int iY, ITEM eItem);  // (원작대로)
	// 인덱스를 넣어주면, TileInfo + 설치방향을 지정한다.

	void ChangeTileIdx(CTileMap2D* pTileMap, int iX, int iY);
	bool ChangeTileTorch(CTileMap2D* pTileMap, int iX, int iY, ITEM eItem); 
	// 토치 전용 함수이다. 주변 타일에 따라서 참조인덱스를 바꿔주는 처리를 한다.
	// + 주변 타일이 없는 경우 Object를 생성해주자.

	void DigOneBlockItem(CTileMap2D* pTileMap, int iX, int iY, ITEM eItem, int iParticle = 5);
	// OneBlock아이템을 캔 경우에 생겨야할 파티클을 처리하고, 해당 자리를 비워주는 처리.
	// 아이템 생성 + 주변 타일에 대해서 함수 호출까지 처리한다.

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

