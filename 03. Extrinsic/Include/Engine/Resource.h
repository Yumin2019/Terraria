#pragma once
#include "Entity.h"
class CResource :
	public CEntity
{
public:
	CResource(RES_TYPE eType);
	CResource(const CResource& res);
	virtual ~CResource();

private:
	wstring		m_strPath; // �ڽ��� ��θ� ������ �ֵ��� �Ͽ�, ���ҽ��� �ٽ� �ҷ����� ��Ȳ���� �̿��Ѵ�.
	int			m_iRef; 
	RES_TYPE	m_eType;

	UINT		m_iCloneCount;

	// ��� ���ҽ� ��ü�� ��쿡 Resource�� ����� �޵��� �Ѵ�.

	// �θ��� ��ü�� ���� ������ private�� ������ �صΰ�, �̰��� �ڽĿ��� �����ڸ� ȣ���Ͽ� ó�� �ѹ��� �ʱ�ȭ�ϰ�,
	// ���Ŀ��� �� ���� �о�� �� �ֵ��ϸ� �ϴ� ���̴�.

public:
	RES_TYPE GetType() const { return m_eType; }
	const wstring& GetPath() { return m_strPath; }


	int GetRef() const { return m_iRef; }
	UINT GetCloneCount() const { return m_iCloneCount; }
	void SetCloneCount(int iCount) { m_iCloneCount = iCount; }

public:
	void SetPath(const wstring& strPath) { m_strPath = strPath; }

public:
	virtual CResource* Clone() { ++m_iCloneCount;  return nullptr; } // ������ �θ��.
	void SaveToScene(FILE* pFile);

	template <typename T>
	bool LoadFromScene(FILE* pFile);

	virtual void Load(const wstring& strFullPath) {};
	virtual void Save(const wstring& strPath) {}

private:
	void AddRef() { ++m_iRef; }
	void Release() { --m_iRef; }

	template <typename T>
	friend class Ptr;
	// ������ ������� Ptr<T> Ŭ������ ���ؼ� friend ó���� �Ѵ�.
	// ��, Ptr Ŭ������ ���ؼ� ���۷��� ī��Ʈ ���� �����ϰڴٴ� �ǹ��̴�.

};

template<typename T>
inline bool CResource::LoadFromScene(FILE * pFile)
{
	wstring strName = LoadWString(pFile);

	m_strPath = LoadWString(pFile);

	// ���ҽ��� �̹� �ε��Ǿ� �ִ��� Ȯ��.
	Ptr<T> pRes = CResMgr::GetInst()->FindRes<T>(strName);
	if (pRes != nullptr)
		return false; // �̹� �ִ�.

	// ���� ��� �ε��ϰ� true
	SetName(strName);

	wstring strPath = CPathMgr::GetResPath();
	strPath += m_strPath;
	Load(strPath);

	return true;
}
