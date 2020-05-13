#include "stdafx.h"
#include "UIItemScript.h"
#include "ScaleScript.h"


CUIItemScript::CUIItemScript() :
	CScript((UINT)SCRIPT_TYPE::UIITEMSCRIPT),
	m_pItem(nullptr),
	m_pScale(nullptr)
{
}


CUIItemScript::~CUIItemScript()
{
}


void CUIItemScript::Awake()
{
	const vector<CScript*>& vecScript = GetObj()->GetScriptVec();

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		if (vecScript[i]->GetScriptType() == (UINT)SCRIPT_TYPE::SCALESCRIPT)
		{
			m_pScale = (CScaleScript*)vecScript[i];
			break;
		}
	}

	if (m_pScale == nullptr)
		assert(false);


	wstring str = GetObj()->GetName();
	str = str.substr(0, str.length() - 2);

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

	float fLight = 0.f;
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

void CUIItemScript::Start()
{
	wstring strSub = GetObj()->GetName();
	strSub = strSub.substr(0, strSub.size() - 2);

	GetObj()->SetName(strSub);
}
