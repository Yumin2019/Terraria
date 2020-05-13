#include "stdafx.h"
#include "PanelScript.h"
#include <Engine/ConstBuffer.h>
#include <Engine/Device.h>
#include <Engine/CollisionMgr.h>


CPanelScript::CPanelScript() :
	CScript((UINT)SCRIPT_TYPE::PANELSCRIPT),
	m_tPanelArr{},
	m_pTarget(nullptr),
	m_iCollIdx(-1)
{
	for (size_t i = 0; i < 50; ++i)
	{
		m_tPanelArr[i].iUse = 1;
	}
}


CPanelScript::~CPanelScript()
{
}

void CPanelScript::SetPanel(int iIdx, int iData)
{
	if (iIdx < 0 || iIdx >= 50)
		assert(false);

	m_tPanelArr[iIdx].iUse = iData;
}

void CPanelScript::SetPanel(int iIdx, int iData, unsigned char r, unsigned char g, unsigned char b)
{
	SetPanel(iIdx, iData);
	m_tPanelArr[iIdx].fRValue = r / 255.f;
	m_tPanelArr[iIdx].fGValue = g / 255.f;
	m_tPanelArr[iIdx].fBValue = b / 255.f;
}

Vec2 CPanelScript::GetPanelSize()
{
	return GetTransform()->GetLocalScale() / Vec2(10.f, 5.f);
}

void CPanelScript::Awake()
{
	// PanelScript는 왼쪽 상단을 기준으로 오프셋이 지정되어 있어야 한다. 
	if (GetTransform()->GetPivot() != Vec2(0.f, 1.f))
		assert(false);
}

void CPanelScript::Update()
{
	// Target이 있다면, 이것과 충돌처리를 하고, 인덱스를 반환해주는 처리를 한다.
	if (m_pTarget != nullptr)
	{
		if (CCollisionMgr::GetInst()->IsCollision(m_pTarget->GetCollider2D(), GetCollider2D()))
		{
			// Panel의 출력 여부는 사용하는 Script에 따라서 달라질 부분이다. 아무튼 우리는....
			// 보여지고 있는 부분에 대한 충돌처리를 진행한다.
			// 가운데 점의 위치를 기준으로 충돌 처리를 진행한다.

			Vec2 tPanelSize = Vec2(GetTransform()->GetLocalScale()) / Vec2(10.f, 5.f);
			Vec2 tStartPos = Vec2(GetTransform()->GetLocalPos());

			Vec2 tRelVec = Vec2(m_pTarget->GetTransform()->GetLocalPos()) - tStartPos;  // Panel을 기준으로 한 상대적 벡터.

			int iX = tRelVec.x / tPanelSize.x; 
			int iY = -tRelVec.y / tPanelSize.y; 

			int iIdx = iY * 10 + iX;

			if (iIdx < 0 || iIdx >= 50)
			{
				// 인덱스가 잘못 나온 경우.
				m_iCollIdx = -1;
				return;
			}

			// Show 여부를 판단한다.
			if (m_tPanelArr[iIdx].iUse)
				m_iCollIdx = iIdx;
			else
				m_iCollIdx = -1;
		}
		else
			m_iCollIdx = -1;
	}
}

void CPanelScript::UpdateData()
{
	static CConstBuffer* pPanelBuffer = CDevice::GetInst()->FindConstBuffer(L"PanelBuffer");

	pPanelBuffer->SetData(m_tPanelArr, sizeof(PanelBuffer) * 50);
	pPanelBuffer->UpdateData((UINT)SHADER_TYPE::ST_VERTEX | (UINT)SHADER_TYPE::ST_PIXEL);
}
