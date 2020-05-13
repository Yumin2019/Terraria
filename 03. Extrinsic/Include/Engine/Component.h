#pragma once
#include "Entity.h"

#include "GameObject.h"

class CComponent :
	public CEntity
{


public:
	CComponent(COMPONENT_TYPE eType); // �ڽ��� ��ü������ �ݵ�� �����ڿ��� �� �����ڸ� ȣ���Ͽ�
	// Type�� �Է��ϵ��� �Ѵ�.
	virtual ~CComponent();

	friend class CGameObject;
	friend class CEventMgr;

private:
	COMPONENT_TYPE	 m_eType;
	CGameObject*	 m_pObj; // �Ҽ� Object
	bool			 m_bChanged;
	bool			 m_bEnable;
	

public:
	COMPONENT_TYPE GetComponentType() const { return m_eType; }
	CGameObject* GetObj() const { return m_pObj; }

	CCollider3D* GetCollider3D() const { return m_pObj->GetCollider3D(); }
	CCollider2D* GetCollider2D() const { return m_pObj->GetCollider2D(); }
	CTransform* GetTransform() const { return m_pObj->GetTransform(); }
	CMeshRender* GetMeshRender() const { return m_pObj->GetMeshRender(); }
	CAnimator2D* GetAnimator2D() const { return m_pObj->GetAnimator2D(); }
	CLight2D* GetLight2D() const { return m_pObj->GetLight2D(); }
	CCamera* GetCamera() const { return m_pObj->GetCamera(); }
	CTileMap2D* GetTileMap2D() const { return m_pObj->GetTileMap2D(); }

bool IsEnable() const { return m_bEnable; } // Ȱ��ȭ����
	bool IsChanged() const { return m_bChanged; }

public:
	void SetEnable(bool bEnable) { m_bEnable = bEnable; }
	void Changed() { m_bChanged = true; }

private:
	void SetGameObject(CGameObject* pObj) { m_pObj = pObj; }

public:
	virtual CComponent* Clone() = 0; // ���� �����Լ��μ� Component �ڽĵ鿡�� Clone�Լ��� 
	// ���� ���� ����

public:
	// Awake�� Start�Լ��� �ʿ��� component���� �������Ͽ� ����Ѵ�.
	virtual void Awake() {};
	virtual void Start() {};
	virtual void Update() {}
	virtual void LateUpdate() {}
	virtual void FinalUpdate() {}
	virtual void Enable() {} // Ȱ��ȭ / ��Ȱ��ȭ.
	virtual void Disable() {}

	virtual bool SaveToScene(FILE* pFile) = 0;
	virtual bool LoadFromScene(FILE* pFile) = 0;


	friend void ComponentSetChange(CComponent*, bool);

};

