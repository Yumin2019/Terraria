#pragma once
#include "Entity.h"

class CComponent; // Component 배열의 사용을 위한 전방 선언

// 각 Component를 쉽게 가져오기 위한 함수를 정의하는 용도의 전방선언
class CTransform;
class CMeshRender;
class CCamera;
class CCollider2D;
class CCollider3D;
class CAnimator2D;
class CAnimator3D;
class CTileMap2D;
class CScript;
class CLight2D;
class CItemScript;
class CItem3D;


class CGameObject :
	public CEntity
{
public:
	CGameObject();
	virtual ~CGameObject();
	CLONE(CGameObject);

	friend class CEventMgr;

private:
	CComponent*				m_pComArray[(UINT)COMPONENT_TYPE::END];
	vector<CGameObject*>	m_vecChild; // 자신의 자식에 대한 처리를 하기위한 vector
	CGameObject*			m_pParent;
	int						m_iLayerIdx;

	bool					m_bEnable;
	bool					m_bActive;
	
	vector<CScript*>		m_vecScript;

	bool					m_bChanged; 

public:
	bool IsChanged() const { return m_bChanged; }
	void Changed() { m_bChanged = true; } // 일단은 Layer 바뀌면 Changed 해주자.

	size_t GetScriptSize() const { return m_vecScript.size(); }

public:
	void SetEnable(bool bEnable) { m_bEnable = bEnable; } // Enable은 활성/ 비활성이 가능하게 처리한다.
	void ActiveDisable();
// 이걸 꺼두면, 모든 처리에서 제외시키고, vetor에서도 뺀다.

public:
	bool IsEnable() const { return m_bEnable; }
	bool IsActive() const { return m_bActive; }
	

public:
	void AddComponent(CComponent* pComponent);
	void DeleteComponent(COMPONENT_TYPE eType);
	CComponent* GetComponent(COMPONENT_TYPE eType) const { return m_pComArray[(UINT)eType]; }
	CTransform* GetTransform() const { return (CTransform*)m_pComArray[(UINT)COMPONENT_TYPE::TRANSFORM]; }
	CMeshRender* GetMeshRender() const { return (CMeshRender*)m_pComArray[(UINT)COMPONENT_TYPE::MESHRENDER]; }
	CCamera* GetCamera() const { return (CCamera*)m_pComArray[(UINT)COMPONENT_TYPE::CAMERA]; }
	CCollider2D* GetCollider2D() const { return (CCollider2D*)m_pComArray[(UINT)COMPONENT_TYPE::COLLIDER2D]; }
	CCollider3D* GetCollider3D() const { return (CCollider3D*)m_pComArray[(UINT)COMPONENT_TYPE::COLLIDER3D]; }
	CAnimator2D* GetAnimator2D() const { return (CAnimator2D*)m_pComArray[(UINT)COMPONENT_TYPE::ANIMATOR2D]; }
	CLight2D* GetLight2D() const { return (CLight2D*)m_pComArray[(UINT)COMPONENT_TYPE::LIGHT2D]; }
	CTileMap2D* GetTileMap2D() const { return (CTileMap2D*)m_pComArray[(UINT)COMPONENT_TYPE::TILEMAP2D]; }
	//CTileMap3D* GetTileMap3D() const { return (CTileMap3D*)m_pComArray[(UINT)COMPONENT_TYPE::TILEMAP3D]; }

	void DeleteScript(UINT iScriptType);
public:
	CScript* FindScript(const wstring& strScriptName);
	// 스크립트의 이름을 토대로 오브젝트가 가진
	// 스크립트를 찾아주는 함수이다.
	// 스크립트에 이름을 써둔 경우에만 찾아올 수 있다.
	CScript* FindScript(UINT iScriptType);

	void SetDisable(bool bFlag);



	const vector<CScript*> GetScriptVec() const { return m_vecScript; }
	bool HasScript() const { return !m_vecScript.empty(); }

	void AddChild(CGameObject* pChild); // 넣어준 GameObject를 이 G.O의 자식으로 설정한다. 
	// 실행 도중에 이 함수를 호출하면 ERROR 상황이다.
	CGameObject* GetParent() const { return m_pParent; }
	int GetLayerIdx() const { return m_iLayerIdx; }
	bool IsAncestor(CGameObject* pObj) const;

	void ClearParent(CGameObject* pNextParent = nullptr);
	const vector<CGameObject*>& GetChildVec() const { return m_vecChild; }

	void SetLayerIdx(int iLayerIdx) { m_iLayerIdx = iLayerIdx; Changed(); } // 인덱스를 지정해주는 함수지만, 이전 레이어의 정보는 신경쓰지 않는다.
public:
	// Scene -> Layer -> Obj 하향식 구조에서 호출할 함수이다.
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();
	void Render();
	void Enable();
	void Disable();

	void RegisterToLayer();

private:
	CGameObject(const CGameObject& game);
	// GameObject의 경우에는 Component 를 각각 가지고 있어야 한다.
	// 따라서, 단순히 Shallow Copy가 아닌 Deep Copy를 해줘야 한다.
	// 따라서 따로 복사 생성자를 구현한다.

	// 복사 생성자가 호출이 되는 경우
	/*
	class A = b;
	class A(b);

	무의식적인 복사를 막기 위해  private로 복사생성자를 호출한다.
	복사를 원하는 경우에는 public으로 되어있는 Clone함수를 호출하면 된다.
	*/
};

