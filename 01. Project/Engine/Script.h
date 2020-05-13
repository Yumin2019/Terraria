#pragma once
#include "Component.h"

/*
Script :
스크립트는 각 게임에서 필요한 기능을 
구현하는 컴포넌트이다.
모든 게임에서 필수적으로 필요한 기능이라면
컴포넌트로 지원을 해야하고, 각 게임마다
필요한 기능이라면 이것을 script로 구현이된다.

script는 GameObject에서 여러개를 가지고
있을 수 있다.
*/

// Script는 Component를 이용하여 게임의 로직을
// 구현할 것이다.

// 따라서, Script를 상속받는 Script클래스들에서
// 주로 사용할 헤더파일을 여기서 연결해준다.



#include "Transform.h"
#include "MeshRender.h"
#include "Collider2D.h"
#include "Animator2D.h"
#include "Light2D.h"



#include "TimeMgr.h"
#include "KeyMgr.h"
#include "ResMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Layer.h"
#include "EventMgr.h"
#include "FontMgr.h"


class CScript :
	public CComponent
{
public:
	CScript(UINT iScriptType);
	virtual ~CScript();
	// 추상 클래스 Clone X

	friend class CCollider2D;
	friend class CCollider3D;

public:
	UINT		m_iScriptType;
	static	int g_iReverse;

public:
	UINT GetScriptType() const { return m_iScriptType; }

public:


	// 데이터만 넣어서 GameObject를 생성 / 제거 해주는 함수이다.
	static void CreateObject(CGameObject* pObj, int iLayerIdx);
	static void CreateObject(CGameObject* pObj, const wstring& strLayerName);
	// 어느 오브젝트를 어느 위치에.

	static void DeleteObject(CGameObject* pObj);
	// 어느 오브젝트를 삭제할지. 삭제하는 오브젝트가 부모 오브젝트인 경우에는 
	// 자식까지 모두 사라지는 처리가 이루어 진다.
	// EventMgr::Update에서 계층 관계에 있는 오브젝트를 죽이는 처리를 하고,
	// 다음 프레임에서 이것들을 삭제.

	static void ClearParent(CGameObject* pTarget);
	// 특정 오브젝트의 계층 관계를 정리하는 함수.


	static void TransferLayer(CGameObject* pTarget, const wstring& strLayerName, bool bMoveAll);
	// 특정 오브젝트를 넣어준 Layer로 bool Flag를 참고하여, 옮겨주는 함수이다.

	static void TransferLayer(CGameObject* pTarget, int iLayerIdx, bool bMoveAll);
	// 특정 오브젝트를 넣어준 LayerIdx를 이용하여 보내는 함수이다.

	static void EnableObj(CGameObject* pObj);
	static void DisableObj(CGameObject* pObj);

	static void EnableComponent(CComponent* pCom);
	static void DisableComponent(CComponent* pCom);

	static void AddChild(CGameObject* pParent, CGameObject* pChild);
	// 부모와 자식을 넣어주면 이것의 자식을 주는 함수.


protected:

	void AddChild(CGameObject* pChild);
	// Script의 소속 Object를 부모로 하여, 자식을 넣어주는 함수.
	
	void ClearParent();
	// Script의 소속 Object의 계층 관계를 정리하는 함수.
	
	void TransferLayer(const wstring& strLayerName, bool bMoveAll);
	// Layer의 이름을 넣어주면 이것을 토대로 Idx를 구하고(이곳으로 갈), MoveAll의 값도 넣어서
	// Script의 소속 Object가 해당 Layer로 이동하도록 하는 함수.

	void TransferLayer(int iLayerIdx, bool bMoveAll);
	// Idx , bool

	
protected:

	// 충돌시에 처리할 로직에 대한 함수이다. 원하는 경우에 가상함수를 생성해준다.
	virtual void OnCollisionEnter(CCollider2D* pOther) {}
	virtual void OnCollision(CCollider2D* pOther) {}
	virtual void OnCollisionExit(CCollider2D* pOther) {}

public:
	virtual void UpdateData() {}

public:
	virtual void Update() = 0;
	virtual void TileCollision() {}
	virtual void Render() {}

	virtual bool SaveToScene(FILE* pFile) { return true; }
	virtual bool LoadFromScene(FILE* pFile) { return true; }

	virtual CScript* Clone() = 0;
};

