#pragma once
#include <Engine/Script.h>

/*
ItemObj Script������.... Object�� ������ �������� �������� �Ѱ��Ѵ�.

��ǥ������, �������� ������ ���Ŀ� Jump�� �ϵ��� �����ϰ�, �÷��̾���� �Ÿ��� �Ǵ��ϴٰ� �÷��̾�� �ٰ����� ������(?)
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

	ITEM_TYPE		m_eType; // �������� ����
	ITEM			m_eItem; // � ������.

	int				m_iCount; // ��� �Ǵ� ����

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

