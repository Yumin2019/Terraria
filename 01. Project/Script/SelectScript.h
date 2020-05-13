#pragma once
#include <Engine/Script.h>

enum class SELECT_TYPE
{
	NONE, // 도구
	ONE, // 타일
	TWO_W, // 각종 아이템
	TWO_H,
	FOUR,
	TWO_W_THREE_H
};

class CSelectScript :
	public CScript
{
public:
	CSelectScript();
	virtual ~CSelectScript();
	CLONE(CSelectScript);

private:
	SELECT_TYPE		m_eSelect; // Select Obj의 Type값이다. 이 값에 따라서, 충돌체의 모양이 달라진다.


public:
	SELECT_TYPE GetSelectType() const { return m_eSelect; }
	void ChangeSelectType(SELECT_TYPE eType);
	bool IsIdx(int iIdx) { return (iIdx >= 0 && iIdx < 4000); }

	void OnCollisionTileEnter(CTileMap2D* pTileMap, int iX, int iY);
	void OnCollisionTileExit(CTileMap2D* pTileMap, int iX, int iY);
	// SELECT_TYPE에 따라서, 주어진 타일의 위치를 LT로 생각하고 TILE과 충돌했으면 카운팅을 한다.


public:
	virtual void Update() {}
};

