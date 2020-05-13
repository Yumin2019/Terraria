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
	vector<CGameObject*> m_vecParentObj; // 레이어에 속한 Parent Obj Vector
	 vector<CGameObject*> m_vecLayerObj; // 레이어에 속한 모든 오브젝트를 담을 Vector


	int					m_iLayerIdx;

public:
	void AddGameObject(CGameObject* pObject, bool bMoveAll = false); 
	// 넣어주는 오브젝트가 자식인지 부모인지를 구분하여, Layer가 있는지 없는지를 구분하여 예외처리가 가득한 부분이다.

	int GetLayerIdx() const { return m_iLayerIdx; } // Layer 번호를 얻어오는 함수.
	const vector<CGameObject*>& GetParentObjVec() const { return m_vecParentObj; }
	const vector<CGameObject*>& GetLayerObjVec() const { return m_vecLayerObj; }

	void RegisterObj(CGameObject* pObj) { m_vecLayerObj.push_back(pObj); } // 아직 모르겠다.
	void CheckParentObj(); // Parent Obj Vector에서 부모가 nullptr가 아닌 물체, 즉 부모를 가진 물체를 
	// ParentObjVec에서 지워버리는 함수이다. 부모가 있다는 것은 더 이상 부모로서 존재할 수 없는다는 의미.

	void RemoveParentObj(CGameObject* pObj); // 넣어준 부모 오브젝트를 없애버린다.

private:
	void SetLayerIdx(int iLayerIdx); // Layer 번호를 셋팅하는 함수


public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();
	void Render();

};

