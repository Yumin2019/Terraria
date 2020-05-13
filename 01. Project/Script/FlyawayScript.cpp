#include "stdafx.h"
#include "FlyawayScript.h"


CFlyawayScript::CFlyawayScript() :
	CScript((UINT)SCRIPT_TYPE::FLYAWAYSCRIPT)
{
}


CFlyawayScript::~CFlyawayScript()
{
}

void CFlyawayScript::AddFly(const Vec3 & tDir, const Vec3 & tSpeed, const Vec3 & tChange, float fPlayTime)
{
	Flyaway tFly = Flyaway(tDir, tSpeed, tChange, fPlayTime);
	m_FlyawayList.push_back(tFly);
}

void CFlyawayScript::Update()
{
	if (!m_FlyawayList.empty())
	{
		Vec3 tPos = GetTransform()->GetLocalPos();
		float fTime = DT;

		if (fTime == 0.f)
			int i = 0;

		list<Flyaway>::iterator iter;
		list<Flyaway>::iterator iterEnd = m_FlyawayList.end();

		for (iter = m_FlyawayList.begin(); iter != iterEnd;)
		{
			/* IT'S SHOW TIME 다양한 힘들의 집합. */

			Vec3 tMove = iter->tFlyDir * iter->tFlySpeed * fTime;
			// 방향 * 각 방향에 대한 가중치 * 시간 = 해당 시간에 이동해야하는 vec 를 구해서 pos에 더한다.

			tPos += tMove;
				
			// Power 조절.
			iter->fPowerTime += fTime;

			if (iter->fPowerTime >= 0.1f)
			{
				iter->fPowerTime -= 0.1f;
				iter->tFlySpeed -= iter->tFlyChange;
			}

			iter->fSecond += fTime;

			// 종료 시간이 지난 경우. 없애는 처리.
			if (iter->fSecond >= iter->fPlayTime)
			{
				iter = m_FlyawayList.erase(iter);
				iterEnd = m_FlyawayList.end();
				continue;
			}


			++iter;

			/*
			
				Vec3		tFlyDir;
				// this is dir 예를 들어 왼쪽하단 부터 시작하여 오른쪽 하단으로 끝나는 포물선을 그리고 싶다면... 1.f 1.f 0.f가 들어다면 된다.

				Vec3        tFlySpeed; // 각 Dir에 맞는 Speed를 의미한다.


				Vec3		tFlyChange; // 파워 조절 값.
				float		fPowerTime; // 파워값 조절에 사용하는 시간.

				float		fSecond;  // PlayTime 용도.
				float		fPlayTime;

			*/
		}

		// 최종적으로 구한 pos를 셋팅.
		GetTransform()->SetLocalPos(tPos);
	}

	/*
	이 FlayawayScript는 .. 사실상 오브젝트를 던지는 수단에 해당하기 때문에, 자연스럽게 표현할 수 있다.
	예를 들면 대포 같은 아이템을 사용하는 경우에 이용하기가 편할 것이다.
	*/
}

