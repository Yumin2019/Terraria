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
	// Animator는 여러개의 애니메이션을 들고 있고,
	// 애니메이션은 여러개의 프레임 정보를 가지고 있는다.
	// 애니메이션 객체에서 여러 Clip을 관리하는 것이 아닌, 각 클립 자체가 애니메이션 객체 하나로 관리되는 걸로 생각한다.

	CAnimation2D*				m_pCurAnim; // 맞구마잉, 성우쌤은 이걸 m_pCurClip으로 이름지으셨지.
	wstring						m_strDefaultAni; // 기본 애니메이션을 설정해준다. ONCE_RETURN 옵션에서 사용

	bool						m_bPause; // 애니메이션의 정지 여부이다.
										  // 이 값이 켜져 있는 경우에는 애니메이션에 대한 처리를 하지 않는다.

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
	// 편집된 이미지, 가로로 이어진 이미지를 LeftTop과 Length를 기준으로 한 프레임씩 따서, 애니메이션을 만들어주는
	// 함수이다. 

	void PlayAnimation(const wstring& strAnimKey); // 애니메이션 호출
	CAnimation2D* FindAnimation(const wstring& strKey); 
	void UpdateData(); // 애니메이션에서 사용할 데이터를 넘기는 함수로 UV좌표를 토대로 프레임의 정보를 넘기는데 사용된다.
	static void ClearData(); // 위에서 사용한 정보를 Clear하는데 이용.

	void StopAnimation(); // 초기상태 & stop

	virtual bool SaveToScene(FILE* pFile);
	virtual bool LoadFromScene(FILE* pFile);

};

