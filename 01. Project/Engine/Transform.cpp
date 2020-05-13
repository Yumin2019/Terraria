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
	// CollisionMgr������ PrePos�� ���� ��ġ�� �����ϱ� ������ �ʴ´�.
	// ����, Update (Script�� ���� �̵� ����) ������ ���� ��ǥ�� ���� �������� ��ǥ�� �ǹ��Ѵ�.
	// �̰��� �޾Ƶ���.
	m_tPrevPos = GetLocalPos();
}

void CTransform::FinalUpdate()
{
	/*
		0, 0 �� ��쿡�� �츮�� �����ϴ� ��ǥ���� 0.5 x sizeX + PosX, 0.5 x sizeY + PosY�� ó�� �Ѵ�.

		0 0 �� ��쿡�� 0.5 0.5..
		1 1�� ��쿡�� -0.5 -0.5
		0.8 0.8�� ��쿡�� -0.3 -0.3


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

		// ���� ��� * (ũ�� * ȸ�� * �̵�)(�������)
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


	// �θ� ���� �߰�
	if (GetObj()->GetParent())
		m_matWorld *= GetObj()->GetParent()->GetTransform()->GetWorldMatrix();

	// �ڽ��� WorldMatrix�� ���ϰ�, �θ��� WorldMatrix�� ���ϴ� ó���� ���ش�.
	// �� �����, �θ�� �谡 �ǰ�,

}

void CTransform::Awake()
{
	// 0 1 �̸�... 0.5 - 0.5
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
	// TransformMatrix��� �̸��� ���� ��� ���� �����͸� �ѹ� �޾ƿ� ���Ŀ�..
	pCB->SetData(&g_tTransform);
	// ������� (Transform)�� ��쿡�� �̸� �־��� size�� ���ؼ� �ѱ�
	// �������� ũ�⸦ �˰� �ִ�.

	// ���� ���� g_tTransform (�ѱ���� �ϴ� �����͸� ��Ƶ� ���� ����) ��
	// Map , Unmap �Լ��� ȣ���Ͽ�, �޸𸮸� �������ִ� ������ ��ģ��.
	pCB->UpdateData((UINT)SHADER_TYPE::ST_VERTEX | (UINT)SHADER_TYPE::ST_PIXEL);
	// ��� ���۸� ���� Shader�Լ��� �����Ѵ�. Vertex | Pixel
}

bool CTransform::IsCasting(const Vec3 & tPos)
{
	Vec3 tWorldPos = GetWorldPos();
	Vec3 tWorldScale = GetWorldScale();

	// l t r b
	// x y z w
	Vec4 tRect = Vec4(tWorldPos.x - tWorldScale.x / 2.f, tWorldPos.y + tWorldScale.y / 2.f,
		tWorldPos.x + tWorldScale.x / 2.f, tWorldPos.y - tWorldScale.y / 2.f);

	// Rect �ȿ� Point�� ����ִٸ� �浹.
	if (tRect.x <= tPos.x && tRect.z >= tPos.x &&
		tRect.y >= tPos.y && tRect.w <= tPos.y)
		return true;

	/*
	����� �� ��
	�� ������ ���� ������
	Ŀ�´����̼� �ɷ�
	���ӿ� ���� ���

	������ ���ƾ� �� ��
	�������� �µ�
	�Ҽ����� �µ�
	�ڽ��� �߸��� �������� �ʴ� �µ�.
	*/
	return false;
}
