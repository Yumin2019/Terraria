#pragma once

#include <Engine/Script.h>

class CCraftingScript :
	public CScript
{
public:
	CCraftingScript();
	virtual ~CCraftingScript();
	CLONE(CCraftingScript);

private:
	static Crafting g_CraftInfo[(UINT)ITEM::CRAFTING_END - (UINT)ITEM::WORKBENCH];


private:
	class CInventoryScript*		m_pInven;
	class CPanelScript*			m_pPanel;
	class CMouseScript*			m_pMouse;
	class CUIItemScript*		m_pUIItemArr[50];

	int							m_iCount; // 현재 상황에서 만들 수 있는 것의 개수.
	int							m_iCollIdx;

	CGameObject*				m_pCraftObj;

	// 현재 시점에서의 블럭 인덱스이다. 해당 IDX~IDX +5까지 보여준다. 
	int							m_iShowIdx;

	// 보여주는 아이템에서의 Idx정보로... 0 ~ 4를 사용하여 셋팅한다.
	int							m_iShowCount;

	map<wstring, class CUIItemScript*>	m_mapItem;
	// 생성가능한 아이템에 대한 정보를 알고 있도록 하는 map정보이다.
	// 아이템을 추가하는 경우에 Prototype에서 새로운 정보를 추가하여, 연결해주고 이곳에 Value와 Key를 이용하여 추가한다.
	// 아이템을 삭제하는 경우에 Pair의 정보를 지우고, 실질적인 GameObject까지 날려주는 처리를 한다.

private:
	CUIItemScript* FindPrevItem(const wstring& strItem);

private:
	void SetItemPos(CUIItemScript * pUIScript);
	void CraftingClick();
	CUIItemScript* SetItem(size_t iIdx, CUIItemScript* pUIScript);
	UINT GetIdx(ITEM eType) { return (UINT)eType - (UINT)ITEM::WORKBENCH; }

public:
	void OpenCrafting(); // Crafting 시스템을 열은 경우에 호출할 함수이다.
	void ShowItem(); 
	// Inventory가 Open상태일 경우. 만들 수 있는 아이템을 출력하는 역할을 하는 함수이다.
	// 현재 재료의 개수에서 변동이 일어난 경우를 인식해서, 만들 수 있는 아이템이 추가되거나, 만들 수 없는 아이템을 계산하는 처리도 포함된다.
	
public:
	virtual void Awake();
	virtual void Update();
	virtual void Render();
};

