#pragma once
#include "Component.h"
#include "Ptr.h"
#include "Material.h"
#include "Mesh.h"


class CCollider2D :
	public CComponent
{

private:
	static UINT				g_iColID; 
	// 42억의 수를 토대로 충돌체에게 정수값을 전달해준다.


public:
	CCollider2D();
	CCollider2D(const CCollider2D& col); // ID값에 대한 처리를 하기 위해 복사 생성자를 구현
	virtual ~CCollider2D();

	CLONE(CCollider2D);

public:
	void operator = (const CCollider2D& col);

private:
	Vec3						m_tOffset;			// 추측하건데, 원본 정점의 Local좌표를 기준으로
	// 각 정점 마다 옮길 Offset 정보라고 추측한다.

	Vec3						m_tScale;
	// 충돌체를 몇 배수의 크기로 키울 것인지 

	COLLIDER2D_TYPE				m_eType;
	unsigned int				m_iCollisionCount;

	// MeshRender의 경우에는 GameObject를 출력하기 위한 Component이다. 
	// 따라서, Collider를 따로 출력하기 위한 (DEBUG) Mesh와 Render를 가지고 있는 것이다.
	Ptr<CMesh>						m_pColMesh;
	Ptr<CMaterial>					m_pColMtrl;

	// Offset 정보와 Scale 값을 적용한 Collider World Matrix이다.
	Matrix						m_matColWorld;

	UINT						m_iColID; // 충돌체 고유 ID값

	bool						m_bRender;

public:
	virtual void Awake();
	virtual void FinalUpdate();
	virtual bool SaveToScene(FILE* pFile);
	virtual bool LoadFromScene(FILE* pFile);
	void Render();


	void OnCollisionEnter(CCollider2D* pOther);
	void OnCollision(CCollider2D* pOther);
	void OnCollisionExit(CCollider2D* pOther);
	unsigned int GetCollisionCount() const { return m_iCollisionCount; }


public:
	void SetOffset(const Vec3& tOffset) { m_tOffset = tOffset; Changed(); }
	void SetOffset(float x, float y, float z) { m_tOffset = Vec3(x, y, z); Changed(); }

	void SetScale(const Vec3& tScale) { m_tScale = tScale; Changed(); }
	void SetScale(float x, float y, float z) { m_tScale = Vec3(x, y, z); Changed(); }

	void SetCollider2DType(COLLIDER2D_TYPE eType);
	void SetRender(bool bRender) { m_bRender = bRender; }


public:
	const Vec3& GetOffset() const { return m_tOffset; }
	const Vec3& GetScale() const { return m_tScale; }

	UINT GetColID() const { return m_iColID; }
	COLLIDER2D_TYPE GetColliderType() const { return m_eType; }
	const Matrix& GetColliderWorldMat() const { return m_matColWorld; }
	




	
};

