// InfoView.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "InfoView.h"

#include "LayerDlg.h"
#include "LayerCheckDlg.h"

// Dlg
#include "TransformDlg.h"
#include "MeshRenderDlg.h"
#include "Collider2DDlg.h"
#include "CameraDlg.h"
#include "Light2DDlg.h"
#include "Animator2DDlg.h"
#include "ScriptDlg.h"

#include <Engine/TileMap2D.h> // 아직 Dlg가 없음..

#include <Engine/GameObject.h>
#include <Engine/SceneMgr.h>
#include <Engine/KeyMgr.h>
#include <Engine/EventMgr.h>


// CInfoView

IMPLEMENT_DYNCREATE(CInfoView, CView)

CInfoView::CInfoView() :
	m_pDlgArr{},
	m_pTarget(nullptr),
	m_pInspector(nullptr)
{

}

CInfoView::~CInfoView()
{
	
}

BEGIN_MESSAGE_MAP(CInfoView, CView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CInfoView 그리기

void CInfoView::SetTarget(CGameObject * pTarget)
{
	// Tool에서 지원하는 Dlg에 대해서 Target을 지정해준다.

	m_pTarget = pTarget;

	m_pInspector->SetTarget(pTarget);
	
	for (size_t i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		// Engine에서 지원하는 것이라면 모두 Target을 지정.
		if (m_pDlgArr[i])
			m_pDlgArr[i]->SetTarget(pTarget);
	}

	m_pScript->SetTarget(pTarget);
	
	
}

bool CInfoView::Init()
{
	// 자신이 가지고 있는 객체의 초기화를 진행한다.
	for (size_t i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_pDlgArr[(UINT)i])
			m_pDlgArr[(UINT)i]->Init();
	}

	if (m_pInspector)
		m_pInspector->Init();

	return true;
}


void CInfoView::Update()
{
	if (m_pTarget)
	{

		CRect InfoViewRC, DlgRC;
		GetClientRect(InfoViewRC);

		m_pInspector->Update();
		m_pInspector->GetWindowRect(DlgRC);
		m_pInspector->SetWindowPos(nullptr, 0, 0, InfoViewRC.Width(), DlgRC.Height(), 0);
		m_pInspector->ShowWindow(true);

		size_t iHeight = DlgRC.Height();


		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (m_pTarget->GetComponent((COMPONENT_TYPE)i) == nullptr)
			{
				// 오브젝트가 Component를 가지고 있지 않다.

				if (m_pDlgArr[i] != nullptr)
				{
					// Tool에서 해당 Component를 지원한다.
					m_pDlgArr[i]->EndDialog();
				}
				continue;
			}

			// 오브젝트는 Component를 가지고 있으나 Tool에서 지원을 안 하면 그냥 넘긴다.
			if (m_pDlgArr[i] == nullptr)
				continue;

			// 가지고 있고 처리할 수 있다.
			m_pDlgArr[i]->ShowWindow(true);
			m_pDlgArr[i]->GetWindowRect(DlgRC);
			m_pDlgArr[i]->SetWindowPos(nullptr, 0, iHeight - 1, InfoViewRC.Width(), DlgRC.Height(), 0);

			iHeight += DlgRC.Height() -1;

			m_pDlgArr[i]->Update();

		}

		// Script
		
			m_pScript->ShowWindow(true);
			m_pScript->GetWindowRect(DlgRC);
			m_pScript->SetWindowPos(nullptr, 0, iHeight - 1, InfoViewRC.Width(), DlgRC.Height(), 0);

			iHeight += DlgRC.Height() - 1;

			m_pScript->Update();
		
	}
	else
	{
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (m_pDlgArr[i] != nullptr)
				m_pDlgArr[i]->EndDialog();
		}

		m_pInspector->ShowWindow(false);
	}


}


void CInfoView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CInfoView 진단

#ifdef _DEBUG
void CInfoView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CInfoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CInfoView 메시지 처리기


