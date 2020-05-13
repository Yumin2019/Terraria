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

	// Game Mode������ ����ϴ� �Լ��̴�. 
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
		int iResult = MessageBox(nullptr, L"�ߺ�Ű ���", L"��ü�Ͻðڽ��ϱ� ?", MB_YESNOCANCEL);
		
		if (iResult == IDYES)
		{
			// ����� �߰�.
			DeletePrototype(strName);

			// �߰� �� �Ŵϱ� �̸� .. �޼���.
			MessageBox(nullptr, L"��ü�Ϸ�", L"", MB_OK);
		}
		else if (iResult == IDNO || iResult == IDCANCEL)
		{
			return;
		}
	}
	else if (!pObj->GetChildVec().empty())
	{
		// �ڽ��� ������ ����.
		assert(false);
		return;
	}
	else if (pObj->GetParent() != nullptr)
	{
		// �θ� ������ ����.
		assert(false);
		return;
	}

	// �ַ� Layer�� �Ҽӵ� ������Ʈ�� Prototype���� ���� ���̴�. �̷� ��쿡�� ���縦 �� ���� Prototype���� �־��ش�.
	// Stop ���¿��� �̷�����Ƿ� Awake�� �ʿ����.
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

	// �ش� ������Ʈ�� �����ϰ� Map���� ���ش�.
	m_mapPrototype.erase(strName);

	SAFE_DELETE(pFind);
}


bool CPrototypeMgr::LoadPrototype(FILE * pFile)
{
	// ������ ���� ������ (Scene�� �޶����� ��)
	Safe_Delete_Map(m_mapPrototype);

	// Prototype�� ������ �д´�.
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
	// Prototype�� ������ �����Ѵ�.
	int iSize = m_mapPrototype.size();
	fwrite(&iSize, 4, 1, pFile);
	
	for (const auto& pair : m_mapPrototype)
	{
		// ������ŭ ������.
		CSaveLoadMgr::SaveGameObject(pair.second, pFile);
	}

	return true;
}

bool CPrototypeMgr::HardCodingInit()
{
	// ���� & ���� �κ��� ������ ���ؼ� ����Ѵ�.
	
	return true;
}
