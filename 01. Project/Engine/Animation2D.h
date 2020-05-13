#pragma once
#include "Entity.h"
#include "Animator2D.h"

#include "Ptr.h"


// 2D Animation 1 Frame 정보

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
	CAnimator2D*			m_pAnimator; // 자신을 이용하여 돌리는 Animator를 알고 있도록 한다.
	vector<Ani2DFrame>		m_vecFrame; // 하나의 애니메이션 객체가 여러개의 프레임 정보를 가지고 있는다.
	int						m_iCurFrame; // 애니메이션의 현재 프레임.
	float					m_fSecond; // 애니메이션을 돌리는데 사용할 float
	ANIMATION_OPTION		m_eAniOption;

public:
	vector<Ani2DFrame>&  GetFrameVec() { return m_vecFrame; }

	ANIMATION_OPTION GetOption() const { return m_eAniOption; }
	void SetOption(ANIMATION_OPTION eOption) { m_eAniOption = eOption; }

public:
	void FinalUpdate(); // Finish 상태면 멈추고, 아니라면, Frame정보를 Update하는 함수이다.
	void UpdateData(); // Frame 정보를 넘기는 상수버퍼에 프레임의 정보를 셋팅하여 Shader함수에서 사용하도록 한다.

	void Play()
	{
		m_iCurFrame = 0;
		m_fSecond = 0.f;
	}


public:
	void CreateHorizon(Ptr<CTexture> pTexture, const Vec2& tLeftTop, const Vec2& tLength,
		int iFrameCount, float fDuration = 1.f, ANIMATION_OPTION eOption = ANIMATION_OPTION::AO_LOOP);
	// 넣어준 정보를 토대로, Frame마다 Texture를 참조하도록 하고, 시작 지점, 길이를 통해서 
	// 한 프레임마다 정보를 셋팅하도록 한다.

	void CreateVertical(Ptr<CTexture> pTexture, const Vec2& tLeftTop, const Vec2& tLength,
		int iFrameCount, float fDuration = 1.f, ANIMATION_OPTION eOption = ANIMATION_OPTION::AO_LOOP);

	void CreateAnimation(const vector<Ani2DFrame>& vecAnim, ANIMATION_OPTION eOption = ANIMATION_OPTION::AO_LOOP);


	CLONE(CAnimation2D);

private:
	void SetAnimator2D(CAnimator2D* pAnimator2D) { m_pAnimator = pAnimator2D; }
	bool SaveToScene(FILE* pFile);
	bool LoadFromScene(FILE* pFile);
};