int CInfoView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// WM_CREATE 메세지 발생시 호출.

	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TransformDlg를 생성한다.
	m_pDlgArr[(UINT)COMPONENT_TYPE::TRANSFORM] = new CTransformDlg;
	m_pDlgArr[(UINT)COMPONENT_TYPE::TRANSFORM]->Create(IDD_TRANSFORM, this); // 부모를 InfoView로 IDD_TRANSFORM 의 형식으로 생성.
	m_pDlgArr[(UINT)COMPONENT_TYPE::TRANSFORM]->ShowWindow(TRUE);
	// 객체 생성 != 윈도우 생성 ( Create호출시 생성 ) 윈도우를 보이기 위해서는
	// ShowWindow함수 호출.
	
	CRect rt, rtDlg;
	GetClientRect(rt);
	// InfoView의 Rect

	m_pDlgArr[(UINT)COMPONENT_TYPE::TRANSFORM]->GetClientRect(rtDlg); // TransformDlg의 Rect
	m_pDlgArr[(UINT)COMPONENT_TYPE::TRANSFORM]->SetWindowPos(nullptr, 0, 0, rt.Width(), rtDlg.Height(), 0);
	// InfoView의 0 0 에서 시작하여, InfoView의 Width를 가져가고, 자신의 Height를 토대로 만들어준다.

	// Inspector
	m_pInspector = new CLayerDlg;
	m_pInspector->Create(IDD_LAYER, this);
	m_pInspector->ShowWindow(TRUE);
	m_pInspector->GetClientRect(rtDlg);
	m_pInspector->SetWindowPos(nullptr, 0, 0, rt.Width(), rtDlg.Height(), 0);

	



	// MeshRender Dlg
	m_pDlgArr[(UINT)COMPONENT_TYPE::MESHRENDER] = new CMeshRenderDlg;
	m_pDlgArr[(UINT)COMPONENT_TYPE::MESHRENDER]->Create(IDD_MESHRENDER, this);
	m_pDlgArr[(UINT)COMPONENT_TYPE::MESHRENDER]->ShowWindow(TRUE);
	m_pDlgArr[(UINT)COMPONENT_TYPE::MESHRENDER]->GetClientRect(rtDlg);
	m_pDlgArr[(UINT)COMPONENT_TYPE::MESHRENDER]->SetWindowPos(nullptr, 0, 0, rt.Width(), rtDlg.Height(), 0);




	// Collider2D Dlg
	m_pDlgArr[(UINT)COMPONENT_TYPE::COLLIDER2D] = new CCollider2DDlg;
	m_pDlgArr[(UINT)COMPONENT_TYPE::COLLIDER2D]->Create(IDD_COLLIDER2D, this);
	m_pDlgArr[(UINT)COMPONENT_TYPE::COLLIDER2D]->ShowWindow(TRUE);
	m_pDlgArr[(UINT)COMPONENT_TYPE::COLLIDER2D]->GetClientRect(rtDlg);
	m_pDlgArr[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetWindowPos(nullptr, 0, 0, rt.Width(), rtDlg.Height(), 0);


	// Camera Dlg
	m_pDlgArr[(UINT)COMPONENT_TYPE::CAMERA] = new CCameraDlg;
	m_pDlgArr[(UINT)COMPONENT_TYPE::CAMERA]->Create(IDD_CAMERA, this);
	m_pDlgArr[(UINT)COMPONENT_TYPE::CAMERA]->ShowWindow(TRUE);
	m_pDlgArr[(UINT)COMPONENT_TYPE::CAMERA]->GetClientRect(rtDlg);
	m_pDlgArr[(UINT)COMPONENT_TYPE::CAMERA]->SetWindowPos(nullptr, 0, 0, rt.Width(), rtDlg.Height(), 0);



	// Light2D Dlg
	m_pDlgArr[(UINT)COMPONENT_TYPE::LIGHT2D] = new CLight2DDlg;
	m_pDlgArr[(UINT)COMPONENT_TYPE::LIGHT2D]->Create(IDD_LIGHT2D, this);

	m_pDlgArr[(UINT)COMPONENT_TYPE::LIGHT2D]->ShowWindow(TRUE);
	m_pDlgArr[(UINT)COMPONENT_TYPE::LIGHT2D]->GetClientRect(rtDlg);
	m_pDlgArr[(UINT)COMPONENT_TYPE::LIGHT2D]->SetWindowPos(nullptr, 0, 0, rt.Width(), rtDlg.Height(), 0);


	// ANIMATOR2D
	m_pDlgArr[(UINT)COMPONENT_TYPE::ANIMATOR2D] = new CAnimator2DDlg;
	m_pDlgArr[(UINT)COMPONENT_TYPE::ANIMATOR2D]->Create(IDD_ANIMATOR2D, this);
	m_pDlgArr[(UINT)COMPONENT_TYPE::ANIMATOR2D]->ShowWindow(TRUE);
	m_pDlgArr[(UINT)COMPONENT_TYPE::ANIMATOR2D]->GetClientRect(rtDlg);
	m_pDlgArr[(UINT)COMPONENT_TYPE::ANIMATOR2D]->SetWindowPos(nullptr, 0, 0, rt.Width(), rtDlg.Height(), 0);


	// SCRIPT
	m_pScript = new CScriptDlg;
	m_pScript->Create(IDD_SCRIPTDLG, this);
	m_pScript->ShowWindow(FALSE);
	m_pScript->GetClientRect(rtDlg);
	m_pScript->SetWindowPos(nullptr, 0, 0, rt.Width(), rtDlg.Height(), 0);

	return 0;
}

