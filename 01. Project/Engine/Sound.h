#pragma once
#include "Resource.h"
#include <FMOD/fmod.hpp>

using namespace FMOD;

#ifdef _DEBUG
#pragma comment(lib, "FMOD/fmodL_vc.lib")
#else
#pragma comment(lib, "FMOD/fmod_vc.lib")
#endif



class CSound :
	public CResource
{
public:
	CSound();
	virtual ~CSound();

private:
	Sound*		m_pSound;
	bool		m_bLoop;
	string		m_strPath;

public:
	bool IsLoop() const { return m_bLoop; }
	Sound* GetSound() const { return m_pSound; }

public:
	void SetOption(bool bLoop);
	// 기본인자는 LOOP Load이전에 SetMode를 통해서 옵션을 셋팅해주는 것으로 한다.


public:
	virtual void Load(const wstring& strFullPath);


};

