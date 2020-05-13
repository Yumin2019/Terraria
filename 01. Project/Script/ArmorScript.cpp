#include "stdafx.h"
#include "ArmorScript.h"
#include "ItemScript.h"
#include "PlayerScript.h"
#include <Engine/KeyMgr.h>

CArmorScript::CArmorScript() :
	CDirectionScript((UINT)SCRIPT_TYPE::ARMORSCRIPT),
	m_pPlayer(nullptr),
	m_pItem(nullptr),
	m_eItem(ITEM::NONE),
	m_eType(ITEM_TYPE::NONE),
	m_iArmor(2)
{
}


CArmorScript::~CArmorScript()
{
}

void CArmorScript::Awake()
{

	const vector<CScript*>& vecScript = GetObj()->GetScriptVec();

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		if (vecScript[i]->GetScriptType() == (UINT)SCRIPT_TYPE::ITEMSCRIPT)
		{
			m_pItem = (CItemScript*)vecScript[i];
			break;
		}
	}

	if (m_pItem == nullptr)
		assert(false);

	vector<CGameObject*> vecFind;
	CSceneMgr::GetInst()->FindObjByTag(L"Player", vecFind);

	if (vecFind.empty())
		assert(false);

	m_pPlayer = (CPlayerScript*)vecFind[0]->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT);

	if (m_pPlayer == nullptr)
		assert(false);

	GetTransform()->SetPivot(0.5f, 0.5f);

	float fLight = 1.f;
	GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fLight);

	fLight = 0.8f;
	GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_1, &fLight);

}

void CArmorScript::Start()
{
	m_pItem->SetArmor(this);
	m_eType = m_pItem->GetType();
	m_eItem = m_pItem->GetItem();

	

	// 연결하고 일단 꺼둔다.
	SetEnable(false);

	switch (m_eType)
	{
	case ITEM_TYPE::HELMET:
		m_tOffset.x = 0.f;
		m_tOffset.y = 5.f;
		break;

	case ITEM_TYPE::ARMOR:

		m_tOffset.x = 1.f;
		m_tOffset.y = 5.f;
		break;

	case ITEM_TYPE::LEGGINGS:

		m_tOffset.x = 1.f;
		m_tOffset.y = 7.f;
		break;
	}
}

void CArmorScript::LateUpdate()
{
	CDirectionScript::LateUpdate();

	if (m_pPlayer != nullptr)
	{
		// 장착중인 경우에는 Player가 존재.
		Vec3 tLocalPos = m_pPlayer->GetTransform()->GetLocalPos();

		//if (KEYPRESS(KEY_TYPE::KEY_Z) && m_eType == ITEM_TYPE::HELMET)
		//{
		//	// Head
		//	if (KEYDOWN(KEY_TYPE::KEY_A))
		//	{
		//		m_tOffset.x -= 1.f;
		//	}

		//	if (KEYDOWN(KEY_TYPE::KEY_D))
		//	{
		//		m_tOffset.x += 1.f;
		//	}

		//	if (KEYDOWN(KEY_TYPE::KEY_W))
		//	{
		//		m_tOffset.y += 1.f;
		//	}

		//	if (KEYDOWN(KEY_TYPE::KEY_S))
		//	{
		//		m_tOffset.y -= 1.f;
		//	}

		//}
		//else if (KEYPRESS(KEY_TYPE::KEY_X) && m_eType == ITEM_TYPE::ARMOR)
		//{
		//	// Armor

		//	if (KEYDOWN(KEY_TYPE::KEY_A))
		//	{
		//		m_tOffset.x -= 1.f;
		//	}

		//	if (KEYDOWN(KEY_TYPE::KEY_D))
		//	{
		//		m_tOffset.x += 1.f;
		//	}

		//	if (KEYDOWN(KEY_TYPE::KEY_W))
		//	{
		//		m_tOffset.y += 1.f;
		//	}

		//	if (KEYDOWN(KEY_TYPE::KEY_S))
		//	{
		//		m_tOffset.y -= 1.f;
		//	}
		//}
		//else if (KEYPRESS(KEY_TYPE::KEY_C) && m_eType == ITEM_TYPE::LEGGINGS)
		//{
		//	// Leggings

		//	if (KEYDOWN(KEY_TYPE::KEY_A))
		//	{
		//		m_tOffset.x -= 1.f;
		//	}

		//	if (KEYDOWN(KEY_TYPE::KEY_D))
		//	{
		//		m_tOffset.x += 1.f;
		//	}

		//	if (KEYDOWN(KEY_TYPE::KEY_W))
		//	{
		//		m_tOffset.y += 1.f;
		//	}

		//	if (KEYDOWN(KEY_TYPE::KEY_S))
		//	{
		//		m_tOffset.y -= 1.f;
		//	}
		//}


		// 플레이어 오른쪽기준으로 셋팅. 방어구 왼쪽.
		// 고려할 점 : 플레이어 왼쪽 기준. 방어구 오른쪽. (이때 반대부호.)
		
		tLocalPos.x += m_tOffset.x * -GetDir();
		tLocalPos.y += m_tOffset.y;
		tLocalPos.z = GetTransform()->GetLocalPos().z;

		GetTransform()->SetLocalPos(tLocalPos);

	}
}

bool CArmorScript::SaveToScene(FILE * pFile)
{
	CDirectionScript::SaveToScene(pFile);

	return true;
}

bool CArmorScript::LoadFromScene(FILE * pFile)
{
	CDirectionScript::LoadFromScene(pFile);

	return true;
}

void CArmorScript::Enable(bool bEnable)
{
	SetEnable(bEnable);

	GetMeshRender()->SetEnable(bEnable);
}

void CArmorScript::PlayAnimation(const wstring & strAnim)
{
	if (GetAnimator2D() != nullptr)
	{
		GetAnimator2D()->PlayAnimation(strAnim);
	}
}