// Component Add
#include <Engine/Animator2D.h>
#include <Engine/Camera.h>
#include <Engine/Collider2D.h>
#include <Engine/Light2D.h>
#include <Engine/MeshRender.h>

void CInfoView::AddAnimator2D()
{
	// 타겟이 없거나, 이미 Component를 가진 경우 return.
	if(m_pTarget == nullptr || m_pTarget->GetAnimator2D())
		return;

	m_pTarget->AddComponent(new CAnimator2D);
}


void CInfoView::AddCamera()
{
	if (m_pTarget == nullptr || m_pTarget->GetCamera())
		return;

	m_pTarget->AddComponent(new CCamera);

}


void CInfoView::AddCollider2D()
{
	if (m_pTarget == nullptr || m_pTarget->GetCollider2D())
		return;

	m_pTarget->AddComponent(new CCollider2D);
}


void CInfoView::AddLight2D()
{
	if (m_pTarget == nullptr || m_pTarget->GetLight2D())
		return;

	m_pTarget->AddComponent(new CLight2D);

}

void CInfoView::AddTileMap2D()
{
	if (m_pTarget == nullptr || m_pTarget->GetTileMap2D())
		return;

	m_pTarget->AddComponent(new CTileMap2D);

}

void CInfoView::AddMeshRender()
{
	if (m_pTarget == nullptr || m_pTarget->GetMeshRender())
		return;

	m_pTarget->AddComponent(new CMeshRender);

}

void CInfoView::DeleteCamera()
{

	if (m_pTarget == nullptr || !m_pTarget->GetCamera())
		return;

	m_pTarget->DeleteComponent(COMPONENT_TYPE::CAMERA);
}

void CInfoView::DeleteAnimator2D()
{
	if (m_pTarget == nullptr || !m_pTarget->GetAnimator2D())
		return;

	m_pTarget->DeleteComponent(COMPONENT_TYPE::ANIMATOR2D);
}

void CInfoView::DeleteCollider2D()
{
	if (m_pTarget == nullptr || !m_pTarget->GetCollider2D())
		return;

	m_pTarget->DeleteComponent(COMPONENT_TYPE::COLLIDER2D);
}

void CInfoView::DeleteMeshRender()
{
	if (m_pTarget == nullptr || !m_pTarget->GetMeshRender())
		return;

	m_pTarget->DeleteComponent(COMPONENT_TYPE::MESHRENDER);
}

void CInfoView::DeleteLight2D()
{
	if (m_pTarget == nullptr || !m_pTarget->GetLight2D())
		return;

	m_pTarget->DeleteComponent(COMPONENT_TYPE::LIGHT2D);
}

void CInfoView::DeleteTileMap2D()
{
	if (m_pTarget == nullptr || !m_pTarget->GetTileMap2D())
		return;

	m_pTarget->DeleteComponent(COMPONENT_TYPE::TILEMAP2D);
}

