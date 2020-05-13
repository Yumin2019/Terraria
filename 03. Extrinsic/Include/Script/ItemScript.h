#pragma once
#include "DirectionScript.h"


class CItemScript :
	public CDirectionScript
{
public:
	CItemScript();
	virtual ~CItemScript();
	CLONE(CItemScript);

private:
	ITEM_TYPE		m_eType; // �������� ����
	ITEM			m_eItem; // � ������.



	bool			m_bSway;
	float			m_fSecond;
	float			m_fSwayTime;
	float			m_fAngle;
	float			m_fCoefficient;

	CGameObject*			m_pUIObj;
	class CUIItemScript*	m_pUIItem;
	class CArmorScript*		m_pArmor; // ArmorScript�� ����ϴ� ���(��)�� ���ȴ�.



public:
	virtual void Awake();
	virtual void Update() {}
	virtual void LateUpdate();

public:

	void SetArmor(CArmorScript* pArmor) { m_pArmor = pArmor; }
	CArmorScript* GetArmor() const { return m_pArmor; }

	bool IsSway() const { return m_bSway; }
	void StopSway();

	CGameObject* GetUIObj() const { return m_pUIObj; }
	void SetUIObj(CGameObject* pUIObj);

	CUIItemScript* GetUIItem() const { return m_pUIItem; }

public:
	ITEM_TYPE GetType() const { return m_eType; }
	ITEM	  GetItem() const { return m_eItem; }
	void SetType(ITEM_TYPE eType, ITEM eItem) { m_eItem = eItem; m_eType = eType; }

public:
	void SwayItem(float fSwayTime);

	virtual bool LoadFromScene(FILE* pFile);
	virtual bool SaveToScene(FILE* pFile);
};

