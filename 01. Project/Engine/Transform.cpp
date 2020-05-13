#include "stdafx.h"
#include "Transform.h"
#include "ConstBuffer.h"
#include "Device.h"
#include "Script.h"

Transform g_tTransform;


CTransform::CTransform() :
	CComponent(COMPONENT_TYPE::TRANSFORM),
	m_tPivot(0.5f, 0.5f),
	m_bRotOffset(false),
	m_bMoveX(false),
	m_bMoveY(false),
	m_fDirX(0.f),
	m_fDirY(0.f),
	m_bTileColl(false)
{
}


CTransform::~CTransform()
{
}


void CTransform::Update()
{
	// CollisionMgr에서는 PrePos와 현재 위치가 동일하길 원하지 않는다.
	// 따라서, Update (Script를 통한 이동 전에) 들어오는 현재 좌표란 이전 프레임의 좌표를 의미한다.
	// 이것을 받아두자.
	m_tPrevPos = GetLocalPos();
}

void CTransform::FinalUpdate()
{
	/*
		0, 0 의 경우에는 우리가 생각하는 좌표에서 0.5 x sizeX + PosX, 0.5 x sizeY + PosY로 처리 한다.

		0 0 의 경우에는 0.5 0.5..
		1 1의 경우에는 -0.5 -0.5
		0.8 0.8의 경우에는 -0.3 -0.3


		0.5 - offset

	*/


	if (!m_bRotOffset)
	{
		Vec2 tLocalPos = Vec2(m_tLocalPos.x, m_tLocalPos.y) + m_tPivotTrans;

		Matrix matTranslation = XMMatrixTranslation(tLocalPos.x, tLocalPos.y, m_tLocalPos.z);
		Matrix matScale = XMMatrixScaling(m_tLocalScale.x, m_tLocalScale.y, m_tLocalScale.z);

		Matrix matRot = XMMatrixRotationX(m_tLocalRot.x);
		matRot *= XMMatrixRotationY(m_tLocalRot.y);
		matRot *= XMMatrixRotationZ(m_tLocalRot.z);

		// 로컬 행렬 * (크기 * 회전 * 이동)(월드행렬)
		m_matWorld = matScale * matRot * matTranslation;
	}
	else
	{
		Matrix matTrans0 = XMMatrixTranslation(m_tPivotTrans.x, m_tPivotTrans.y, 0.f);
		Matrix matScale = XMMatrixScaling(m_tLocalScale.x, m_tLocalScale.y, m_tLocalScale.z);

		Vec2 tLocalPos = Vec2(m_tLocalPos.x, m_tLocalPos.y) + m_tPivotTrans;

		Matrix matTrans1 = XMMatrixTranslation(tLocalPos.x, tLocalPos.y, m_tLocalPos.z);

		Matrix matRot = XMMatrixRotationX(m_tLocalRot.x);
		matRot *= XMMatrixRotationY(m_tLocalRot.y);
		matRot *= XMMatrixRotationZ(m_tLocalRot.z);

		m_matWorld = matScale * matTrans0 * matRot * matTrans1;
	}


	// 부모 관계 추가
	if (GetObj()->GetParent())
		m_matWorld *= GetObj()->GetParent()->GetTransform()->GetWorldMatrix();

	// 자신의 WorldMatrix를 구하고, 부모의 WorldMatrix를 곱하는 처리를 해준다.
	// 이 결과로, 부모는 배가 되고,

}

void CTransform::Awake()
{
	// 0 1 이면... 0.5 - 0.5
	// 0 0		   0.5   0.5
	// 1 0		   -0.5 0.5
	// 1 1         -0.5 -0.5
	Vec2 tOffsetPercent = Vec2(0.5f, 0.5f) - m_tPivot;
	m_tPivotTrans = (tOffsetPercent * Vec2(m_tLocalScale.x, m_tLocalScale.y));
}

