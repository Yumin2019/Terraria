#include "stdafx.h"
#include "ScriptMgr.h"

#include "ArmorPanelScript.h"
#include "ArmorScript.h"
#include "BackGroundScript.h"
#include "CraftingScript.h"
#include "DayScript.h"
#include "EquipmentScript.h"
#include "FlyawayScript.h"
#include "FontObjScript.h"
#include "GravityScript.h"
#include "HeartUIScript.h"
#include "InventoryScript.h"
#include "ItemObjScript.h"
#include "ItemScript.h"
#include "JumpScript.h"
#include "LogoScript.h"
#include "MainCameraScript.h"
#include "MonsterHPScript.h"
#include "MonsterScript.h"
#include "MouseScript.h"
#include "PanelScript.h"
#include "ParticleScript.h"
#include "PlayerScript.h"
#include "ScaleScript.h"
#include "SelectScript.h"
#include "ShiningScript.h"
#include "SlimeScript.h"
#include "ToggleScript.h"
#include "UIItemScript.h"
#include "ZombieScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"ArmorPanelScript");
	_vec.push_back(L"ArmorScript");
	_vec.push_back(L"BackGroundScript");
	_vec.push_back(L"CraftingScript");
	_vec.push_back(L"DayScript");
	_vec.push_back(L"EquipmentScript");
	_vec.push_back(L"FlyawayScript");
	_vec.push_back(L"FontObjScript");
	_vec.push_back(L"GravityScript");
	_vec.push_back(L"HeartUIScript");
	_vec.push_back(L"InventoryScript");
	_vec.push_back(L"ItemObjScript");
	_vec.push_back(L"ItemScript");
	_vec.push_back(L"JumpScript");
	_vec.push_back(L"LogoScript");
	_vec.push_back(L"MainCameraScript");
	_vec.push_back(L"MonsterHPScript");
	_vec.push_back(L"MonsterScript");
	_vec.push_back(L"MouseScript");
	_vec.push_back(L"PanelScript");
	_vec.push_back(L"ParticleScript");
	_vec.push_back(L"PlayerScript");
	_vec.push_back(L"ScaleScript");
	_vec.push_back(L"SelectScript");
	_vec.push_back(L"ShiningScript");
	_vec.push_back(L"SlimeScript");
	_vec.push_back(L"ToggleScript");
	_vec.push_back(L"UIItemScript");
	_vec.push_back(L"ZombieScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"ArmorPanelScript" == _strScriptName)
		return new CArmorPanelScript;
	if (L"ArmorScript" == _strScriptName)
		return new CArmorScript;
	if (L"BackGroundScript" == _strScriptName)
		return new CBackGroundScript;
	if (L"CraftingScript" == _strScriptName)
		return new CCraftingScript;
	if (L"DayScript" == _strScriptName)
		return new CDayScript;
	if (L"EquipmentScript" == _strScriptName)
		return new CEquipmentScript;
	if (L"FlyawayScript" == _strScriptName)
		return new CFlyawayScript;
	if (L"FontObjScript" == _strScriptName)
		return new CFontObjScript;
	if (L"GravityScript" == _strScriptName)
		return new CGravityScript;
	if (L"HeartUIScript" == _strScriptName)
		return new CHeartUIScript;
	if (L"InventoryScript" == _strScriptName)
		return new CInventoryScript;
	if (L"ItemObjScript" == _strScriptName)
		return new CItemObjScript;
	if (L"ItemScript" == _strScriptName)
		return new CItemScript;
	if (L"JumpScript" == _strScriptName)
		return new CJumpScript;
	if (L"LogoScript" == _strScriptName)
		return new CLogoScript;
	if (L"MainCameraScript" == _strScriptName)
		return new CMainCameraScript;
	if (L"MonsterHPScript" == _strScriptName)
		return new CMonsterHPScript;
	if (L"MonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"MouseScript" == _strScriptName)
		return new CMouseScript;
	if (L"PanelScript" == _strScriptName)
		return new CPanelScript;
	if (L"ParticleScript" == _strScriptName)
		return new CParticleScript;
	if (L"PlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"ScaleScript" == _strScriptName)
		return new CScaleScript;
	if (L"SelectScript" == _strScriptName)
		return new CSelectScript;
	if (L"ShiningScript" == _strScriptName)
		return new CShiningScript;
	if (L"SlimeScript" == _strScriptName)
		return new CSlimeScript;
	if (L"ToggleScript" == _strScriptName)
		return new CToggleScript;
	if (L"UIItemScript" == _strScriptName)
		return new CUIItemScript;
	if (L"ZombieScript" == _strScriptName)
		return new CZombieScript;
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::ARMORPANELSCRIPT:
		return L"ArmorPanelScript";
		break;

	case SCRIPT_TYPE::ARMORSCRIPT:
		return L"ArmorScript";
		break;

	case SCRIPT_TYPE::BACKGROUNDSCRIPT:
		return L"BackGroundScript";
		break;

	case SCRIPT_TYPE::CRAFTINGSCRIPT:
		return L"CraftingScript";
		break;

	case SCRIPT_TYPE::DAYSCRIPT:
		return L"DayScript";
		break;

	case SCRIPT_TYPE::EQUIPMENTSCRIPT:
		return L"EquipmentScript";
		break;

	case SCRIPT_TYPE::FLYAWAYSCRIPT:
		return L"FlyawayScript";
		break;

	case SCRIPT_TYPE::FONTOBJSCRIPT:
		return L"FontObjScript";
		break;

	case SCRIPT_TYPE::GRAVITYSCRIPT:
		return L"GravityScript";
		break;

	case SCRIPT_TYPE::HEARTUISCRIPT:
		return L"HeartUIScript";
		break;

	case SCRIPT_TYPE::INVENTORYSCRIPT:
		return L"InventoryScript";
		break;

	case SCRIPT_TYPE::ITEMOBJSCRIPT:
		return L"ItemObjScript";
		break;

	case SCRIPT_TYPE::ITEMSCRIPT:
		return L"ItemScript";
		break;

	case SCRIPT_TYPE::JUMPSCRIPT:
		return L"JumpScript";
		break;

	case SCRIPT_TYPE::LOGOSCRIPT:
		return L"LogoScript";
		break;

	case SCRIPT_TYPE::MAINCAMERASCRIPT:
		return L"MainCameraScript";
		break;

	case SCRIPT_TYPE::MONSTERHPSCRIPT:
		return L"MonsterHPScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"MonsterScript";
		break;

	case SCRIPT_TYPE::MOUSESCRIPT:
		return L"MouseScript";
		break;

	case SCRIPT_TYPE::PANELSCRIPT:
		return L"PanelScript";
		break;

	case SCRIPT_TYPE::PARTICLESCRIPT:
		return L"ParticleScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"PlayerScript";
		break;

	case SCRIPT_TYPE::SCALESCRIPT:
		return L"ScaleScript";
		break;

	case SCRIPT_TYPE::SELECTSCRIPT:
		return L"SelectScript";
		break;

	case SCRIPT_TYPE::SHININGSCRIPT:
		return L"ShiningScript";
		break;

	case SCRIPT_TYPE::SLIMESCRIPT:
		return L"SlimeScript";
		break;

	case SCRIPT_TYPE::TOGGLESCRIPT:
		return L"ToggleScript";
		break;

	case SCRIPT_TYPE::UIITEMSCRIPT:
		return L"UIItemScript";
		break;

	case SCRIPT_TYPE::ZOMBIESCRIPT:
		return L"ZombieScript";
		break;

	}
	return nullptr;
}