#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Ptr.h"
#include "PathMgr.h"
#include "Sound.h"


class CResMgr
{
	DECLARE_SINGLE(CResMgr);

private:
	map<wstring, CResource*>	m_mapRes[(UINT)RES_TYPE::END];
	// Resource 별로 각각 map을 가지고 있도록 한다.

	vector<wstring> m_vecDefMtrl;

	vector<CResource*> m_vecCloneRes[(UINT)RES_TYPE::END];


	// Sound System
private:
	System*					m_pSoundSys;
	Channel*				m_arrChannel[(UINT)CHANNEL_TYPE::END];

private:
	void CreateSound();
	void CreateDefaultMesh();
	void CreateDefaultShader();
	void CreateDefaultMaterial();

public:
	// Sound 

	void SoundUpdate();
	void SetVolume(CHANNEL_TYPE eType, float fVolume);
	bool IsPlaying(CHANNEL_TYPE eType) const;
	void Stop(CHANNEL_TYPE eType);
	void Play(const wstring & strName, CHANNEL_TYPE eType, float fVomue = 1.f); 
	// 해당 Sound를, Channel은 어떤 것으로 하여 출력한다. Pause상태로 시작할지 여부

	// 모든 Channel Stop & Volume Set.
public:
	bool Init();

	template <typename T>
	void AddRes(const wstring& strKey, Ptr<T> pRes);

	template <typename T>
	void AddCloneRes(Ptr<T> pOriginRes);
	// 스마트 포인터를 넣어주면 이것의 Type을 토대로 하여 복사본을 생성하여 벡터에 넣어두고, 이것을 반환해주는 함수이다.
	
	template <typename T>
	Ptr<T> FindRes(const wstring& strKey);

	// FindRes함수와 AddRes함수를 T*를 사용했던 것을 스마트 포인터를 이용하는 형태로 바꾼다.

	template <typename T>
	Ptr<T> Load(const wstring& strKey, const wstring& strPath /* 상대 경로 */);

	System* GetSoundSys() const { return m_pSoundSys; }
	const map<wstring, CResource*>& GetResMap(RES_TYPE eType) const { return m_mapRes[(UINT)eType]; }

	// Def Mtrl
	const vector<wstring>& GetDefMtrl() const { return m_vecDefMtrl; }
};

template<typename T>
inline void CResMgr::AddRes(const wstring & strKey, Ptr<T> pRes)
{
	if (pRes == nullptr)
		assert(nullptr);

	static const type_info& mesh = typeid(CMesh);
	static const type_info& shader = typeid(CShader);
	static const type_info& material = typeid(CMaterial);
	static const type_info& texture = typeid(CTexture);
	static const type_info& sound = typeid(CSound);

	static RES_TYPE eType = RES_TYPE::END;

	Ptr<T> pFind = FindRes<T>(strKey); // Resource* 로 나와서, 이것을 복사생성자로 입력받음.
	// 넣어준 타입에 따른 리소스 map을 찾아서, 해당 map에서 strKey값에 해당하는
	// 리소스를 찾아온다.
	
	if (pFind != nullptr)
	{
		// 이미 해당 키를 가진 리소스가 존재하는 경우 허용하지 않는다.
		assert(nullptr);
	}

	// 리소스 map에서 strKey값을 키로 하는 리소스가 없는 경우 생성해준다.

	size_t iHashCode = typeid(T).hash_code();

	if (mesh.hash_code() == iHashCode)
		eType = RES_TYPE::MESH;

	else if (shader.hash_code() == iHashCode)
		eType = RES_TYPE::SHADER;

	else if (material.hash_code() == iHashCode)
		eType = RES_TYPE::MATERIAL;

	else if (texture.hash_code() == iHashCode)
		eType = RES_TYPE::TEXTURE;

	else if (sound.hash_code() == iHashCode)
		eType = RES_TYPE::SOUND;
	
	// 강제로 캐스팅을 하기 위해서, Resource**에 pRes의 주소를 넣어준다.
	// pRes는 리소스를 가리키는 객체이고, (Resource*라고 생각하면 된다)
	// 이것의 주소로 Resource**를 받고서,
	// 이것을 역참조하여 Res*에 접근(객체의 주소를 가보면 Res의 주소가 나온다) 한다.
	CResource** ppResource = (CResource**)&pRes;
	m_mapRes[(UINT)eType].insert(make_pair(strKey, *ppResource));
	pRes->SetName(strKey);
}

