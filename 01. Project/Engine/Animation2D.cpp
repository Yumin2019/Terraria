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
			// �ִϸ��������� ����ؼ� �� �ɼ��̴ϱ� �̷��� ó���϶�� ��.
			switch (m_eAniOption)
			{
			case ANIMATION_OPTION::AO_LOOP:
				m_iCurFrame = 0;

				break;

			case ANIMATION_OPTION::AO_ONCE_LOOP: // ���߿�
				break;

			case ANIMATION_OPTION::AO_DESTROY:
				break;

			case ANIMATION_OPTION::AO_ONCE_RETURN:
					m_pAnimator->PlayDefaultAnim();
				// ���� ���� (�� Animation.. �̰���. �� ������ �ʱ�� ������, ���� �ִϸ��̼����� Default�� ����)
				break;
			}

		}
	}

}

void CAnimation2D::UpdateData()
{
	static CConstBuffer* pAnimBuffer = CDevice::GetInst()->FindConstBuffer(L"Anim2D");

	m_vecFrame[m_iCurFrame].pTexture->UpdateData((UINT)SHADER_TYPE::ST_PIXEL, 17); // 17�� animation 2D tex

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

	// ��ü �ð��� �־��ָ�, �ð� / count�� ���ؼ� 1���� �ð��� ���Ѵ�.
	float fCurDuration = fDuration / iFrameCount;

	// ���⼭ �߰��ϴ� �̹����� ���� �������� ������ �Ǿ��ְ�, ���� �� �ٷ� �Ǿ��ִ� �̹����̴�.

	Vec2 tImageSize = Vec2(pTexture->GetWidth(), pTexture->GetHeight());
	Ani2DFrame tFrame = {};

	tFrame.fDuration = fCurDuration;
	tFrame.pTexture = pTexture;
	tFrame.tLength = tLength / tImageSize; // UV


	for (int i = 0; i < iFrameCount; ++i)
	{
		Vec2 tLT = tLeftTop;
		tLT.x += i * tLength.x;

		tFrame.tLeftTop = tLT / tImageSize; // UV��ǥ�� ǥ���ϱ� ���ؼ�, �̹����� ������� �����ش�.

		m_vecFrame.push_back(tFrame);

	}
}

void CAnimation2D::CreateVertical(Ptr<CTexture> pTexture, 
	const Vec2 & tLeftTop, const Vec2 & tLength,
	int iFrameCount, float fDuration, ANIMATION_OPTION eOption)
{
	m_eAniOption = eOption;

	float fCurDuration = fDuration / iFrameCount;

	// ���� �� ��.
	Vec2 tImageSize = Vec2(pTexture->GetWidth(), pTexture->GetHeight());
	Ani2DFrame tFrame = {};

	tFrame.fDuration = fCurDuration;
	tFrame.pTexture = pTexture;
	tFrame.tLength = tLength / tImageSize; // UV


	for (int i = 0; i < iFrameCount; ++i)
	{
		Vec2 tLT = tLeftTop;
		tLT.y += i * tLength.y;

		tFrame.tLeftTop = tLT / tImageSize; // UV��ǥ�� ǥ���ϱ� ���ؼ�, �̹����� ������� �����ش�.

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
			
			// �����Ⱚ .
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
