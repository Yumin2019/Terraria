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

		// UI + �ڽ��� �̸� �̸��� �����Ǿ� �ִ�.
		// �ش� ������Ʈ(UI Obj)�� �����Ͽ�, �ڽ��� UIObj�� �����ϰ�.
		// �ش� UIObj������ �̰��� �˰� �ְ� �Ѵ�.

		// �⺻ �����ۿ� ���� �����̴�.
		wstring strName = GetObj()->GetName() + L"UI";

		const vector<CGameObject*>& vecObj = pLayer->GetParentObjVec();

		for (size_t i = 0; i < vecObj.size(); ++i)
		{
			if (vecObj[i]->GetName() == strName)
			{
				CUIItemScript* pScript = (CUIItemScript*)vecObj[i]->FindScript((UINT)SCRIPT_TYPE::UIITEMSCRIPT);

				m_pUIItem = pScript;
				m_pUIObj = vecObj[i];

				// ã�ƿԴµ�, UI Item ��ü�� Script�� ����... ? 
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
		//DisableComponent(GetObj()->GetMeshRender()); // ������ �ʵ��� ó��.

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

	// �ð��� ���Դٰ� ����. 
	// 1s�� ���� ��ŭ�� ȸ���� �̷�����ٰ� �غ���.

	m_bSway = true;
	m_fSecond = 0.f;
	m_fAngle = XM_PI / 4.f * -GetDir();
	GetTransform()->SetLocalRot(0.f, 0.f, m_fAngle);
	m_fSwayTime = fSwayTime;
	m_fCoefficient = 1.f / fSwayTime;

	// ���⿡ ����, Pivot�� ����Ǿ�� �Ѵ�.
	if (GetDir() == -1)
		GetTransform()->SetPivot(0.f, 0.f);
	else
		GetTransform()->SetPivot(1.f, 0.f);
	// Tool : 4������

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