template<typename T>
void CResMgr::AddCloneRes(Ptr<T> pCloneRes)
{
	static const type_info& mesh = typeid(CMesh);
	static const type_info& shader = typeid(CShader);
	static const type_info& material = typeid(CMaterial);
	static const type_info& texture = typeid(CTexture);
	static const type_info& sound = typeid(CSound);

	static RES_TYPE eType = RES_TYPE::END;

	
	size_t iHashCode = typeid(T).hash_code();

	if (mesh.hash_code() == iHashCode)
		eType = RES_TYPE::MESH;

	else if (shader.hash_code() == iHashCode)
		eType = RES_TYPE::SHADER;

	else if (material.hash_code() == iHashCode)
		eType = RES_TYPE::MATERIAL;

	else if (texture.hash_code() == iHashCode)
		eType = RES_TYPE::TEXTURE;

	else if (sound.hash_code() == iHashCode)
		eType = RES_TYPE::SOUND;
	// 들어온 Type을 통해서, 넣어야 할 vector를 고른다.

	// 강제로 캐스팅을 해서, 해당 vector에 넣어준다.
	CResource** ppRes = (CResource**)&pCloneRes;
	m_vecCloneRes[(UINT)eType].push_back(*ppRes);
}

template<typename T>
inline Ptr<T> CResMgr::FindRes(const wstring & strKey)
{
	static const type_info& mesh = typeid(CMesh);
	static const type_info& shader = typeid(CShader);
	static const type_info& material = typeid(CMaterial);
	static const type_info& texture = typeid(CTexture);
	static const type_info& sound = typeid(CSound);

	// typeinfo 구조체를 레퍼런스 형태로 받아와서 수정을 할 수 없도록 하고, 
	// static 키워드로 한번만 값을 받아오도록 한다.
	// 각 클래스의 고유한 정보는 변하지 않으니까.

	static map<wstring, CResource*>::iterator iter;
	static RES_TYPE eType = RES_TYPE::END;

	size_t iHashCode = typeid(T).hash_code();
	
	if (mesh.hash_code() == iHashCode)
		eType = RES_TYPE::MESH;

	else if (shader.hash_code() == iHashCode)
		eType = RES_TYPE::SHADER;

	else if (material.hash_code() == iHashCode)
		eType = RES_TYPE::MATERIAL;

	else if (texture.hash_code() == iHashCode)
		eType = RES_TYPE::TEXTURE;

	else if (sound.hash_code() == iHashCode)
		eType = RES_TYPE::SOUND;

	iter = m_mapRes[(UINT)eType].find(strKey);

	if (iter == m_mapRes[(UINT)eType].end())
		return nullptr;

	return (T*)iter->second;
}

template<typename T>
inline Ptr<T> CResMgr::Load(const wstring & strKey, const wstring & strPath)
{
	static const type_info& mesh = typeid(CMesh);
	static const type_info& shader = typeid(CShader);
	static const type_info& material = typeid(CMaterial);
	static const type_info& texture = typeid(CTexture);
	static const type_info& sound = typeid(CSound);

	static RES_TYPE eType = RES_TYPE::END;

	Ptr<T> pRes = FindRes<T>(strKey);

	// strKey를 통해서 넣어준 리소스가 존재한다면 그것을 넘겨주고,
	// 없다면 Load를 한다.

	if (pRes != nullptr)
		return pRes;

	// 리소스가 없는 경우.

	size_t iHashCode = typeid(T).hash_code();

	if (mesh.hash_code() == iHashCode)
		eType = RES_TYPE::MESH;

	else if (shader.hash_code() == iHashCode)
		eType = RES_TYPE::SHADER;

	else if (material.hash_code() == iHashCode)
		eType = RES_TYPE::MATERIAL;

	else if (texture.hash_code() == iHashCode)
		eType = RES_TYPE::TEXTURE;

	else if (sound.hash_code() == iHashCode)
		eType = RES_TYPE::SOUND;

	// 리소스의 타입을 T 를 통해 알아온 다음에, 해당 타입의 리소스 map에 이 리소스를 넣어준다.
	pRes = new T;
	wstring strFullPath = CPathMgr::GetResPath();
	strFullPath += strPath;

	// T Type에 따른 Load함수가 있을 것이다.
	pRes->Load(strFullPath);

	CResource** ppResource = (CResource**)&pRes;
	m_mapRes[(UINT)eType].insert(make_pair(strKey, *ppResource));
	pRes->SetName(strKey);
	pRes->SetPath(strPath); // 상대 경로 저장.

	return pRes;
}
