#pragma once
#include "Component.h"
#include "Texture.h"
#include "Ptr.h"

class CAnimation2D;

class CAnimator2D :
	public CComponent
{
public:
	CAnimator2D();
	CAnimator2D(const CAnimator2D& anim);
	virtual ~CAnimator2D();


	CLONE(CAnimator2D);

private:
	map<wstring, CAnimation2D*>	m_mapAnim; 
	// Animator�� �������� �ִϸ��̼��� ��� �ְ�,
	// �ִϸ��̼��� �������� ������ ������ ������ �ִ´�.
	// �ִϸ��̼� ��ü���� ���� Clip�� �����ϴ� ���� �ƴ�, �� Ŭ�� ��ü�� �ִϸ��̼� ��ü �ϳ��� �����Ǵ� �ɷ� �����Ѵ�.

	CAnimation2D*				m_pCurAnim; // �±�����, ������� �̰� m_pCurClip���� �̸���������.
	wstring						m_strDefaultAni; // �⺻ �ִϸ��̼��� �������ش�. ONCE_RETURN �ɼǿ��� ���

	bool						m_bPause; // �ִϸ��̼��� ���� �����̴�.
										  // �� ���� ���� �ִ� ��쿡�� �ִϸ��̼ǿ� ���� ó���� ���� �ʴ´�.

public:
	void SetDefaultAni(const wstring& strName) { m_strDefaultAni = strName; }
	void SetPause(bool bPause) { m_bPause = bPause; }

	bool IsPause() const { return m_bPause; }
	wstring GetCurAnim() const;
	size_t GetCurIdx() const;

	map<wstring, CAnimation2D*>& GetAnimations() { return m_mapAnim; }
	const wstring& GetDefaultAni() const { return m_strDefaultAni; }

public:
	virtual void FinalUpdate();


public:
	bool AddAnimation(const wstring& strKey, Ptr<CTexture> pTex, const Vec2& tLeftTop, const Vec2& tLength,
		int iFrameCount, float fDuration = 1.f, ANIMATION_OPTION eOption = ANIMATION_OPTION::AO_LOOP, bool bHorizon = false);

	void PlayDefaultAnim() { PlayAnimation(m_strDefaultAni); }
	// ������ �̹���, ���η� �̾��� �̹����� LeftTop�� Length�� �������� �� �����Ӿ� ����, �ִϸ��̼��� ������ִ�
	// �Լ��̴�. 

	void PlayAnimation(const wstring& strAnimKey); // �ִϸ��̼� ȣ��
	CAnimation2D* FindAnimation(const wstring& strKey); 
	void UpdateData(); // �ִϸ��̼ǿ��� ����� �����͸� �ѱ�� �Լ��� UV��ǥ�� ���� �������� ������ �ѱ�µ� ���ȴ�.
	static void ClearData(); // ������ ����� ������ Clear�ϴµ� �̿�.

	void StopAnimation(); // �ʱ���� & stop

	virtual bool SaveToScene(FILE* pFile);
	virtual bool LoadFromScene(FILE* pFile);

};

