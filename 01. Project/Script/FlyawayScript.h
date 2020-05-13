#pragma once
#include <Engine/Script.h>

typedef struct _tagFlyaway
{
	Vec3		tFlyDir;
	// this is dir ���� ��� �����ϴ� ���� �����Ͽ� ������ �ϴ����� ������ �������� �׸��� �ʹٸ�... 1.f 1.f 0.f�� ���ٸ� �ȴ�.

	Vec3        tFlySpeed; // �� Dir�� �´� Speed�� �ǹ��Ѵ�.
	Vec3		tFlyChange; // �Ŀ� ���� ��.
	float		fPowerTime; // �Ŀ��� ������ ����ϴ� �ð�.

	float		fSecond;  // PlayTime �뵵.
	float		fPlayTime;

	_tagFlyaway() :
		fPowerTime(0.f),
		fSecond(0.f),
		fPlayTime(0.f)
	{
	}

	_tagFlyaway(const Vec3& tDir, const Vec3& tSpeed, const Vec3& tChange, float fPlayTime) :
		tFlyDir(tDir),
		tFlySpeed(tSpeed),
		tFlyChange(tChange),
		fPowerTime(0.f),
		fSecond(0.f),
		fPlayTime(fPlayTime)
	{}

}Flyaway, *PFlyAway;

class CFlyawayScript :
	public CScript
{
public:
	CFlyawayScript();
	~CFlyawayScript();
	CLONE(CFlyawayScript);

private:
	list<Flyaway> m_FlyawayList;

public:
	void AddFly(const Vec3& tDir, const Vec3& tSpeed, const Vec3& tChange, float fPlayTime);
	bool IsFlyaway() const { return (m_FlyawayList.size() != 0); } // flayway�� �����ϴ� ?
	void Clear() { m_FlyawayList.clear(); }

public:
	virtual void Update();

};

