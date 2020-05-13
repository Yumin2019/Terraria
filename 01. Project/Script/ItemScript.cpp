#include "stdafx.h"
#include "ItemScript.h"
#include "UIItemScript.h"

#include <Engine/Scene.h>
#include <Engine/SceneMgr.h>
#include <Engine/Layer.h>

CItemScript::CItemScript() :
	m_pUIObj(nullptr),
	m_pUIItem(nullptr),
	m_pArmor(nullptr),
	m_eType(ITEM_TYPE::NONE),
	m_eItem(ITEM::NONE),
	m_fSwayTime(1.f),
	m_fCoefficient(1.f),
	CDirectionScript((UINT)SCRIPT_TYPE::ITEMSCRIPT)
{
}


CItemScript::~CItemScript()
{
}

void CItemScript::Awake()
{
	if(GetObj()->GetName() == L"CopperAxe" ||
		GetObj()->GetName() == L"CopperPickaxe" ||
		GetObj()->GetName() == L"CopperSword")
	{

		CScene* pScene = CSceneMgr::GetInst()->GetScene();
		CLayer* pLayer = pScene->FindLayer(L"UIItem");

		if (pLayer == nullptr)
			assert(false);

		// UI + 자신의 이름 이름이 설정되어 있다.
		// 해당 오브젝트(UI Obj)에 접근하여, 자신의 UIObj로 설정하고.
		// 해당 UIObj에서는 이것을 알고 있게 한다.

		// 기본 아이템에 대한 연결이다.
		wstring strName = GetObj()->GetName() + L"UI";

		const vector<CGameObject*>& vecObj = pLayer->GetParentObjVec();

		for (size_t i = 0; i < vecObj.size(); ++i)
		{
			if (vecObj[i]->GetName() == strName)
			{
				CUIItemScript* pScript = (CUIItemScript*)vecObj[i]->FindScript((UINT)SCRIPT_TYPE::UIITEMSCRIPT);

				m_pUIItem = pScript;
				m_pUIObj = vecObj[i];

				// 찾아왔는데, UI Item 물체에 Script가 없네... ? 
				if (pScript)
				{
					pScript->SetItem(this);
				}

				break;
			}
		}
	}

	wstring str = GetObj()->GetName();

	for (UINT i = 0; i < (UINT)ITEM::CRAFTING_END; ++i)
	{
		if (str == ITEM_NAME[i])
		{
			m_eType = GetItemType((ITEM)i);
			m_eItem = (ITEM)i;
			break;
		}
	}

	if (m_eType == ITEM_TYPE::NONE)
		assert(false);


	float fLight = 1.f;
	GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fLight);



	if (m_eType == ITEM_TYPE::WALL)
	{
		switch (m_eItem)
		{
		case ITEM::WOOD_WALL:
		case ITEM::STONE_WALL:
			fLight = 0.6f;
			break;

		case ITEM::DIRT_WALL:
			fLight = 0.35f;
			break;

		case ITEM::CACTUS_WALL:
			fLight = 0.4f;

			break;

		default:
			assert(false);
			break;
		}
	}
	else
		fLight = 0.8f;

	GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_1, &fLight);
}

void CItemScript::LateUpdate()
{
	CDirectionScript::LateUpdate();

	if (m_bSway)
	{
		m_fSecond += DT;
		m_fAngle += m_fCoefficient * DT * XM_PI * GetDir();


		if (m_fSecond >= m_fSwayTime)
		{
			StopSway();
		}

		GetTransform()->SetLocalRot(0.f, 0.f, m_fAngle);

	}
}

void CItemScript::StopSway()
{
	m_bSway = false;
	GetObj()->GetMeshRender()->SetEnable(false);

	if (GetObj()->GetCollider2D())
		GetObj()->GetCollider2D()->SetEnable(false);

	switch (m_eType)
	{
	case ITEM_TYPE::LEGGINGS:
	case ITEM_TYPE::ARMOR:
	case ITEM_TYPE::HELMET:
		break;

	case ITEM_TYPE::AXE:
	case ITEM_TYPE::WEAPON:
		//DisableComponent(GetObj()->GetMeshRender()); // 보이지 않도록 처리.

		break;
	}
}

void CItemScript::SetUIObj(CGameObject * pUIObj)
{
	m_pUIObj = pUIObj;

	m_pUIItem = (CUIItemScript*)pUIObj->FindScript((UINT)SCRIPT_TYPE::UIITEMSCRIPT);

	if (m_pUIItem == nullptr || m_pUIObj == nullptr)
		assert(false);
}

void CItemScript::SwayItem(float fSwayTime)
{

	// 시간이 들어왔다고 하자. 
	// 1s에 파이 만큼의 회전이 이루어진다고 해보자.

	m_bSway = true;
	m_fSecond = 0.f;
	m_fAngle = XM_PI / 4.f * -GetDir();
	GetTransform()->SetLocalRot(0.f, 0.f, m_fAngle);
	m_fSwayTime = fSwayTime;
	m_fCoefficient = 1.f / fSwayTime;

	// 방향에 따라서, Pivot이 변경되어야 한다.
	if (GetDir() == -1)
		GetTransform()->SetPivot(0.f, 0.f);
	else
		GetTransform()->SetPivot(1.f, 0.f);
	// Tool : 4프레임

}

bool CItemScript::LoadFromScene(FILE * pFile)
{
	CDirectionScript::LoadFromScene(pFile);

	return true;
}

bool CItemScript::SaveToScene(FILE * pFile)
{
	CDirectionScript::SaveToScene(pFile);

	return true;
}

