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
	// PanelScript�� ���� ����� �������� �������� �����Ǿ� �־�� �Ѵ�. 
	if (GetTransform()->GetPivot() != Vec2(0.f, 1.f))
		assert(false);
}

void CPanelScript::Update()
{
	// Target�� �ִٸ�, �̰Ͱ� �浹ó���� �ϰ�, �ε����� ��ȯ���ִ� ó���� �Ѵ�.
	if (m_pTarget != nullptr)
	{
		if (CCollisionMgr::GetInst()->IsCollision(m_pTarget->GetCollider2D(), GetCollider2D()))
		{
			// Panel�� ��� ���δ� ����ϴ� Script�� ���� �޶��� �κ��̴�. �ƹ�ư �츮��....
			// �������� �ִ� �κп� ���� �浹ó���� �����Ѵ�.
			// ��� ���� ��ġ�� �������� �浹 ó���� �����Ѵ�.

			Vec2 tPanelSize = Vec2(GetTransform()->GetLocalScale()) / Vec2(10.f, 5.f);
			Vec2 tStartPos = Vec2(GetTransform()->GetLocalPos());

			Vec2 tRelVec = Vec2(m_pTarget->GetTransform()->GetLocalPos()) - tStartPos;  // Panel�� �������� �� ����� ����.

			int iX = tRelVec.x / tPanelSize.x; 
			int iY = -tRelVec.y / tPanelSize.y; 

			int iIdx = iY * 10 + iX;

			if (iIdx < 0 || iIdx >= 50)
			{
				// �ε����� �߸� ���� ���.
				m_iCollIdx = -1;
				return;
			}

			// Show ���θ� �Ǵ��Ѵ�.
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
