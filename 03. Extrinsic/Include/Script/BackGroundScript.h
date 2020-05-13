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

	// Layer�� ���ǵ�.
	Vec2		m_tSpeed;

	// ���ҽ����� ����� ����.
	Vec2		m_tResSrc;
	Vec2		m_tResDest;

	// ����� ������ ũ��
	Vec2		m_tLength;

	
	// ���ҽ����� ����ϰ� �ִ� ��ġ�� ��Ÿ���� ������ǥ. ���������� ������ǥ�� ���.
	Vec2		m_tSrc;	
	Vec2		m_tDest;

	// ����� ������ ũ�� (�� ��ŭ�� ��, ���� ���� ���̴�.)
	Vec2		m_tOutputLength;
	Vec2		m_tImageLength; 
	// UV��ǥ�� ��ü �̹����� ũ�⸦ �������� �ϱ� ������ �޾Ƶ־� �Ѵ�.

	float			m_fCameraGapY;


	// ������ �ѱ� ���� UV��ǥ�� �������� �Ѱܾ� �Ѵ�.

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
	void SetRelativePos(const Vec2& tSrc, const Vec2& tDest); // ��� ��ǥ�� �־��ָ�, �װ��� ���� �̹��������� ������ǥ�� ����.
	void SetRelativePos(float srcX, float srcY, float destX, float destY);
	void SetOutputLength(const Vec2& tLength) { m_tOutputLength = tLength; }
	void SetOutuptLength(float width, float height) { m_tOutputLength = Vec2(width, height); }
	void SetImageLength(const Vec2& tLength) { m_tImageLength = tLength; }
	void SetImageLength(float width, float height) { m_tImageLength = Vec2(width, height); }

	void SetInfo(const Vec2& tSpeed, const Vec2& tResSrc, const Vec2& tResDest, const Vec2& tSrcPos, /*Dest�� OutputInfo*/
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

