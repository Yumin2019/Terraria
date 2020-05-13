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
			/* IT'S SHOW TIME �پ��� ������ ����. */

			Vec3 tMove = iter->tFlyDir * iter->tFlySpeed * fTime;
			// ���� * �� ���⿡ ���� ����ġ * �ð� = �ش� �ð��� �̵��ؾ��ϴ� vec �� ���ؼ� pos�� ���Ѵ�.

			tPos += tMove;
				
			// Power ����.
			iter->fPowerTime += fTime;

			if (iter->fPowerTime >= 0.1f)
			{
				iter->fPowerTime -= 0.1f;
				iter->tFlySpeed -= iter->tFlyChange;
			}

			iter->fSecond += fTime;

			// ���� �ð��� ���� ���. ���ִ� ó��.
			if (iter->fSecond >= iter->fPlayTime)
			{
				iter = m_FlyawayList.erase(iter);
				iterEnd = m_FlyawayList.end();
				continue;
			}


			++iter;

			/*
			
				Vec3		tFlyDir;
				// this is dir ���� ��� �����ϴ� ���� �����Ͽ� ������ �ϴ����� ������ �������� �׸��� �ʹٸ�... 1.f 1.f 0.f�� ���ٸ� �ȴ�.

				Vec3        tFlySpeed; // �� Dir�� �´� Speed�� �ǹ��Ѵ�.


				Vec3		tFlyChange; // �Ŀ� ���� ��.
				float		fPowerTime; // �Ŀ��� ������ ����ϴ� �ð�.

				float		fSecond;  // PlayTime �뵵.
				float		fPlayTime;

			*/
		}

		// ���������� ���� pos�� ����.
		GetTransform()->SetLocalPos(tPos);
	}

	/*
	�� FlayawayScript�� .. ��ǻ� ������Ʈ�� ������ ���ܿ� �ش��ϱ� ������, �ڿ������� ǥ���� �� �ִ�.
	���� ��� ���� ���� �������� ����ϴ� ��쿡 �̿��ϱⰡ ���� ���̴�.
	*/
}

