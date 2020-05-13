#pragma once

#include "SceneMgr.h"
#include "RenderMgr.h"
// ��� �����͸� �����ϴ� ��쿡�� �� ����� ��������� ��������� �Ѵ�.


class CCore
{
	SINGLE(CCore);

private:
	HWND		m_hWnd;
	Resolution	m_tRes;
	SCENE_MODE	m_eMode;

	// mode�� �����Ͽ� ȣ���� �Լ� 
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

