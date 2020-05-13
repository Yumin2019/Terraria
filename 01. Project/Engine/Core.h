#pragma once

#include "SceneMgr.h"
#include "RenderMgr.h"
// 멤버 포인터를 선언하는 경우에는 그 멤버의 헤더파일을 선언해줘야 한다.


class CCore
{
	SINGLE(CCore);

private:
	HWND		m_hWnd;
	Resolution	m_tRes;
	SCENE_MODE	m_eMode;

	// mode를 구분하여 호출할 함수 
	void(CSceneMgr::*m_pUpdateFunc)(void);
	void(CRenderMgr::*m_pRenderFunc)(void);


public:
	bool Init(HWND hWnd, const Resolution& tResolution, bool bWindow);
	void Logic();
	void SetSceneMode(SCENE_MODE eMode);
	SCENE_MODE GetSceneMode() const { return m_eMode; }

private:

	
public:
	const Resolution& GetRes() const { return m_tRes; }
	void ChangeWindowSize(HWND hWnd);



};

