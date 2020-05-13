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
	// 42���� ���� ���� �浹ü���� �������� �������ش�.


public:
	CCollider2D();
	CCollider2D(const CCollider2D& col); // ID���� ���� ó���� �ϱ� ���� ���� �����ڸ� ����
	virtual ~CCollider2D();

	CLONE(CCollider2D);

public:
	void operator = (const CCollider2D& col);

private:
	Vec3						m_tOffset;			// �����ϰǵ�, ���� ������ Local��ǥ�� ��������
	// �� ���� ���� �ű� Offset ������� �����Ѵ�.

	Vec3						m_tScale;
	// �浹ü�� �� ����� ũ��� Ű�� ������ 

	COLLIDER2D_TYPE				m_eType;
	unsigned int				m_iCollisionCount;

	// MeshRender�� ��쿡�� GameObject�� ����ϱ� ���� Component�̴�. 
	// ����, Collider�� ���� ����ϱ� ���� (DEBUG) Mesh�� Render�� ������ �ִ� ���̴�.
	Ptr<CMesh>						m_pColMesh;
	Ptr<CMaterial>					m_pColMtrl;

	// Offset ������ Scale ���� ������ Collider World Matrix�̴�.
	Matrix						m_matColWorld;

	UINT						m_iColID; // �浹ü ���� ID��

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

