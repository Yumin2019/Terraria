#include "stdafx.h"
#include "Collider2D.h"
#include "Script.h"

#include "ResMgr.h"

#include "Transform.h"
#include "Device.h"
#include "ConstBuffer.h"


UINT CCollider2D::g_iColID = 0;

CCollider2D::CCollider2D() :
	CComponent(COMPONENT_TYPE::COLLIDER2D),
	m_pColMesh(nullptr),
	m_pColMtrl(nullptr),
	m_iColID(g_iColID++),
	m_iCollisionCount(0),
	m_eType(COLLIDER2D_TYPE::END)
{
	m_pColMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Collider2DMtrl_Green");
}

CCollider2D::CCollider2D(const CCollider2D & col) :
	CComponent(COMPONENT_TYPE::COLLIDER2D),
	m_tOffset(col.m_tOffset),
	m_tScale(col.m_tScale),
	m_eType(col.m_eType),
	m_iCollisionCount(0),
	m_pColMesh(col.m_pColMesh),
	m_pColMtrl(col.m_pColMtrl),
	m_matColWorld(col.m_matColWorld),
	m_iColID(g_iColID++),
	m_bRender(false)
{
	
	
}

/* 새로운 생성이 일어나는 경우에는 당연히 ID값을 만들어서 줘야지. 
근데, 이미 만들어진 충돌체에 데입을 받는 경우에는 ... 자신의 ID값을 그대로 유지시켜야지. (나머지 정보는 따라가고)
*/


CCollider2D::~CCollider2D()
{
	/*
	42억의 수라면, 충분히 사용하고도 남으므로 반환처리는 하지 않는다.
	*/
}

void CCollider2D::operator=(const CCollider2D & col) 
{

	
	// ID값의 처리를 하기 위해서 데입 연산자를 재정의 한다.
	// 데입이 일어나는 경우는 해당 CCollider2D의 정보를 그대로 가져와서 사용하는 경우이다.
	// 따라서, 충돌체의 고유한 값을 제외하고는 나머지를 복사한다.
	UINT iColID = col.m_iColID;
	//*this = col;
	m_iColID = iColID;

}



void CCollider2D::Awake()
{
	/*Vec3 tScale = GetTransform()->GetWorldScale() * m_tScale;
	
	Vec2 tOffsetPercent = Vec2(0.5f, 0.5f) - GetTransform()->GetPivot();
	m_tPivotTrans = (tOffsetPercent * Vec2(tScale.x, tScale.y));*/
}

void CCollider2D::FinalUpdate()
{
	// Transform의 Offset에 따라서, 기준으로 생각하는 위치가 달라져야 하는 것 아닌가 ? 
	Vec3 tFinalPos = m_tOffset;/* + m_tPivotTrans;*/
	tFinalPos = tFinalPos / GetTransform()->GetWorldScale(); // World Scale 변경

	Matrix matTranslation = XMMatrixTranslation(tFinalPos.x, tFinalPos.y, tFinalPos.z - 1.f);
	Matrix matScale = XMMatrixScaling(m_tScale.x, m_tScale.y, m_tScale.z);

	m_matColWorld = matScale * matTranslation;
	m_matColWorld *= GetTransform()->GetWorldMatrix();

	// Transform이 WorldMtrix를 구해낸 상태에서 이후에 Collider2D의 Matrix를 구한다.

}

bool CCollider2D::SaveToScene(FILE * pFile)
{
	UINT iType = (UINT)GetComponentType();
	fwrite(&iType, 4, 1, pFile);

	fwrite(&m_tOffset, sizeof(Vec3), 1, pFile);
	fwrite(&m_tScale, sizeof(Vec3), 1, pFile);
	fwrite(&m_eType, sizeof(COLLIDER2D_TYPE), 1, pFile);

	return true;
}

bool CCollider2D::LoadFromScene(FILE * pFile)
{
	fread(&m_tOffset, sizeof(Vec3), 1, pFile);
	fread(&m_tScale, sizeof(Vec3), 1, pFile);
	fread(&m_eType, sizeof(COLLIDER2D_TYPE), 1, pFile);
	SetCollider2DType(m_eType);
	
	return true;
}

void CCollider2D::Render()
{
	if (m_bRender)
	{
		static CConstBuffer* pCB = CDevice::GetInst()->FindConstBuffer(L"TransformMatrix");

		g_tTransform.matWorld = m_matColWorld;
		pCB->SetData(&g_tTransform);
		pCB->UpdateData((UINT)SHADER_TYPE::ST_VERTEX | (UINT)SHADER_TYPE::ST_PIXEL);

		m_pColMtrl->UpdateData();

		if (m_pColMesh != nullptr)
			m_pColMesh->Render();
	}

}

void CCollider2D::OnCollisionEnter(CCollider2D * pOther)
{
 	++m_iCollisionCount;
	
	if(m_iCollisionCount == 1)
		m_pColMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Collider2DMtrl_Red");

	// 충돌한 물체의 Script에서 OnCollisionEnter함수를 호출해준다.
	const vector<CScript*> vecScript = GetObj()->GetScriptVec();

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnCollisionEnter(pOther);
	}
}

void CCollider2D::OnCollision(CCollider2D * pOther)
{
	const vector<CScript*> vecScript = GetObj()->GetScriptVec();

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnCollision(pOther);
	}
}

void CCollider2D::OnCollisionExit(CCollider2D * pOther)
{
	--m_iCollisionCount;

	if (m_iCollisionCount == 0)
	{
		m_pColMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Collider2DMtrl_Green");
	}

	const vector<CScript*> vecScript = GetObj()->GetScriptVec();

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnCollisionExit(pOther);
	}
}

void CCollider2D::SetCollider2DType(COLLIDER2D_TYPE eType)
{
	// Mesh를 연결해주는 작업을 한다.

	m_eType = eType;
	Changed();

	switch (eType)
	{
	case COLLIDER2D_TYPE::RECT:
		m_pColMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ColRectMesh");

		break;

	case COLLIDER2D_TYPE::CIRCLE:
		m_pColMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ColCircleMesh");

		break;
	}
}
