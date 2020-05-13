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
	int GetCollIdx() const { return m_iCollIdx; } // Mouse와 충돌한 인덱스를 반환한다.

public:
	bool empty(size_t idx) { return !m_pItemArr[idx]; } // 해당 인덱스가 비어있는가.
	CItemScript* SetItem(size_t idx, CItemScript* pItem);
	CItemScript* GetItem(int idx) { return m_pItemArr[idx]; }

	bool AddItem(CItemScript* pItem, bool bReverse = false);  // Script를 통한 아이템 추가.
	bool AddItem(ITEM eItem, int iCount, bool bReverse = false); 
	// item tag를 통해서 아이템을 추가하는 함수이다. 내부적으로 필요한 경우에 할당이 일어난다.

	void ClearItem(CItemScript* pItem); 
	void ClearItem(int iIdx);
	// Inventory에 존재하는 아이템의 포인터를 넣어주면, 해당 아이템과 연결된 오브젝트를 정리해준다. ( Player의 handle까지고 고려하여)

	bool IsOpen() const { return g_bOpen; }

	int GetEmptyIdx(bool bReverse = false) const;
	int AddedItem(ITEM eItem) const; // 해당 아이템이 인벤토리에 있는지 확인하는 함수이다.
	// 있는 경우에는 idx를 반환하고, 없는 경우에는 -1을 반환.

	void SetOpen(bool bOpen); 
	// Panel의 색상 처리 & Open, Close에 대한 위치 설정.

	void SetHandleItem(int idx, unsigned char r, unsigned char g, unsigned char b); 
	// Panel의 색상 처리 & Scale 변환 선택에 대한 처리를 한다.


	// 재료로 사용한 아이템의 개수를 줄이는 처리를 하기 위한 함수이다.
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

