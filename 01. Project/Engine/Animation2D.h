#pragma once
#include "Entity.h"
#include "Animator2D.h"

#include "Ptr.h"


// 2D Animation 1 Frame ����

typedef struct _tag2DFrame
{
	Ptr<CTexture> pTexture;
	Vec2		  tLeftTop;
	Vec2		  tLength;
	Vec2		  tOffset;
	float	      fDuration;

	_tag2DFrame() :
		fDuration(0.f)
	{}

}Ani2DFrame, *PAni2DFrame;


/*

enum class ANIMATION_OPTION
{
	AO_LOOP,
	AO_ONCE_LOOP,
	AO_DESTROY,
	AO_ONCE_RETURN
};
*/

class CAnimation2D :
	public CEntity
{
public:
	CAnimation2D();
	virtual ~CAnimation2D();

	friend class CAnimator2D;

private:
	CAnimator2D*			m_pAnimator; // �ڽ��� �̿��Ͽ� ������ Animator�� �˰� �ֵ��� �Ѵ�.
	vector<Ani2DFrame>		m_vecFrame; // �ϳ��� �ִϸ��̼� ��ü�� �������� ������ ������ ������ �ִ´�.
	int						m_iCurFrame; // �ִϸ��̼��� ���� ������.
	float					m_fSecond; // �ִϸ��̼��� �����µ� ����� float
	ANIMATION_OPTION		m_eAniOption;

public:
	vector<Ani2DFrame>&  GetFrameVec() { return m_vecFrame; }

	ANIMATION_OPTION GetOption() const { return m_eAniOption; }
	void SetOption(ANIMATION_OPTION eOption) { m_eAniOption = eOption; }

public:
	void FinalUpdate(); // Finish ���¸� ���߰�, �ƴ϶��, Frame������ Update�ϴ� �Լ��̴�.
	void UpdateData(); // Frame ������ �ѱ�� ������ۿ� �������� ������ �����Ͽ� Shader�Լ����� ����ϵ��� �Ѵ�.

	void Play()
	{
		m_iCurFrame = 0;
		m_fSecond = 0.f;
	}


public:
	void CreateHorizon(Ptr<CTexture> pTexture, const Vec2& tLeftTop, const Vec2& tLength,
		int iFrameCount, float fDuration = 1.f, ANIMATION_OPTION eOption = ANIMATION_OPTION::AO_LOOP);
	// �־��� ������ ����, Frame���� Texture�� �����ϵ��� �ϰ�, ���� ����, ���̸� ���ؼ� 
	// �� �����Ӹ��� ������ �����ϵ��� �Ѵ�.

	void CreateVertical(Ptr<CTexture> pTexture, const Vec2& tLeftTop, const Vec2& tLength,
		int iFrameCount, float fDuration = 1.f, ANIMATION_OPTION eOption = ANIMATION_OPTION::AO_LOOP);

	void CreateAnimation(const vector<Ani2DFrame>& vecAnim, ANIMATION_OPTION eOption = ANIMATION_OPTION::AO_LOOP);


	CLONE(CAnimation2D);

private:
	void SetAnimator2D(CAnimator2D* pAnimator2D) { m_pAnimator = pAnimator2D; }
	bool SaveToScene(FILE* pFile);
	bool LoadFromScene(FILE* pFile);
};

