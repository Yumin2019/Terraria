#pragma once

class CGameObject;

class CPrototypeMgr
{
	DECLARE_SINGLE(CPrototypeMgr);

private:
	map<wstring, CGameObject*> m_mapPrototype;

public:
	const map<wstring, CGameObject*>& GetPrototypes() const { return m_mapPrototype; }

	CGameObject* CreateCloneObj(const wstring& strName, const wstring& strLayerName); // 복사후 Layer까지 넣어준다.
	CGameObject* FindObj(const wstring& strName);
	void CreatePrototype(const wstring& strName, CGameObject* pObj);
	void DeletePrototype(const wstring& strName);

	bool LoadPrototype(FILE* pFile);
	bool SavePrototype(FILE* pFile);
	void clear() { m_mapPrototype.clear(); }

	bool HardCodingInit();

};

