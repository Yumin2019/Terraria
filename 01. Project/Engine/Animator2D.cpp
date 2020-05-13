#include "stdafx.h"
#include "Animator2D.h"
#include "Animation2D.h"
#include "DebugMgr.h"

#include "ConstBuffer.h"
#include "Device.h"

CAnimator2D::CAnimator2D() :
	CComponent(COMPONENT_TYPE::ANIMATOR2D),
	m_pCurAnim(nullptr),
	m_bPause(true)
{
}

CAnimator2D::CAnimator2D(const CAnimator2D & anim) :
	CComponent(anim)
{

	// �ִϸ��̼��� �����ؼ�, Ű���� �����ϰ� �־ �߰����ش�.
	// �̶� ���ο� Animator �������ش�. 

	m_mapAnim.clear();

	map<wstring, CAnimation2D*>::const_iterator iter;
	map<wstring, CAnimation2D*>::const_iterator iterEnd = anim.m_mapAnim.end();

	for (iter = anim.m_mapAnim.begin(); iter != iterEnd; ++iter)
	{
		CAnimation2D* pAni = iter->second;
		pAni = pAni->Clone();
		pAni->SetAnimator2D(this);
		m_mapAnim.insert(make_pair(iter->first, pAni));
	}

	m_strDefaultAni = anim.m_strDefaultAni;

	// ���� �ִϸ��̼��� �����ϴ� ��쿡 �̸� ��Ƶ���.
	m_pCurAnim = FindAnimation(m_strDefaultAni); 

	if (anim.m_pCurAnim != nullptr)
		PlayAnimation(anim.m_pCurAnim->GetName());

}


CAnimator2D::~CAnimator2D() 
{
	Safe_Delete_Map(m_mapAnim);
}


wstring CAnimator2D::GetCurAnim() const
{
	if (m_pCurAnim)
		return m_pCurAnim->GetName();

	return L"";
}

size_t CAnimator2D::GetCurIdx() const
{
	if(m_pCurAnim)
		return m_pCurAnim->m_iCurFrame;

	return -1;
}

void CAnimator2D::FinalUpdate()
{
	if (m_pCurAnim && !m_bPause)
	{
		m_pCurAnim->FinalUpdate();
	}

}

void CAnimator2D::StopAnimation()
{
	SetPause(true);

	if (m_pCurAnim)
	{
		m_pCurAnim->m_iCurFrame = 0;
		m_pCurAnim->m_fSecond = 0.f;
	}
}

bool CAnimator2D::SaveToScene(FILE * pFile)
{
	UINT iType = (UINT)GetComponentType();
	fwrite(&iType, 4, 1, pFile);

	UINT iAnimCount = m_mapAnim.size();
	fwrite(&iAnimCount, 4, 1, pFile);

	for (auto& pair : m_mapAnim)
	{
		SaveWString(pFile, pair.first);
		pair.second->SaveToScene(pFile);
	}

	fwrite(&m_pCurAnim, sizeof(void*), 1, pFile);
	if (m_pCurAnim != nullptr)
		SaveWString(pFile, m_pCurAnim->GetName());

	SaveWString(pFile, m_strDefaultAni);

	return true;
}

bool CAnimator2D::LoadFromScene(FILE * pFile)
{
	UINT iAnimCount = m_mapAnim.size();
	fread(&iAnimCount, 4, 1, pFile);

	for (UINT i = 0; i < iAnimCount; ++i)
	{
		wstring strName = LoadWString(pFile);
		CAnimation2D* pAnim = new CAnimation2D;
		pAnim->SetName(strName);
		pAnim->SetAnimator2D(this);
		pAnim->LoadFromScene(pFile);
		m_mapAnim.insert(make_pair(strName, pAnim));
	}

	void* pData = nullptr;
	fread(&pData, sizeof(void*), 1, pFile);

	if (pData != nullptr)
	{
		wstring strName = LoadWString(pFile);
		m_pCurAnim = FindAnimation(strName);
	}

	m_strDefaultAni = LoadWString(pFile);

	if (m_strDefaultAni == L"")
	{
		if(!m_mapAnim.empty())
			m_strDefaultAni = m_mapAnim.begin()->second->GetName();
	}

	return true;
}

