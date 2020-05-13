#include "stdafx.h"
#include "Animation2D.h"
#include "TimeMgr.h"
#include "ConstBuffer.h"
#include "Device.h"

#include "ResMgr.h"


CAnimation2D::CAnimation2D() :
	m_pAnimator(nullptr),
	m_iCurFrame(0),
	m_fSecond(0.f),
	m_eAniOption(ANIMATION_OPTION::AO_LOOP)
{
}


CAnimation2D::~CAnimation2D()
{
}

void CAnimation2D::FinalUpdate()
{
	
	m_fSecond += DT;

	while (m_vecFrame[m_iCurFrame].fDuration <= m_fSecond)
	{
		m_fSecond -= m_vecFrame[m_iCurFrame].fDuration;
		++m_iCurFrame;


		if (m_iCurFrame >= m_vecFrame.size())
		{
			// 애니메이터한테 명령해서 이 옵션이니까 이렇게 처리하라고 해.
			switch (m_eAniOption)
			{
			case ANIMATION_OPTION::AO_LOOP:
				m_iCurFrame = 0;

				break;

			case ANIMATION_OPTION::AO_ONCE_LOOP: // 나중에
				break;

			case ANIMATION_OPTION::AO_DESTROY:
				break;

			case ANIMATION_OPTION::AO_ONCE_RETURN:
					m_pAnimator->PlayDefaultAnim();
				// 기존 정보 (이 Animation.. 이겠지. 이 정보를 초기로 돌리고, 다음 애니메이션으로 Default를 설정)
				break;
			}

		}
	}

}

void CAnimation2D::UpdateData()
{
	static CConstBuffer* pAnimBuffer = CDevice::GetInst()->FindConstBuffer(L"Anim2D");

	m_vecFrame[m_iCurFrame].pTexture->UpdateData((UINT)SHADER_TYPE::ST_PIXEL, 17); // 17번 animation 2D tex

	static Ani2D data = {};
	data.IsAni2D = true;
	data.tLength = m_vecFrame[m_iCurFrame].tLength;
	data.tLeftTop = m_vecFrame[m_iCurFrame].tLeftTop;
	data.tOffset = m_vecFrame[m_iCurFrame].tOffset;
	
	pAnimBuffer->SetData(&data);
	pAnimBuffer->UpdateData((UINT)SHADER_TYPE::ST_VERTEX | (UINT)SHADER_TYPE::ST_PIXEL);
}

void CAnimation2D::CreateHorizon(Ptr<CTexture> pTexture,
	const Vec2 & tLeftTop, const Vec2 & tLength,
	int iFrameCount, float fDuration, ANIMATION_OPTION eOption)
{
	m_eAniOption = eOption;

	// 전체 시간을 넣어주면, 시간 / count를 통해서 1장의 시간을 구한다.
	float fCurDuration = fDuration / iFrameCount;

	// 여기서 추가하는 이미지는 같은 간격으로 편집이 되어있고, 가로 한 줄로 되어있는 이미지이다.

	Vec2 tImageSize = Vec2(pTexture->GetWidth(), pTexture->GetHeight());
	Ani2DFrame tFrame = {};

	tFrame.fDuration = fCurDuration;
	tFrame.pTexture = pTexture;
	tFrame.tLength = tLength / tImageSize; // UV


	for (int i = 0; i < iFrameCount; ++i)
	{
		Vec2 tLT = tLeftTop;
		tLT.x += i * tLength.x;

		tFrame.tLeftTop = tLT / tImageSize; // UV좌표로 표현하기 위해서, 이미지의 사이즈로 나눠준다.

		m_vecFrame.push_back(tFrame);

	}
}

void CAnimation2D::CreateVertical(Ptr<CTexture> pTexture, 
	const Vec2 & tLeftTop, const Vec2 & tLength,
	int iFrameCount, float fDuration, ANIMATION_OPTION eOption)
{
	m_eAniOption = eOption;

	float fCurDuration = fDuration / iFrameCount;

	// 세로 한 줄.
	Vec2 tImageSize = Vec2(pTexture->GetWidth(), pTexture->GetHeight());
	Ani2DFrame tFrame = {};

	tFrame.fDuration = fCurDuration;
	tFrame.pTexture = pTexture;
	tFrame.tLength = tLength / tImageSize; // UV


	for (int i = 0; i < iFrameCount; ++i)
	{
		Vec2 tLT = tLeftTop;
		tLT.y += i * tLength.y;

		tFrame.tLeftTop = tLT / tImageSize; // UV좌표로 표현하기 위해서, 이미지의 사이즈로 나눠준다.

		m_vecFrame.push_back(tFrame);

	}
}

void CAnimation2D::CreateAnimation(const vector<Ani2DFrame>& vecAnim, ANIMATION_OPTION eOption)
{
	if (vecAnim.empty())
		assert(false);

	m_eAniOption = eOption;
	m_vecFrame = vecAnim;
}

bool CAnimation2D::SaveToScene(FILE * pFile)
{
	UINT iFrameCount = m_vecFrame.size();
	fwrite(&iFrameCount, 4, 1, pFile);

	for (UINT i = 0; i < iFrameCount; ++i)
	{
		fwrite(&m_vecFrame[i], sizeof(Ani2DFrame), 1, pFile);

		if (m_vecFrame[i].pTexture != nullptr)
		{
			SaveWString(pFile, m_vecFrame[i].pTexture->GetName());
			SaveWString(pFile, m_vecFrame[i].pTexture->GetPath());
		}
	}

	fwrite(&m_iCurFrame, 4, 1, pFile);
	fwrite(&m_fSecond, 4, 1, pFile);
	fwrite(&m_eAniOption, sizeof(ANIMATION_OPTION), 1, pFile);



	return true;
}

bool CAnimation2D::LoadFromScene(FILE * pFile)
{
	UINT iFrameCount = 0;
	fread(&iFrameCount, 4, 1, pFile);

	for (UINT i = 0; i < iFrameCount; ++i)
	{
		Ani2DFrame tFrame = {};
		fread(&tFrame, sizeof(Ani2DFrame), 1, pFile);

		if (tFrame.pTexture != nullptr)
		{
			wstring strName = LoadWString(pFile);
			wstring strPath = LoadWString(pFile);
			
			// 쓰레기값 .
			memset(&tFrame.pTexture, 0, sizeof(void*));
			tFrame.pTexture = CResMgr::GetInst()->FindRes<CTexture>(strName);

			if (tFrame.pTexture == nullptr)
			{
				tFrame.pTexture = CResMgr::GetInst()->Load<CTexture>(strName, strPath);
			}
		}

		m_vecFrame.push_back(tFrame);
	}

	fread(&m_iCurFrame, 4, 1, pFile);
	fread(&m_fSecond, 4, 1, pFile);
	fread(&m_eAniOption, sizeof(ANIMATION_OPTION), 1, pFile);


	return true;
}
