#include "stdafx.h"
#include "Sound.h"
#include "ResMgr.h"


CSound::CSound() :
	CResource(RES_TYPE::SOUND),
	m_pSound(nullptr),
	m_bLoop(false) 
{
}


CSound::~CSound()
{
	if (m_pSound != nullptr)
		m_pSound->release();
}


void CSound::SetOption(bool bLoop)
{
	// 이미 생성된 이후라면 assert
	if (m_pSound != nullptr)
		assert(false);

	m_bLoop = bLoop;

	FMOD_MODE eMode = FMOD_DEFAULT;

	if (m_bLoop)
		eMode = FMOD_LOOP_NORMAL;

	System* pSystem = CResMgr::GetInst()->GetSoundSys();

	FMOD_RESULT result = pSystem->createSound(m_strPath.c_str(), eMode, nullptr, &m_pSound);

	if (FMOD_OK != result)
		assert(false);
}

void CSound::Load(const wstring & strFullPath)
{
	m_strPath.assign(strFullPath.begin(), strFullPath.end());
}
