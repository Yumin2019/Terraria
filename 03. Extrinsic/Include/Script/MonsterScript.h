#pragma once
#include "DirectionScript.h"

/*
MonsterScript를 상속받는 Script들은....
	virtual void Awake();



	해당 함수들은 정의해줘야 한다. (부모껄 호출하고)
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

	// Monster Second : 기본으로 주어지는 값
	float	m_fSecond;
	float	m_fLimitTime;

	int		m_iHP;
	int		m_iHPMax;
	int		m_iAttackMin;
	int		m_iAttackMax;

public:
	// 전투 관련
	bool IsSlime() const { return m_bSlime; }
	int GetAttack() const;
	void Attacked(int iDamage);

	int GetHPMax() const { return m_iHPMax; }
	int GetHP() const { return m_iHP; }


public:
	CPlayerScript* GetPlayer() const { return m_pPlayer; }
	CFlyawayScript* GetFly() const { return m_pFly; }
	CGravityScript* GetGravity() const { return m_pGravity; }

	bool GetFind() const { return m_bFind; } // 플레이어를 찾았는지 여부.
	float GetSpeed() const { return m_fSpeed; }

public:
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }

	void SetMonsterUI(CMonsterHPScript* pHP) { m_pMonsterUI = pHP; }
	

protected:
	float GetPlayerDist() const; // 플레이어와의 거리를 반환.

	// 몬스터가 플레이어를 봐라보도록 한다. X의 거리차를 return.
	float LookAtPlayer();

public:
	virtual void Awake();
	virtual void Update(); // Distance에 따른 Find 변수 관리.
	virtual bool SaveToScene(FILE* pFile);
	virtual bool LoadFromScene(FILE* pFile);
	virtual void LateUpdate();
	virtual void TileCollision();

};

