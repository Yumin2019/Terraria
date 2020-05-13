#pragma once
#include <Engine/Script.h>

class CUIItemScript :
	public CScript
{
public:
	CUIItemScript();
	virtual ~CUIItemScript();
	CLONE(CUIItemScript);

private:
	class CItemScript* m_pItem;
	class CScaleScript* m_pScale;
	

	ITEM_TYPE		m_eType; // 아이템의 종류
	ITEM			m_eItem; // 어떤 아이템.

public:
	ITEM_TYPE GetType() const { return m_eType; }
	ITEM	  GetItem() const { return m_eItem; }
	void SetType(ITEM_TYPE eType, ITEM eItem) { m_eItem = eItem; m_eType = eType; }

public:
	CItemScript* GetItemScript() const { return m_pItem; }
	void SetItem(CItemScript* pItem) { m_pItem = pItem; }
public:
	
public:
	virtual void Awake();
	virtual void Start();
	virtual void Update() {}
};