bool CTransform::SaveToScene(FILE * pFile)
{

	UINT iType = (UINT)GetComponentType();
	fwrite(&iType, 4, 1, pFile);

	fwrite(&m_tLocalPos, sizeof(Vec3), 1, pFile);
	fwrite(&m_tLocalScale, sizeof(Vec3), 1, pFile);
	fwrite(&m_tLocalRot, sizeof(Vec3), 1, pFile);

	fwrite(&m_tPivot, sizeof(Vec2), 1, pFile);
	fwrite(&m_tPivotTrans, sizeof(Vec2), 1, pFile);
	fwrite(&m_bRotOffset, 1, 1, pFile);


	return true;
}

bool CTransform::LoadFromScene(FILE * pFile)
{
	fread(&m_tLocalPos, sizeof(Vec3), 1, pFile);
	fread(&m_tLocalScale, sizeof(Vec3), 1, pFile);
	fread(&m_tLocalRot, sizeof(Vec3), 1, pFile);

	fread(&m_tPivot, sizeof(Vec2), 1, pFile);
	fread(&m_tPivotTrans, sizeof(Vec2), 1, pFile);
	fread(&m_bRotOffset, 1, 1, pFile);


	return true;
}

void CTransform::TileCollision()
{
	const vector<CScript*>& vecScript = GetObj()->GetScriptVec();

	auto iter = vecScript.begin();
	auto iterEnd = vecScript.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->TileCollision();
	}

	FinalUpdate();

	if (GetCollider2D() != nullptr)
	{
		GetCollider2D()->FinalUpdate();
	}
}


Vec3 CTransform::GetWorldPos() const
{
	return m_matWorld.Translation();
}

Vec3 CTransform::GetWorldScale() const
{
	CGameObject* pParent = GetObj()->GetParent();
	Vec3 tWorldScale = m_tLocalScale;

	while (pParent)
	{
		tWorldScale *= pParent->GetTransform()->GetLocalScale();

		pParent = pParent->GetParent();
	}

	return tWorldScale;
}

void CTransform::UpdateData()
{
	g_tTransform.matWorld = m_matWorld;
	g_tTransform.matWV = g_tTransform.matWorld * g_tTransform.matView;
	g_tTransform.matWVP = g_tTransform.matWV * g_tTransform.matProj;

	static CConstBuffer* pCB = GET_SINGLE(CDevice)->FindConstBuffer(L"TransformMatrix");
	// TransformMatrix라는 이름을 가진 상수 버퍼 포인터를 한번 받아온 이후에..
	pCB->SetData(&g_tTransform);
	// 상수버퍼 (Transform)의 경우에는 미리 넣어준 size를 통해서 넘길
	// 데이터의 크기를 알고 있다.

	// 전역 변수 g_tTransform (넘기려고 하는 데이터를 담아둔 전역 변수) 를
	// Map , Unmap 함수를 호출하여, 메모리를 복사해주는 과정을 거친다.
	pCB->UpdateData((UINT)SHADER_TYPE::ST_VERTEX | (UINT)SHADER_TYPE::ST_PIXEL);
	// 상수 버퍼를 보낼 Shader함수를 지정한다. Vertex | Pixel
}

bool CTransform::IsCasting(const Vec3 & tPos)
{
	Vec3 tWorldPos = GetWorldPos();
	Vec3 tWorldScale = GetWorldScale();

	// l t r b
	// x y z w
	Vec4 tRect = Vec4(tWorldPos.x - tWorldScale.x / 2.f, tWorldPos.y + tWorldScale.y / 2.f,
		tWorldPos.x + tWorldScale.x / 2.f, tWorldPos.y - tWorldScale.y / 2.f);

	// Rect 안에 Point가 들어있다면 충돌.
	if (tRect.x <= tPos.x && tRect.z >= tPos.x &&
		tRect.y >= tPos.y && tRect.w <= tPos.y)
		return true;

	/*
	갖춰야 할 것
	각 직무에 대한 전문성
	커뮤니케이션 능력
	게임에 대한 흥미

	갖추지 말아야 할 것
	부정적인 태도
	불성실한 태도
	자신의 잘못을 인정하지 않는 태도.
	*/
	return false;
}
