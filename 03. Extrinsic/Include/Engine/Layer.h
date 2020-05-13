#pragma once
#include "Entity.h"

class CGameObject;

class CLayer :
	public CEntity
{

	friend class CScene;


public:
	CLayer();
	virtual ~CLayer();



private:
	vector<CGameObject*> m_vecParentObj; // ���̾ ���� Parent Obj Vector
	 vector<CGameObject*> m_vecLayerObj; // ���̾ ���� ��� ������Ʈ�� ���� Vector


	int					m_iLayerIdx;

public:
	void AddGameObject(CGameObject* pObject, bool bMoveAll = false); 
	// �־��ִ� ������Ʈ�� �ڽ����� �θ������� �����Ͽ�, Layer�� �ִ��� �������� �����Ͽ� ����ó���� ������ �κ��̴�.

	int GetLayerIdx() const { return m_iLayerIdx; } // Layer ��ȣ�� ������ �Լ�.
	const vector<CGameObject*>& GetParentObjVec() const { return m_vecParentObj; }
	const vector<CGameObject*>& GetLayerObjVec() const { return m_vecLayerObj; }

	void RegisterObj(CGameObject* pObj) { m_vecLayerObj.push_back(pObj); } // ���� �𸣰ڴ�.
	void CheckParentObj(); // Parent Obj Vector���� �θ� nullptr�� �ƴ� ��ü, �� �θ� ���� ��ü�� 
	// ParentObjVec���� ���������� �Լ��̴�. �θ� �ִٴ� ���� �� �̻� �θ�μ� ������ �� ���´ٴ� �ǹ�.

	void RemoveParentObj(CGameObject* pObj); // �־��� �θ� ������Ʈ�� ���ֹ�����.

private:
	void SetLayerIdx(int iLayerIdx); // Layer ��ȣ�� �����ϴ� �Լ�


public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();
	void Render();

};

