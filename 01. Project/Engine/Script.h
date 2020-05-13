#pragma once
#include "Component.h"

/*
Script :
��ũ��Ʈ�� �� ���ӿ��� �ʿ��� ����� 
�����ϴ� ������Ʈ�̴�.
��� ���ӿ��� �ʼ������� �ʿ��� ����̶��
������Ʈ�� ������ �ؾ��ϰ�, �� ���Ӹ���
�ʿ��� ����̶�� �̰��� script�� �����̵ȴ�.

script�� GameObject���� �������� ������
���� �� �ִ�.
*/

// Script�� Component�� �̿��Ͽ� ������ ������
// ������ ���̴�.

// ����, Script�� ��ӹ޴� ScriptŬ�����鿡��
// �ַ� ����� ��������� ���⼭ �������ش�.



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
	// �߻� Ŭ���� Clone X

	friend class CCollider2D;
	friend class CCollider3D;

public:
	UINT		m_iScriptType;
	static	int g_iReverse;

public:
	UINT GetScriptType() const { return m_iScriptType; }

public:


	// �����͸� �־ GameObject�� ���� / ���� ���ִ� �Լ��̴�.
	static void CreateObject(CGameObject* pObj, int iLayerIdx);
	static void CreateObject(CGameObject* pObj, const wstring& strLayerName);
	// ��� ������Ʈ�� ��� ��ġ��.

	static void DeleteObject(CGameObject* pObj);
	// ��� ������Ʈ�� ��������. �����ϴ� ������Ʈ�� �θ� ������Ʈ�� ��쿡�� 
	// �ڽı��� ��� ������� ó���� �̷�� ����.
	// EventMgr::Update���� ���� ���迡 �ִ� ������Ʈ�� ���̴� ó���� �ϰ�,
	// ���� �����ӿ��� �̰͵��� ����.

	static void ClearParent(CGameObject* pTarget);
	// Ư�� ������Ʈ�� ���� ���踦 �����ϴ� �Լ�.


	static void TransferLayer(CGameObject* pTarget, const wstring& strLayerName, bool bMoveAll);
	// Ư�� ������Ʈ�� �־��� Layer�� bool Flag�� �����Ͽ�, �Ű��ִ� �Լ��̴�.

	static void TransferLayer(CGameObject* pTarget, int iLayerIdx, bool bMoveAll);
	// Ư�� ������Ʈ�� �־��� LayerIdx�� �̿��Ͽ� ������ �Լ��̴�.

	static void EnableObj(CGameObject* pObj);
	static void DisableObj(CGameObject* pObj);

	static void EnableComponent(CComponent* pCom);
	static void DisableComponent(CComponent* pCom);

	static void AddChild(CGameObject* pParent, CGameObject* pChild);
	// �θ�� �ڽ��� �־��ָ� �̰��� �ڽ��� �ִ� �Լ�.


protected:

	void AddChild(CGameObject* pChild);
	// Script�� �Ҽ� Object�� �θ�� �Ͽ�, �ڽ��� �־��ִ� �Լ�.
	
	void ClearParent();
	// Script�� �Ҽ� Object�� ���� ���踦 �����ϴ� �Լ�.
	
	void TransferLayer(const wstring& strLayerName, bool bMoveAll);
	// Layer�� �̸��� �־��ָ� �̰��� ���� Idx�� ���ϰ�(�̰����� ��), MoveAll�� ���� �־
	// Script�� �Ҽ� Object�� �ش� Layer�� �̵��ϵ��� �ϴ� �Լ�.

	void TransferLayer(int iLayerIdx, bool bMoveAll);
	// Idx , bool

	
protected:

	// �浹�ÿ� ó���� ������ ���� �Լ��̴�. ���ϴ� ��쿡 �����Լ��� �������ش�.
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

