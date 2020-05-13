#pragma once
#include "Component.h"
class CTransform :
	public CComponent
{
public:
	CTransform();
	virtual ~CTransform();

	CLONE(CTransform);

private:
	// 현재 위치는 World로. 이전 위치는. PrevPos
	Vec3	m_tPrevPos;

	Vec3	m_tLocalPos;
	Vec3	m_tLocalScale;
	Vec3	m_tLocalRot;

	Matrix	m_matWorld;

	// Local에서 World 로 변환하는 역할을 해야 한다.
	// 따라서, 위의 3가지 정보를 통합할 Matrix를 가지고 있다.

	Vec2	m_tPivot;
	Vec2	m_tPivotTrans;
	bool	m_bRotOffset;

	// TILE Collision
	
	//  충돌 여부 & 방향
	bool	m_bMoveX;
	bool	m_bMoveY;

	float	m_fDirX;
	float	m_fDirY;

	bool	m_bTileColl; // Tile과 충돌.

	// 일단 사용하는 쪽에서 처리하는 걸로 함.

public:
	virtual void Update();
	virtual void FinalUpdate();
	virtual void Awake();
	virtual bool SaveToScene(FILE* pFile);
	virtual bool LoadFromScene(FILE* pFile);

	bool IsMoveX() const { return m_bMoveX; }
	bool IsMoveY() const { return m_bMoveY; }
	float GetDirX() const { return m_fDirX; }
	float GetDirY() const { return m_fDirY; }
	void SetMoveX(bool bMove, float fDirX) { m_bMoveX = bMove; m_fDirX = fDirX; }
	void SetMoveY(bool bMove, float fDirY) { m_bMoveY = bMove; m_fDirY = fDirY; }
	void SetTileColl(bool bCol) { m_bTileColl = bCol; }
	bool IsTileColl() const { return m_bTileColl; }

	void TileCollision();


public:
	const Vec2& GetPivot() const { return m_tPivot; }

	void SetPivot(float x, float y) { m_tPivot = Vec2(x, y); Awake(); } // 데카르트 좌하단 0 0 우상단 1 1
	void EnableLeftTop() { m_tPivot = Vec2(0.f, 1.f); Awake(); }
	void EnableRotOffset() { m_bRotOffset = true; } // 위치 오프셋의 영향을 받는 회전.
public:
	const Vec3& GetLocalPos() const { return m_tLocalPos; }
	const Vec3& GetLocalScale() const { return m_tLocalScale; }
	const Vec3& GetLocalRot() const { return m_tLocalRot; }
	const Matrix& GetWorldMatrix() const { return m_matWorld; }

	const Vec3& GetPrevPos() const { return m_tPrevPos; }
	const Vec3& GetMove() const { return GetLocalPos() - m_tPrevPos; }
	void SetLocalPos(const Vec3& tPos) { if (tPos == m_tLocalPos) return; m_tLocalPos = tPos; Changed(); }
	void SetLocalPos(float x, float y, float z) { if (Vec3(x, y, z) == m_tLocalPos) return; m_tLocalPos = Vec3(x, y, z); Changed(); }
	void SetLocalPos(float x, float y) { if (Vec3(x, y, m_tLocalPos.z) == m_tLocalPos) return;  m_tLocalPos.x = x; m_tLocalPos.y = y; Changed(); }

	void SetLocalScale(const Vec3& tScale) { if (tScale == m_tLocalScale) return;  m_tLocalScale = tScale; Changed(); Awake(); }
	void SetLocalScale(float x, float y, float z) { if (Vec3(x, y, z) == m_tLocalScale) return; m_tLocalScale = Vec3(x, y, z); Changed(); Awake();}
	void SetLocalScale(const Vec2& tScale) { if (Vec3(tScale.x, tScale.y, m_tLocalScale.z) == m_tLocalScale) return; m_tLocalScale = Vec3(tScale.x, tScale.y, 1.f); Changed(); Awake();}
	void SetLocalScale(float x, float y) { if (Vec3(x, y, 1.f) == m_tLocalScale) return;  m_tLocalScale = Vec3(x, y, 1.f); Changed(); Awake();}

	void SetLocalRot(const Vec3& tRot) { if (m_tLocalRot == tRot) return; m_tLocalRot = tRot; Changed(); }
	void SetLocalRot(float x, float y, float z) { if (m_tLocalRot == Vec3(x, y, z)) return; m_tLocalRot = Vec3(x, y, z); Changed(); }

	Vec3 GetWorldPos() const;
	Vec3 GetWorldScale() const;

	void UpdateData();

	bool IsCasting(const Vec3& tPos);

public:
	// 편의성. Trans만 구현.
	void AddLocalPos(const Vec3& vPos) { m_tLocalPos += vPos; }
	void AddLocalPos(float x, float y, float z) { m_tLocalPos += Vec3(x, y, z); }
	void AddLocalPos(float x, float y) { m_tLocalPos += Vec3(x, y, 0.f); }

	

};

