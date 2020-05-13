#pragma once
#include <Engine/Script.h>

typedef struct _tagFlyaway
{
	Vec3		tFlyDir;
	// this is dir 예를 들어 왼쪽하단 부터 시작하여 오른쪽 하단으로 끝나는 포물선을 그리고 싶다면... 1.f 1.f 0.f가 들어다면 된다.

	Vec3        tFlySpeed; // 각 Dir에 맞는 Speed를 의미한다.
	Vec3		tFlyChange; // 파워 조절 값.
	float		fPowerTime; // 파워값 조절에 사용하는 시간.

	float		fSecond;  // PlayTime 용도.
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
	bool IsFlyaway() const { return (m_FlyawayList.size() != 0); } // flayway가 존재하니 ?
	void Clear() { m_FlyawayList.clear(); }

public:
	virtual void Update();

};

