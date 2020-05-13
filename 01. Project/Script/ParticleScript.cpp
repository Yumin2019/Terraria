#include "stdafx.h"
#include "ParticleScript.h"
#include "FlyawayScript.h"
#include "PoolMgr.h"



CParticleScript::CParticleScript() : 
	CScript((UINT)SCRIPT_TYPE::PARTICLESCRIPT),
	m_pFly(nullptr),
	m_bStart(false),
	m_fSecond(0.f),
	m_fLimitTime(0.f)
{
}


CParticleScript::~CParticleScript()
{
}

bool CParticleScript::Init(const Vec3& tCenterPos, float fRadius, ITEM eItem, float fMinRadian, float fMaxRadian, 
	float fLifeTime, float fSpeed, float fChangeSpeed)
{
	float fRatio = (float)(rand() % 70) + 30; // 70 ~ 99
	fRatio *= 0.01f;

	// 랜덤한 각도.
	float fAngle = (fMaxRadian - fMinRadian) * fRatio + fMinRadian; 

	// 비율에 따른 Center에서의 길이 차이.
	float fDist = fRadius * fRatio;

	Vec3 tDir;
	tDir.x = cosf(fAngle);
	tDir.y = sinf(fAngle);

	GetTransform()->SetLocalPos(tCenterPos + tDir * fDist);

	fSpeed *= fRatio;
	fChangeSpeed *= fRatio;
	fLifeTime *= fRatio;

	m_pFly->AddFly(tDir, Vec3(fSpeed, fSpeed, 0.f), Vec3(fChangeSpeed, fChangeSpeed, 0.f), fLifeTime);
	
	m_bStart = true;
	m_fLimitTime = fLifeTime;
	m_fSecond = 0.f;

	// 자신의 참조 인덱스를 설정해준다.

	int iX = 0;
	int iY = 0;

	switch (eItem)
	{
	case ITEM::DIRT:
	case ITEM::DIRT_WALL:

		iX = 0;
		iY = rand() % 3;
		break;

	case ITEM::STONE:
	case ITEM::STONE_WALL:
		iX = 1;
		iY = rand() % 3;
		break;

	case ITEM::WOOD:
	case ITEM::WOOD_WALL:

		iX = 28;
		iY = rand() % 3;
		break;

	case ITEM::CACTUS:
	case ITEM::CACTUS_WALL:

		iX = 2;
		iY = rand() % 3;
		break;

	case ITEM::TORCH:
		iX = 35; iY = rand() % 3;

		break;

	case ITEM::ORANGE_TORCH:	iX = 35; iY = rand() % 3;	break; //
	case ITEM::WHITE_TORCH:		iX = 76; iY = rand() % 3;	break;  //
	case ITEM::VIOLET_TORCH:	iX = 12; iY = rand() % 3 + 3;	break; //

	case ITEM::BLUE_TORCH:		iX = 13; iY = rand() % 3 + 3;		break;
	case ITEM::GREEN_TORCH:		iX = 10; iY = rand() % 3 + 3;		break;

	case ITEM::PINK_TORCH:		iX = 41; iY = rand() % 3 + 3;		break; //
	case ITEM::YELLOW_TORCH:	iX = 42; iY = rand() % 3 + 3;		break;//

	case ITEM::RED_TORCH:		iX = 35; iY = rand() % 3 + 6; break; //

	case ITEM::ICE_TORCH:		iX = 11; iY = rand() % 3 + 3;	break; //

	default:

		iX = 0;
		iY = rand() % 3 + 3;

		break;
	}

	Ptr<CMaterial> pMtrl = GetMeshRender()->GetSharedMaterial();
	pMtrl->SetData(SHADER_PARAM::INT_0, &iX);
	pMtrl->SetData(SHADER_PARAM::INT_1, &iY);

	float fLight = 1.f;
	pMtrl->SetData(SHADER_PARAM::FLOAT_0, &fLight);

	return true;
}

void CParticleScript::Awake()
{
	const vector<CScript*>& vecScript = GetObj()->GetScriptVec();

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		if (vecScript[i]->GetScriptType() == (UINT)SCRIPT_TYPE::FLYAWAYSCRIPT)
		{
			m_pFly = (CFlyawayScript*)vecScript[i];
			break;
		}
	}

	if (m_pFly == nullptr)
		assert(false);

}

void CParticleScript::Update()
{
	if (m_bStart)
	{
		m_fSecond += DT;

		if (m_fSecond >= m_fLimitTime)
		{
			CPoolMgr::GetInst()->ReturnParticle(this);
			return;
		}

		float fRatio = m_fSecond / m_fLimitTime;
		GetTransform()->SetLocalRot(Vec3(0.f, 0.f, fRatio * XM_PI));
		GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fRatio);
	}
}


