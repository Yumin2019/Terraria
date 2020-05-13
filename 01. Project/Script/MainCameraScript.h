#pragma once
#include <Engine/Script.h>
// 플레이어를 찾아서 따라다니는 역할을 한다.

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
	Vec2				m_vRatio;		// 화면을 기준으로 어느 위치에 둘것인지.

public:

	virtual void Awake();
	virtual void Update() {}
	virtual void LateUpdate();
};

