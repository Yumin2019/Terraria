#pragma once
#include "DirectionScript.h"

class CPlayerScript;
class CItemScript;

class CArmorScript :
	public CDirectionScript
{
public:
	CArmorScript();
	virtual ~CArmorScript();
	CLONE(CArmorScript);

private:
	CPlayerScript*		m_pPlayer;
	CItemScript*		m_pItem;

	Vec2				m_tOffset;
	ITEM				m_eItem;
	ITEM_TYPE			m_eType;
	int					m_iArmor;

public:
	virtual void Awake();
	virtual void Start();

	virtual void Update() {}
	virtual void LateUpdate();
	virtual bool SaveToScene(FILE* pFile);
	virtual bool LoadFromScene(FILE* pFile);
	void Enable(bool bEnable);


	void PlayAnimation(const wstring& strAnim);

public:
	void SetArmor(int iArmor) { m_iArmor = iArmor; }
	int GetArmor() const { return m_iArmor; }


public:
	ITEM_TYPE GetType() const { return m_eType; }
	ITEM GetItem() const { return m_eItem; }

	void SetOffset(float x, float y) { m_tOffset = Vec2(x, y); }
	Vec2 GetOffset() const { return m_tOffset; }
	void SetEquipment(CPlayerScript* pPlayer) { m_pPlayer = pPlayer; }

	
};

