#pragma once
#include "DirectionScript.h"


// 아이템에 따라서 최소, 최대 데미지를 통해서 공격력을 반환.
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

	CGameObject*		 m_pTorch; // 손에 든 Torch를 처리한다.
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
	bool				m_bAttacked; // 공격받은 상황.
	float				m_fAttacked; // 공격 받은 이후에 시간을 재준다.
	float				m_fAlpha;    // 실질적인 Alpha값
	float				m_fAlphaPart;// Alpha 간격에 사용할 Second

	float				m_fAlphaTime;		
	float				m_fAlphaLimitTime;	// Alpha처리 진행 시간.

private:
	void PlayAnimation(const wstring& strName);
	void SetArmor();
	void AddHP(int iHP) { m_iHP += iHP; if (m_iHP > m_iHPMax) m_iHP = m_iHPMax; }
	void SetTorch(ITEM eItem);
public:
	// 전투관련.
	void Attacked(int iDamage);
	int Attack();
	bool IsAlpha() const { return m_bAlpha; }

	int GetHPMax() const { return m_iHPMax; }
	int GetHP() const { return m_iHP; }

public:
	bool IsSwayItem();
	bool IsDigable(ITEM eType); // 이 ITEM을 해당 Tool을 이용하여 없앨 수 있는지 판단한다.
	bool IsFall() const;
	void SetHelmet(CArmorScript* pHelmet);
	void SetArmor(CArmorScript* pArmor);
	void SetLeggings(CArmorScript* pLeggings);
	void SetDir(int iDir);

	bool IsJump() const;
	bool IsBench(CTileMap2D* pTileMap) const;
	// 어느 TileMap에서의 Bench인가 정보도 필요함.

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

