#pragma once
#include <Engine/Script.h>

/*
ItemObj Script에서는.... Object로 생성된 아이템의 움직임을 총괄한다.

대표적으로, 아이템이 생성된 이후에 Jump를 하도록 관리하고, 플레이어와의 거리를 판단하다가 플레이어에게 다가가서 먹힌다(?)
*/


class CItemObjScript :
	public CScript
{
public:
	CItemObjScript();
	virtual ~CItemObjScript();
	CLONE(CItemObjScript);

private:

	class CFlyawayScript*	m_pFly;
	class CGravityScript*	m_pGravity;
	class CGameObject*		m_pPlayer;

	bool	m_bJump;
	bool	m_bFly;

	ITEM_TYPE		m_eType; // 아이템의 종류
	ITEM			m_eItem; // 어떤 아이템.

	int				m_iCount; // 얻게 되는 개수

public:
	ITEM_TYPE GetType() const { return m_eType; }
	ITEM	  GetItem() const { return m_eItem; }
	void SetType(ITEM_TYPE eType, ITEM eItem) { m_eItem = eItem; m_eType = eType; }
	int GetCount() const { return m_iCount; }
	void SetCount(int iCount) { m_iCount = iCount; }

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	
};