bool CAnimator2D::AddAnimation(const wstring & strKey, Ptr<CTexture> pTex, 
	const Vec2 & tLeftTop, const Vec2 & tLength, 
	int iFrameCount, float fDuration, ANIMATION_OPTION eOption, bool bHorizon)
{
	CAnimation2D* pAnim = FindAnimation(strKey);

	// strKey�� ���ؼ� �̹� �߰��� �ִϸ��̼��� �����ϴ� ��쿡�� �߰����� �ʴ´�.
	if (nullptr != pAnim)
		return false;

	// �߰��� �ؾ� �ϴ� ���
	pAnim = new CAnimation2D;

	if (bHorizon)
		pAnim->CreateHorizon(pTex, tLeftTop, tLength, iFrameCount, fDuration, eOption);
	else
		pAnim->CreateVertical(pTex, tLeftTop, tLength, iFrameCount, fDuration, eOption);
	
	m_mapAnim.insert(make_pair(strKey, pAnim));
	pAnim->SetAnimator2D(this);
	pAnim->SetName(strKey);

	// ó�� �־��ִ� ���� �⺻ �ִϸ��̼��� �����صд�.
	if (m_mapAnim.size() == 1)
	{
		m_strDefaultAni = strKey;
		m_pCurAnim = pAnim; 

	}

	return true;
}



void CAnimator2D::PlayAnimation(const wstring & strAnimKey)
{

	if (m_pCurAnim)
	{
		// ���� �������� �ִϸ��̼��� ��쿡�� ó������ �ʵ��� �Ѵ�.
		if (m_pCurAnim->GetName() == strAnimKey)
			return;

		// ���ο� ���� ã�ƿ��� ������ �����͸� �⺻���� �������ش�.'
		m_pCurAnim->m_iCurFrame = 0;
		m_pCurAnim->m_fSecond = 0.f;
		// ������ ������ �� �������� ���� + Animator �������� �ٲ��� �ʿ�� ����.
	}
	

	m_pCurAnim = FindAnimation(strAnimKey);

	if (m_pCurAnim == nullptr)
	{
		// ã�ƿ� �ִϸ��̼��� �����ٸ�, �޼����� ������ش�.
		DBG_WARNNING(L"Can't Find Animation");
	}
}

CAnimation2D * CAnimator2D::FindAnimation(const wstring & strKey)
{
	map<wstring, CAnimation2D*>::iterator iter = m_mapAnim.find(strKey);

	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}

void CAnimator2D::UpdateData()
{
	if (m_pCurAnim != nullptr)
		m_pCurAnim->UpdateData();

	// �ִϸ��̼����� ����, UpdateData�Լ��� ȣ���Ͽ�, PixelShader�� �Ѱ� �־�� �ϴ� LeftTop + Length ������
	// �����Ѵ�.
}

void CAnimator2D::ClearData()
{
	// �ϳ��� ��ü�� ���� ó���� �� ���Ŀ� PipeLine���� ������ ����ϴ� ������ �״�� �����ְ� �ȴ�.
	// ����, �ִϸ����� ����� ���� ������Ʈ ���� �̰��� ������ ���� �� �����Ƿ� Data�� Clear���ֵ��� �Ѵ�.

	static CConstBuffer* pAnimBuffer = CDevice::GetInst()->FindConstBuffer(L"Anim2D");

	static Ani2D data = {};
	pAnimBuffer->SetData(&data);
	pAnimBuffer->UpdateData((UINT)SHADER_TYPE::ST_VERTEX | (UINT)SHADER_TYPE::ST_PIXEL);
}
