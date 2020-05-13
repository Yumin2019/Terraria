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
	// Resource ���� ���� map�� ������ �ֵ��� �Ѵ�.

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
	// �ش� Sound��, Channel�� � ������ �Ͽ� ����Ѵ�. Pause���·� �������� ����

	// ��� Channel Stop & Volume Set.
public:
	bool Init();

	template <typename T>
	void AddRes(const wstring& strKey, Ptr<T> pRes);

	template <typename T>
	void AddCloneRes(Ptr<T> pOriginRes);
	// ����Ʈ �����͸� �־��ָ� �̰��� Type�� ���� �Ͽ� ���纻�� �����Ͽ� ���Ϳ� �־�ΰ�, �̰��� ��ȯ���ִ� �Լ��̴�.
	
	template <typename T>
	Ptr<T> FindRes(const wstring& strKey);

	// FindRes�Լ��� AddRes�Լ��� T*�� ����ߴ� ���� ����Ʈ �����͸� �̿��ϴ� ���·� �ٲ۴�.

	template <typename T>
	Ptr<T> Load(const wstring& strKey, const wstring& strPath /* ��� ��� */);

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

	Ptr<T> pFind = FindRes<T>(strKey); // Resource* �� ���ͼ�, �̰��� ��������ڷ� �Է¹���.
	// �־��� Ÿ�Կ� ���� ���ҽ� map�� ã�Ƽ�, �ش� map���� strKey���� �ش��ϴ�
	// ���ҽ��� ã�ƿ´�.
	
	if (pFind != nullptr)
	{
		// �̹� �ش� Ű�� ���� ���ҽ��� �����ϴ� ��� ������� �ʴ´�.
		assert(nullptr);
	}

	// ���ҽ� map���� strKey���� Ű�� �ϴ� ���ҽ��� ���� ��� �������ش�.

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
	
	// ������ ĳ������ �ϱ� ���ؼ�, Resource**�� pRes�� �ּҸ� �־��ش�.
	// pRes�� ���ҽ��� ����Ű�� ��ü�̰�, (Resource*��� �����ϸ� �ȴ�)
	// �̰��� �ּҷ� Resource**�� �ް�,
	// �̰��� �������Ͽ� Res*�� ����(��ü�� �ּҸ� ������ Res�� �ּҰ� ���´�) �Ѵ�.
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
	// ���� Type�� ���ؼ�, �־�� �� vector�� ����.

	// ������ ĳ������ �ؼ�, �ش� vector�� �־��ش�.
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

	// typeinfo ����ü�� ���۷��� ���·� �޾ƿͼ� ������ �� �� ������ �ϰ�, 
	// static Ű����� �ѹ��� ���� �޾ƿ����� �Ѵ�.
	// �� Ŭ������ ������ ������ ������ �����ϱ�.

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

	// strKey�� ���ؼ� �־��� ���ҽ��� �����Ѵٸ� �װ��� �Ѱ��ְ�,
	// ���ٸ� Load�� �Ѵ�.

	if (pRes != nullptr)
		return pRes;

	// ���ҽ��� ���� ���.

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

	// ���ҽ��� Ÿ���� T �� ���� �˾ƿ� ������, �ش� Ÿ���� ���ҽ� map�� �� ���ҽ��� �־��ش�.
	pRes = new T;
	wstring strFullPath = CPathMgr::GetResPath();
	strFullPath += strPath;

	// T Type�� ���� Load�Լ��� ���� ���̴�.
	pRes->Load(strFullPath);

	CResource** ppResource = (CResource**)&pRes;
	m_mapRes[(UINT)eType].insert(make_pair(strKey, *ppResource));
	pRes->SetName(strKey);
	pRes->SetPath(strPath); // ��� ��� ����.

	return pRes;
}
