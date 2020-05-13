#pragma once
#include <Engine/Script.h>


class CItemScript;
class CPanelScript;
class CMouseScript;
class CPlayerScript;
class CCraftingScript;
class CEquipmentScript;

class CInventoryScript :
	public CScript
{
public:
	CInventoryScript();
	virtual ~CInventoryScript();
	CLONE(CInventoryScript);


public:
	static bool		 g_bOpen;
	static bool		 g_bSelect;
	static int		 g_arrMtrlCount[(UINT)ITEM::NUM_END];
	static CCraftingScript*	g_pCraft;

private:
	float	m_fSecond;

public:

	static int GetMtrlCount(ITEM eItem);
	static void SetMtrlCount(ITEM eItem, int iCount);
	static void AddMtrlCount(ITEM eItem, int iCount = 1);
	static void SubMtrlCount(ITEM eItem, int iCount = 1); 

private:
	CPanelScript*		m_pPanel;
	CMouseScript*		m_pMouse;
	CPlayerScript*		m_pPlayer;
	CEquipmentScript*	m_pEquipment;
	CGameObject*		m_pArmorPanel;

	CItemScript* m_pItemArr[50];

	int			 m_iSelectItem;

	Vec2		 m_tPanelSize;

	int			m_iCollIdx;

	


public:
	int GetCollIdx() const { return m_iCollIdx; } // Mouse�� �浹�� �ε����� ��ȯ�Ѵ�.

public:
	bool empty(size_t idx) { return !m_pItemArr[idx]; } // �ش� �ε����� ����ִ°�.
	CItemScript* SetItem(size_t idx, CItemScript* pItem);
	CItemScript* GetItem(int idx) { return m_pItemArr[idx]; }

	bool AddItem(CItemScript* pItem, bool bReverse = false);  // Script�� ���� ������ �߰�.
	bool AddItem(ITEM eItem, int iCount, bool bReverse = false); 
	// item tag�� ���ؼ� �������� �߰��ϴ� �Լ��̴�. ���������� �ʿ��� ��쿡 �Ҵ��� �Ͼ��.

	void ClearItem(CItemScript* pItem); 
	void ClearItem(int iIdx);
	// Inventory�� �����ϴ� �������� �����͸� �־��ָ�, �ش� �����۰� ����� ������Ʈ�� �������ش�. ( Player�� handle������ ����Ͽ�)

	bool IsOpen() const { return g_bOpen; }

	int GetEmptyIdx(bool bReverse = false) const;
	int AddedItem(ITEM eItem) const; // �ش� �������� �κ��丮�� �ִ��� Ȯ���ϴ� �Լ��̴�.
	// �ִ� ��쿡�� idx�� ��ȯ�ϰ�, ���� ��쿡�� -1�� ��ȯ.

	void SetOpen(bool bOpen); 
	// Panel�� ���� ó�� & Open, Close�� ���� ��ġ ����.

	void SetHandleItem(int idx, unsigned char r, unsigned char g, unsigned char b); 
	// Panel�� ���� ó�� & Scale ��ȯ ���ÿ� ���� ó���� �Ѵ�.


	// ���� ����� �������� ������ ���̴� ó���� �ϱ� ���� �Լ��̴�.
	void UsedMtrl(ITEM eMtrl, int iUseCount);

	CMouseScript* GetMouse() const { return m_pMouse; }


private:
	void InventoryClick();

public:
	virtual void Update();
	virtual void Awake();
	virtual void Start();
	virtual void Render();


};

