#pragma once
#include <Engine/Script.h>

#define HELMET_IDX 10
#define ARMOR_IDX 20
#define LEGGINGS_IDX 30

class CEquipmentScript :
	public CScript
{
public:
	CEquipmentScript();
	virtual ~CEquipmentScript();
	CLONE(CEquipmentScript);

private:
	class CPanelScript*		m_pPanel;
	class CMouseScript*		m_pMouse;
	class CPlayerScript*	m_pPlayer;
	class CItemScript*	m_pItemArr[50];

	int						m_iCollIdx;

private:
	void EquipmentClick();

public:
	void OpenEquipment();

	CItemScript* SetItem(size_t idx, CItemScript* pItem);
	CItemScript* GetItem(int idx) { return m_pItemArr[idx]; }

	CItemScript* GetHelmet() const { return m_pItemArr[HELMET_IDX]; }
	CItemScript* GetArmor() const { return m_pItemArr[ARMOR_IDX]; }
	CItemScript* GetLeggings() const { return m_pItemArr[LEGGINGS_IDX]; }

public:
	virtual void Awake();
	virtual void Update();
	virtual void Render();
};

