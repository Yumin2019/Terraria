#pragma once

#include <vector>
#include <string>

enum class SCRIPT_TYPE
{
	ARMORPANELSCRIPT,
	ARMORSCRIPT,
	BACKGROUNDSCRIPT,
	CRAFTINGSCRIPT,
	DAYSCRIPT,
	EQUIPMENTSCRIPT,
	FLYAWAYSCRIPT,
	FONTOBJSCRIPT,
	GRAVITYSCRIPT,
	HEARTUISCRIPT,
	INVENTORYSCRIPT,
	ITEMOBJSCRIPT,
	ITEMSCRIPT,
	JUMPSCRIPT,
	LOGOSCRIPT,
	MAINCAMERASCRIPT,
	MONSTERHPSCRIPT,
	MONSTERSCRIPT,
	MOUSESCRIPT,
	PANELSCRIPT,
	PARTICLESCRIPT,
	PLAYERSCRIPT,
	SCALESCRIPT,
	SELECTSCRIPT,
	SHININGSCRIPT,
	SLIMESCRIPT,
	TOGGLESCRIPT,
	UIITEMSCRIPT,
	ZOMBIESCRIPT,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
