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
	// �̸� �������� 32���� Layer�� �̸��� �������ִ� ����̴�. �� ó���� ���ؼ�, �ڽİ� �θ��� ���踦 ���,
	// �������� �������� ����� �� �ִµ�, Layer�� ������鼭 ����� �������� �ذ��� �� �ִ�. (Layer�� �̹� �����Ѵ�)
	// ���� �̸��� ������.
	void AddGameObject(const wstring& strLayerName, CGameObject* pObj, bool bMoveAll); 
	// Layer�� �̸��� ���ؼ� Layer�� ã�ƿ���, ���⿡ Obj�� �־��ش�. �ڽ��� bMoveAll�� flag�� ����, Layer�� ���� ó��
	// �� �߰��ȴ�.
	void AddGameObject(int iLayerIdx, CGameObject* pObj, bool bMoveAll); // Idx����� �߰�.
	CLayer* FindLayer(const wstring& strName) const;
	CGameObject* FindParentObj(const wstring& strName) const;
	CGameObject* FindObject(const wstring& strName) const;

	void SetSceneType(SCENE_TYPE eType) { m_eSceneType = eType; }


};

