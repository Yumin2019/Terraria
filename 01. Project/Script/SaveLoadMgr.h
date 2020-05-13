#pragma once

class CLayer;
class CScene;
class CGameObject;


class CSaveLoadMgr
{
private:
	CSaveLoadMgr() {}
	virtual ~CSaveLoadMgr() {}



public:
	static bool SaveScene(const wstring& strFilePath);
	static bool LoadScene(const wstring& strFilePath);

	static bool SaveLayer(CLayer* pLayer, FILE* pFile);
	static bool LoadLayer(CLayer* pLayer, FILE* pFile);

	static bool SaveGameObject(CGameObject* pObject, FILE* pFile);
	static bool LoadGameObject(CGameObject* pObject, FILE* pFile);

	static bool SaveResource(FILE* pFile);
	static bool LoadResource(FILE* pFile);
};

