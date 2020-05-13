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

	int							m_iCount; // ���� ��Ȳ���� ���� �� �ִ� ���� ����.
	int							m_iCollIdx;

	CGameObject*				m_pCraftObj;

	// ���� ���������� �� �ε����̴�. �ش� IDX~IDX +5���� �����ش�. 
	int							m_iShowIdx;

	// �����ִ� �����ۿ����� Idx������... 0 ~ 4�� ����Ͽ� �����Ѵ�.
	int							m_iShowCount;

	map<wstring, class CUIItemScript*>	m_mapItem;
	// ���������� �����ۿ� ���� ������ �˰� �ֵ��� �ϴ� map�����̴�.
	// �������� �߰��ϴ� ��쿡 Prototype���� ���ο� ������ �߰��Ͽ�, �������ְ� �̰��� Value�� Key�� �̿��Ͽ� �߰��Ѵ�.
	// �������� �����ϴ� ��쿡 Pair�� ������ �����, �������� GameObject���� �����ִ� ó���� �Ѵ�.

private:
	CUIItemScript* FindPrevItem(const wstring& strItem);

private:
	void SetItemPos(CUIItemScript * pUIScript);
	void CraftingClick();
	CUIItemScript* SetItem(size_t iIdx, CUIItemScript* pUIScript);
	UINT GetIdx(ITEM eType) { return (UINT)eType - (UINT)ITEM::WORKBENCH; }

public:
	void OpenCrafting(); // Crafting �ý����� ���� ��쿡 ȣ���� �Լ��̴�.
	void ShowItem(); 
	// Inventory�� Open������ ���. ���� �� �ִ� �������� ����ϴ� ������ �ϴ� �Լ��̴�.
	// ���� ����� �������� ������ �Ͼ ��츦 �ν��ؼ�, ���� �� �ִ� �������� �߰��ǰų�, ���� �� ���� �������� ����ϴ� ó���� ���Եȴ�.
	
public:
	virtual void Awake();
	virtual void Update();
	virtual void Render();
};

