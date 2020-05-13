#pragma once
#include <Engine/Script.h>


class CBackGroundScript :
	public CScript
{
public:
	CBackGroundScript();
	virtual ~CBackGroundScript();
	CLONE(CBackGroundScript);

private:

	class CPlayerScript* m_pPlayer;

	// Layer의 스피드.
	Vec2		m_tSpeed;

	// 리소스에서 사용할 영역.
	Vec2		m_tResSrc;
	Vec2		m_tResDest;

	// 사용할 영역의 크기
	Vec2		m_tLength;

	
	// 리소스에서 출력하고 있는 위치를 나타내는 절대좌표. 내부적으로 절대좌표로 계산.
	Vec2		m_tSrc;	
	Vec2		m_tDest;

	// 출력할 영역의 크기 (이 만큼을 뜯어서, 끼워 맞출 것이다.)
	Vec2		m_tOutputLength;
	Vec2		m_tImageLength; 
	// UV좌표는 전체 이미지의 크기를 기준으로 하기 때문에 받아둬야 한다.

	float			m_fCameraGapY;


	// 정보를 넘길 때는 UV좌표를 기준으로 넘겨야 한다.

public:
	virtual bool SaveToScene(FILE* pFile);
	virtual bool LoadFromScene(FILE* pFile);

public:
	void SetCameraGap(float fGap) { m_fCameraGapY = fGap; }
	void SetSpeed(const Vec2& tSpeed) { m_tSpeed = tSpeed; }
	void SetResPos(const Vec2& tSrc, const Vec2& tDest) { m_tResSrc = tSrc; m_tResDest = tDest; m_tLength = m_tResDest - m_tResSrc; }
	void SetResPos(float srcX, float srcY, float destX, float destY) { m_tResSrc = Vec2(srcX, srcY); m_tResDest = Vec2(destX, destY); m_tLength = m_tResDest - m_tResSrc; }
	void SetLength(const Vec2& tLength) { m_tLength = tLength; }
	void SetLength(float width, float height) { m_tLength = Vec2(width, height); }
	void SetRelativePos(const Vec2& tSrc, const Vec2& tDest); // 상대 좌표를 넣어주면, 그것을 토대로 이미지에서의 절대좌표를 셋팅.
	void SetRelativePos(float srcX, float srcY, float destX, float destY);
	void SetOutputLength(const Vec2& tLength) { m_tOutputLength = tLength; }
	void SetOutuptLength(float width, float height) { m_tOutputLength = Vec2(width, height); }
	void SetImageLength(const Vec2& tLength) { m_tImageLength = tLength; }
	void SetImageLength(float width, float height) { m_tImageLength = Vec2(width, height); }

	void SetInfo(const Vec2& tSpeed, const Vec2& tResSrc, const Vec2& tResDest, const Vec2& tSrcPos, /*Dest는 OutputInfo*/
		const Vec2& tOutputLength, const Vec2& tImageLength, CGameObject* pCamera);

public:
	const Vec2& GetSpeed() const { return m_tSpeed; }
	const Vec2& GetResSrc() const { return m_tResSrc; }
	const Vec2& GetResDest() const { return m_tResDest; }
	const Vec2& GetLength() const { return m_tLength; }


public:
	virtual void Awake();
	virtual void Update();
	virtual void LateUpdate();

	virtual void UpdateData();

};

