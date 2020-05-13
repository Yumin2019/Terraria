#pragma once
#include "DirectionScript.h"

/*
MonsterScript�� ��ӹ޴� Script����....
	virtual void Awake();



	�ش� �Լ����� ��������� �Ѵ�. (�θ� ȣ���ϰ�)
	virtual void LateUpdate();
	virtual void OnCollisionEnter(CCollider2D* pOther) = 0;
	virtual bool SaveToScene(FILE* pFile);
	virtual bool LoadFromScene(FILE* pFile);

*/


class CMonsterScript :
	public CDirectionScript
{
public:
	CMonsterScript(UINT iScriptType);
	CMonsterScript();
	virtual ~CMonsterScript();

	CMonsterScript* Clone() { assert(false); return nullptr; } 

public:
	static int g_iMonster;

private:
	class CPlayerScript* m_pPlayer;
	class CFlyawayScript* m_pFly;
	class CGravityScript* m_pGravity;
	class CMonsterHPScript* m_pMonsterUI;

	float	m_fFindDist;
	bool	m_bFind;
	bool	m_bStop;

protected:
	bool	m_bSlime;

	float	m_fSpeed;

	// Monster Second : �⺻���� �־����� ��
	float	m_fSecond;
	float	m_fLimitTime;

	int		m_iHP;
	int		m_iHPMax;
	int		m_iAttackMin;
	int		m_iAttackMax;

public:
	// ���� ����
	bool IsSlime() const { return m_bSlime; }
	int GetAttack() const;
	void Attacked(int iDamage);

	int GetHPMax() const { return m_iHPMax; }
	int GetHP() const { return m_iHP; }


public:
	CPlayerScript* GetPlayer() const { return m_pPlayer; }
	CFlyawayScript* GetFly() const { return m_pFly; }
	CGravityScript* GetGravity() const { return m_pGravity; }

	bool GetFind() const { return m_bFind; } // �÷��̾ ã�Ҵ��� ����.
	float GetSpeed() const { return m_fSpeed; }

public:
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }

	void SetMonsterUI(CMonsterHPScript* pHP) { m_pMonsterUI = pHP; }
	

protected:
	float GetPlayerDist() const; // �÷��̾���� �Ÿ��� ��ȯ.

	// ���Ͱ� �÷��̾ ���󺸵��� �Ѵ�. X�� �Ÿ����� return.
	float LookAtPlayer();

public:
	virtual void Awake();
	virtual void Update(); // Distance�� ���� Find ���� ����.
	virtual bool SaveToScene(FILE* pFile);
	virtual bool LoadFromScene(FILE* pFile);
	virtual void LateUpdate();
	virtual void TileCollision();

};

