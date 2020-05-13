#pragma once
#include "Entity.h"

class CLayer;
class CGameObject;
class CCamera;


class CScene :
	public CEntity
{
public:
	CScene();
	virtual ~CScene();

private:
	CLayer*				m_pLayerArray[MAX_LAYER];
	vector<CCamera*>	m_vecCamera;
	SCENE_TYPE			m_eSceneType;

public:
	virtual bool Init();
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
	virtual void FinalUpdate();
	virtual void Render();

	CLayer** GetLayerArr() { return m_pLayerArray; }


public:
	CLayer* GetLayer(int iIdx) { return m_pLayerArray[iIdx]; }
	SCENE_TYPE GetSceneType() const { return m_eSceneType; }
	void ChangeLayer(int iLayerIdx1, int iLayerIdx2);

public:
	CLayer* SetLayerName(const wstring& strName, int iIdx); 
	// 미리 생성해준 32개의 Layer의 이름을 지정해주는 방식이다. 이 처리를 통해서, 자식과 부모의 관계를 묶어서,
	// 프리팹의 개념으로 사용할 수 있는데, Layer가 사라지면서 생기는 문제점을 해결할 수 있다. (Layer는 이미 존재한다)
	// 단지 이름이 없을뿐.
	void AddGameObject(const wstring& strLayerName, CGameObject* pObj, bool bMoveAll); 
	// Layer의 이름을 통해서 Layer를 찾아오고, 여기에 Obj를 넣어준다. 자식은 bMoveAll의 flag에 따라서, Layer에 대한 처리
	// 가 추가된다.
	void AddGameObject(int iLayerIdx, CGameObject* pObj, bool bMoveAll); // Idx방식의 추가.
	CLayer* FindLayer(const wstring& strName) const;
	CGameObject* FindParentObj(const wstring& strName) const;
	CGameObject* FindObject(const wstring& strName) const;

	void SetSceneType(SCENE_TYPE eType) { m_eSceneType = eType; }


};

