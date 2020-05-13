#pragma once
#include "Entity.h"

class CComponent; // Component �迭�� ����� ���� ���� ����

// �� Component�� ���� �������� ���� �Լ��� �����ϴ� �뵵�� ���漱��
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
	vector<CGameObject*>	m_vecChild; // �ڽ��� �ڽĿ� ���� ó���� �ϱ����� vector
	CGameObject*			m_pParent;
	int						m_iLayerIdx;

	bool					m_bEnable;
	bool					m_bActive;
	
	vector<CScript*>		m_vecScript;

	bool					m_bChanged; 

public:
	bool IsChanged() const { return m_bChanged; }
	void Changed() { m_bChanged = true; } // �ϴ��� Layer �ٲ�� Changed ������.

	size_t GetScriptSize() const { return m_vecScript.size(); }

public:
	void SetEnable(bool bEnable) { m_bEnable = bEnable; } // Enable�� Ȱ��/ ��Ȱ���� �����ϰ� ó���Ѵ�.
	void ActiveDisable();
// �̰� ���θ�, ��� ó������ ���ܽ�Ű��, vetor������ ����.

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
	// ��ũ��Ʈ�� �̸��� ���� ������Ʈ�� ����
	// ��ũ��Ʈ�� ã���ִ� �Լ��̴�.
	// ��ũ��Ʈ�� �̸��� ��� ��쿡�� ã�ƿ� �� �ִ�.
	CScript* FindScript(UINT iScriptType);

	void SetDisable(bool bFlag);



	const vector<CScript*> GetScriptVec() const { return m_vecScript; }
	bool HasScript() const { return !m_vecScript.empty(); }

	void AddChild(CGameObject* pChild); // �־��� GameObject�� �� G.O�� �ڽ����� �����Ѵ�. 
	// ���� ���߿� �� �Լ��� ȣ���ϸ� ERROR ��Ȳ�̴�.
	CGameObject* GetParent() const { return m_pParent; }
	int GetLayerIdx() const { return m_iLayerIdx; }
	bool IsAncestor(CGameObject* pObj) const;

	void ClearParent(CGameObject* pNextParent = nullptr);
	const vector<CGameObject*>& GetChildVec() const { return m_vecChild; }

	void SetLayerIdx(int iLayerIdx) { m_iLayerIdx = iLayerIdx; Changed(); } // �ε����� �������ִ� �Լ�����, ���� ���̾��� ������ �Ű澲�� �ʴ´�.
public:
	// Scene -> Layer -> Obj ����� �������� ȣ���� �Լ��̴�.
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
	// GameObject�� ��쿡�� Component �� ���� ������ �־�� �Ѵ�.
	// ����, �ܼ��� Shallow Copy�� �ƴ� Deep Copy�� ����� �Ѵ�.
	// ���� ���� ���� �����ڸ� �����Ѵ�.

	// ���� �����ڰ� ȣ���� �Ǵ� ���
	/*
	class A = b;
	class A(b);

	���ǽ����� ���縦 ���� ����  private�� ��������ڸ� ȣ���Ѵ�.
	���縦 ���ϴ� ��쿡�� public���� �Ǿ��ִ� Clone�Լ��� ȣ���ϸ� �ȴ�.
	*/
};

