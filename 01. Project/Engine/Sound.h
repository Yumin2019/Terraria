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
	// �⺻���ڴ� LOOP Load������ SetMode�� ���ؼ� �ɼ��� �������ִ� ������ �Ѵ�.


public:
	virtual void Load(const wstring& strFullPath);


};

