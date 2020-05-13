#pragma once
#include <Engine/Script.h>

/*
PanelScript는 ... 하나의 Texture에 해당하는 것을 여러번 출력하는 경우에 사용이 된다.
UI의 Panel을 출력하기 위함이다.

최대 50개만 지원한다. 10 * 5

Rendering 지원 스크립트이다.

*/

class CPanelScript :
	public CScript
{
public:
	CPanelScript();
	~CPanelScript();
	CLONE(CPanelScript);

private:
	PanelBuffer		m_tPanelArr[50];

	CGameObject*	m_pTarget;
	// Target을 기준으로 충돌한 인덱스를 반환하는데 이용된다. 
	// 이 기능을 사용하기 위해서는 Collider2D가 있어야 한다.
	// UI 한정. 움직이지 않는다는 가정.

	int				m_iCollIdx;
public:
	void SetPanel(int iIdx, int iData);
	void SetPanel(int iIdx, int iData, unsigned char r, unsigned char g, unsigned char b);

	int GetCollIdx() const { return m_iCollIdx; }
	void SetTarget(CGameObject* pTarget)
	{
		if (!pTarget || !GetCollider2D())
			assert(false);

		m_pTarget = pTarget;
	}


	Vec2 GetPanelSize();

public:
	virtual void Awake();
	virtual void Update();
	virtual void FinalUpdate() { UpdateData(); }
	void UpdateData();
};

