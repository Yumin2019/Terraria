#include "stdafx.h"
#include "PrototypeMgr.h"
#include "SaveLoadMgr.h"

#include <Engine/Core.h>
#include <Engine/GameObject.h>
#include <Engine/SceneMgr.h>
#include <Engine/Scene.h>
#include <Engine/Layer.h>
#include <Engine/Script.h>
#include <Engine/EventMgr.h>

DEFINITION_SINGLE(CPrototypeMgr);

CPrototypeMgr::CPrototypeMgr()
{
}


CPrototypeMgr::~CPrototypeMgr()
{
	Safe_Delete_Map(m_mapPrototype);
}

CGameObject * CPrototypeMgr::CreateCloneObj(const wstring & strName, const wstring & strLayerName)
{
	CGameObject* pFind = FindObj(strName);

	if(pFind == nullptr)
		return nullptr;

	pFind = pFind->Clone();

	// Game Mode에서만 사용하는 함수이다. 
	if (pFind->GetMeshRender())
		pFind->GetMeshRender()->GetCloneMaterial()->DisableFileSave();


	pFind->Awake();
	pFind->Start();

	// Layer
	CScript::CreateObject(pFind, strLayerName);

	return pFind;
}

CGameObject * CPrototypeMgr::FindObj(const wstring & strName)
{
	map<wstring, CGameObject*>::iterator iter = m_mapPrototype.find(strName);

	if(iter == m_mapPrototype.end())
		return nullptr;

	return iter->second;
}

void CPrototypeMgr::CreatePrototype(const wstring & strName, CGameObject * pObj)
{
		// nullptr : assert.
	if (pObj == nullptr)
	{
		assert(false);
		return;
	}
	else if (FindObj(strName) != nullptr)
	{
		// used Key
		int iResult = MessageBox(nullptr, L"중복키 사용", L"교체하시겠습니까 ?", MB_YESNOCANCEL);
		
		if (iResult == IDYES)
		{
			// 지우고 추가.
			DeletePrototype(strName);

			// 추가 될 거니까 미리 .. 메세지.
			MessageBox(nullptr, L"교체완료", L"", MB_OK);
		}
		else if (iResult == IDNO || iResult == IDCANCEL)
		{
			return;
		}
	}
	else if (!pObj->GetChildVec().empty())
	{
		// 자식을 가지고 있음.
		assert(false);
		return;
	}
	else if (pObj->GetParent() != nullptr)
	{
		// 부모를 가지고 있음.
		assert(false);
		return;
	}

	// 주로 Layer의 소속된 오브젝트를 Prototype으로 넣을 것이다. 이런 경우에는 복사를 한 것을 Prototype으로 넣어준다.
	// Stop 상태에서 이루어지므로 Awake는 필요없음.
	if (pObj->GetLayerIdx() != -1)
	{
		pObj = pObj->Clone();
	}
	
	pObj->SetName(strName);
	m_mapPrototype.insert(make_pair(strName, pObj));
}

void CPrototypeMgr::DeletePrototype(const wstring & strName)
{
	CGameObject* pFind = FindObj(strName);

	if (pFind == nullptr)
	{
		assert(false);
		return;
	}

	// 해당 오브젝트를 삭제하고 Map에서 없앤다.
	m_mapPrototype.erase(strName);

	SAFE_DELETE(pFind);
}


bool CPrototypeMgr::LoadPrototype(FILE * pFile)
{
	// 기존의 정보 날리기 (Scene이 달라지는 것)
	Safe_Delete_Map(m_mapPrototype);

	// Prototype의 개수를 읽는다.
	int iSize = 0;
	fread(&iSize, 4, 1, pFile);

	for (int i = 0; i < iSize; ++i)
	{
		CGameObject* pObj = new CGameObject;
		CSaveLoadMgr::LoadGameObject(pObj, pFile);

		m_mapPrototype.insert(make_pair(pObj->GetName(), pObj));
	}

	return true;
}

bool CPrototypeMgr::SavePrototype(FILE * pFile)
{
	// Prototype의 개수를 저장한다.
	int iSize = m_mapPrototype.size();
	fwrite(&iSize, 4, 1, pFile);
	
	for (const auto& pair : m_mapPrototype)
	{
		// 개수만큼 돌린다.
		CSaveLoadMgr::SaveGameObject(pair.second, pFile);
	}

	return true;
}

bool CPrototypeMgr::HardCodingInit()
{
	// 도구 & 무기 부분의 연결을 위해서 사용한다.
	
	return true;
}
