#pragma once
#include <Engine/Script.h>

/*
PanelScript�� ... �ϳ��� Texture�� �ش��ϴ� ���� ������ ����ϴ� ��쿡 ����� �ȴ�.
UI�� Panel�� ����ϱ� �����̴�.

�ִ� 50���� �����Ѵ�. 10 * 5

Rendering ���� ��ũ��Ʈ�̴�.

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
	// Target�� �������� �浹�� �ε����� ��ȯ�ϴµ� �̿�ȴ�. 
	// �� ����� ����ϱ� ���ؼ��� Collider2D�� �־�� �Ѵ�.
	// UI ����. �������� �ʴ´ٴ� ����.

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

