#pragma once
#include <Engine/Script.h>
// �÷��̾ ã�Ƽ� ����ٴϴ� ������ �Ѵ�.

class CMainCameraScript :
	public CScript
{
public:
	CMainCameraScript();
	virtual ~CMainCameraScript();
	CLONE(CMainCameraScript);

public:
	static Vec2				g_vMapStart;	// TileMap LT StartPos
	static Vec2				g_vMapEnd;		// TileMap RB End Pos
	static Vec3				g_vMainCamPos;

public:
	class CPlayerScript* m_pPlayer;
	Vec2				m_vRes;
	Vec2				m_vRatio;		// ȭ���� �������� ��� ��ġ�� �Ѱ�����.

public:

	virtual void Awake();
	virtual void Update() {}
	virtual void LateUpdate();
};

