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
	wstring		m_strPath; // 자신의 경로를 가지고 있도록 하여, 리소스를 다시 불러오는 상황에서 이용한다.
	int			m_iRef; 
	RES_TYPE	m_eType;

	UINT		m_iCloneCount;

	// 모든 리소스 객체의 경우에 Resource의 상속을 받도록 한다.

	// 부모의 객체에 대한 변수를 private로 선언을 해두고, 이것을 자식에서 생성자를 호출하여 처음 한번만 초기화하고서,
	// 이후에는 이 값을 읽어올 수 있도록만 하는 것이다.

public:
	RES_TYPE GetType() const { return m_eType; }
	const wstring& GetPath() { return m_strPath; }


	int GetRef() const { return m_iRef; }
	UINT GetCloneCount() const { return m_iCloneCount; }
	void SetCloneCount(int iCount) { m_iCloneCount = iCount; }

public:
	void SetPath(const wstring& strPath) { m_strPath = strPath; }

public:
	virtual CResource* Clone() { ++m_iCloneCount;  return nullptr; } // 없으면 부모로.
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
	// 앞으로 만들어질 Ptr<T> 클래스에 대해서 friend 처리를 한다.
	// 즉, Ptr 클래스를 통해서 레퍼런스 카운트 값을 관리하겠다는 의미이다.

};

template<typename T>
inline bool CResource::LoadFromScene(FILE * pFile)
{
	wstring strName = LoadWString(pFile);

	m_strPath = LoadWString(pFile);

	// 리소스가 이미 로딩되어 있는지 확인.
	Ptr<T> pRes = CResMgr::GetInst()->FindRes<T>(strName);
	if (pRes != nullptr)
		return false; // 이미 있다.

	// 없는 경우 로딩하고 true
	SetName(strName);

	wstring strPath = CPathMgr::GetResPath();
	strPath += m_strPath;
	Load(strPath);

	return true;
}
