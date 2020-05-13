#pragma once
#include "DirectionScript.h"


// �����ۿ� ���� �ּ�, �ִ� �������� ���ؼ� ���ݷ��� ��ȯ.
int GetItemDamage(ITEM eItem);


class CItemScript;
class CInventoryScript;
class CGravityScript;
class CJumpScript;
class CFlyawayScript;
class CArmorScript;

class CPlayerScript :
	public CDirectionScript
{
public:
	CPlayerScript();
	virtual ~CPlayerScript();
	CLONE(CPlayerScript);

public:
	static bool g_bBench;


private:

	CGameObject*		 m_pTorch; // �տ� �� Torch�� ó���Ѵ�.
	bool				 m_bTorch;

	CItemScript*		 m_pItem;
	CInventoryScript*	 m_pInven;


	CJumpScript*		 m_pJump;
	CGravityScript*		 m_pGravity;
	CFlyawayScript*		 m_pFly;

	CArmorScript*		m_pHelmet;
	CArmorScript*		m_pArmor;
	CArmorScript*		m_pLeggings;

	float				m_fSpeed;
	int					m_iArmor;

	int					m_iHPMax;
	int					m_iHP;

	bool				m_bAlpha;
	bool				m_bAttacked; // ���ݹ��� ��Ȳ.
	float				m_fAttacked; // ���� ���� ���Ŀ� �ð��� ���ش�.
	float				m_fAlpha;    // �������� Alpha��
	float				m_fAlphaPart;// Alpha ���ݿ� ����� Second

	float				m_fAlphaTime;		
	float				m_fAlphaLimitTime;	// Alphaó�� ���� �ð�.

private:
	void PlayAnimation(const wstring& strName);
	void SetArmor();
	void AddHP(int iHP) { m_iHP += iHP; if (m_iHP > m_iHPMax) m_iHP = m_iHPMax; }
	void SetTorch(ITEM eItem);
public:
	// ��������.
	void Attacked(int iDamage);
	int Attack();
	bool IsAlpha() const { return m_bAlpha; }

	int GetHPMax() const { return m_iHPMax; }
	int GetHP() const { return m_iHP; }

public:
	bool IsSwayItem();
	bool IsDigable(ITEM eType); // �� ITEM�� �ش� Tool�� �̿��Ͽ� ���� �� �ִ��� �Ǵ��Ѵ�.
	bool IsFall() const;
	void SetHelmet(CArmorScript* pHelmet);
	void SetArmor(CArmorScript* pArmor);
	void SetLeggings(CArmorScript* pLeggings);
	void SetDir(int iDir);

	bool IsJump() const;
	bool IsBench(CTileMap2D* pTileMap) const;
	// ��� TileMap������ Bench�ΰ� ������ �ʿ���.

public:
	int GetArmor() const { return m_iArmor; }

public:
	void SetHandleItem(CItemScript* pItem);
	CItemScript* GetHandleItem() const { return m_pItem; }
	void SetInventory(CInventoryScript* pScript) { m_pInven = pScript; }
	CInventoryScript* GetInven() const { return m_pInven; }
	virtual void Update();
	virtual void LateUpdate();
	virtual void Awake();
	virtual void TileCollision();

	void Input();

	virtual bool SaveToScene(FILE* pFile);
	virtual bool LoadFromScene(FILE* pFile);

	void Flyaway(const Vec3& tDir, const Vec3& tSpeed, const Vec3& tChange, float fTime);

private:
	void ItemOffsetUpdate();
};

