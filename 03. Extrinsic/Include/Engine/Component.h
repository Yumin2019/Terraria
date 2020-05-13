#pragma once
#include "Entity.h"

#include "GameObject.h"

class CComponent :
	public CEntity
{


public:
	CComponent(COMPONENT_TYPE eType); // 자식의 객체에서는 반드시 생성자에서 이 생성자를 호출하여
	// Type을 입력하도록 한다.
	virtual ~CComponent();

	friend class CGameObject;
	friend class CEventMgr;

private:
	COMPONENT_TYPE	 m_eType;
	CGameObject*	 m_pObj; // 소속 Object
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

bool IsEnable() const { return m_bEnable; } // 활성화여부
	bool IsChanged() const { return m_bChanged; }

public:
	void SetEnable(bool bEnable) { m_bEnable = bEnable; }
	void Changed() { m_bChanged = true; }

private:
	void SetGameObject(CGameObject* pObj) { m_pObj = pObj; }

public:
	virtual CComponent* Clone() = 0; // 순수 가상함수로서 Component 자식들에게 Clone함수를 
	// 만들 것을 강요

public:
	// Awake와 Start함수는 필요한 component에서 재정의하여 사용한다.
	virtual void Awake() {};
	virtual void Start() {};
	virtual void Update() {}
	virtual void LateUpdate() {}
	virtual void FinalUpdate() {}
	virtual void Enable() {} // 활성화 / 비활성화.
	virtual void Disable() {}

	virtual bool SaveToScene(FILE* pFile) = 0;
	virtual bool LoadFromScene(FILE* pFile) = 0;


	friend void ComponentSetChange(CComponent*, bool);

};

