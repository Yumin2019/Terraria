#include "stdafx.h"
#include "SaveLoadMgr.h"


#include <Engine/SceneMgr.h>
#include <Engine/Scene.h>
#include <Engine/Layer.h>
#include <Engine/GameObject.h>
#include <Engine/Component.h>
#include <Engine/Script.h>

#include "ScriptMgr.h"
#include "PrototypeMgr.h"

#include <Engine/CollisionMgr.h>
#include <Engine/ResMgr.h>
#include <Engine/Resource.h>
#include <Engine/SceneMgr.h>

#include <Engine/Transform.h>
#include <Engine/MeshRender.h>
#include <Engine/Camera.h>
#include <Engine/Collider2D.h>
#include <Engine/Animator2D.h>
#include <Engine/Light2D.h>
#include <Engine/TileMap2D.h>


bool CSaveLoadMgr::SaveScene(const wstring & strFilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	// 리소스
	SaveResource(pFile);

	// Collision Layer Check 상태 저장.
	fwrite(CCollisionMgr::GetInst()->GetLayerCheck(), sizeof(UINT) * MAX_LAYER, 1, pFile);

	// Scene Prototype 저장.
	CPrototypeMgr::GetInst()->SavePrototype(pFile);

	// SceneObject
	CScene* pScene = CSceneMgr::GetInst()->GetScene();

	// LayerSave
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		SaveLayer(pScene->GetLayer(i), pFile);
	}


	fclose(pFile);
	return true;
}

bool CSaveLoadMgr::LoadScene(const wstring & strFilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	// 리소스
	LoadResource(pFile);

	// Collision Layer Check 상태 불러오기.
	fread(CCollisionMgr::GetInst()->GetLayerCheck(), sizeof(UINT) * MAX_LAYER, 1, pFile);

	// Scene Prototype 불러오기.
	CPrototypeMgr::GetInst()->LoadPrototype(pFile);

	// SceneObject
	CScene* pScene = new CScene;
	pScene->Init();

	// LayerLoad
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		LoadLayer(pScene->GetLayer(i), pFile);
	}

	CSceneMgr::GetInst()->ChangeScene(pScene);


	fclose(pFile);


	return true;
}

bool CSaveLoadMgr::SaveLayer(CLayer * pLayer, FILE * pFile)
{
	// Layer의 이름 저장
	SaveWString(pFile, pLayer->GetName());

	const vector<CGameObject*>& vecParentObj = pLayer->GetParentObjVec();

	// 오브젝트 개수 저장.
	UINT iCount = (UINT)vecParentObj.size();
	fwrite(&iCount, 4, 1, pFile);

	for (UINT i = 0; i < iCount; ++i)
	{
		SaveGameObject(vecParentObj[i], pFile);
	}

	return true;
}

bool CSaveLoadMgr::LoadLayer(CLayer * pLayer, FILE * pFile)
{
	// Layer의 이름 읽기
	wstring strName = LoadWString(pFile);
	pLayer->SetName(strName);

	// 오브젝트 개수 읽기
	UINT iCount = 0;
	fread(&iCount, 4, 1, pFile);

	for (UINT i = 0; i < iCount; ++i)
	{
		CGameObject* pObj = new CGameObject;
		LoadGameObject(pObj, pFile);
		pLayer->AddGameObject(pObj);
	}

	return true;
}

bool CSaveLoadMgr::SaveGameObject(CGameObject * pObject, FILE * pFile)
{
	// 이름 저장
	SaveWString(pFile, pObject->GetName());

	// Component
	UINT i = 0;
	for (; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		CComponent* pCom = pObject->GetComponent((COMPONENT_TYPE)i);
		if (pCom != nullptr)
		{
			pCom->SaveToScene(pFile);
		}
	}

	fwrite(&i, 4, 1, pFile); // end

	const vector<CScript*> vecScript = pObject->GetScriptVec();
	UINT iScriptCount = vecScript.size();
	fwrite(&iScriptCount, 4, 1, pFile);

	for (UINT i = 0; i < iScriptCount; ++i)
	{
		SaveWString(pFile, CScriptMgr::GetScriptName(vecScript[i])); // 어떤 스크립트인지 저장.
		vecScript[i]->SaveToScene(pFile); // {} 저장하길 원하는 정보가 있다면 오버라이딩.
	}
	

	// Child  오브젝트 저장.
	const vector<CGameObject*> vecChild = pObject->GetChildVec();

	UINT iChildCount = vecChild.size();
	fwrite(&iChildCount, 4, 1, pFile);

	for (UINT i = 0; i < iChildCount; ++i)
	{
		SaveGameObject(vecChild[i], pFile);
	}

	return true;
}

bool CSaveLoadMgr::LoadGameObject(CGameObject * pObject, FILE * pFile)
{
	// 이름 읽어오기
	wstring strName = LoadWString(pFile);
	pObject->SetName(strName);

	// Component
	COMPONENT_TYPE iComType = (COMPONENT_TYPE)0;
	fread(&iComType, 4, 1, pFile);


	while (iComType != COMPONENT_TYPE::END)
	{
		CComponent* pCom = nullptr;
		switch (iComType)
		{
		case COMPONENT_TYPE::TRANSFORM:
			pCom = new CTransform;
			break;
		case COMPONENT_TYPE::MESHRENDER:
			pCom = new CMeshRender;
			break;
		case COMPONENT_TYPE::CAMERA:
			pCom = new CCamera;
			break;
		case COMPONENT_TYPE::COLLIDER2D:
			pCom = new CCollider2D;
			break;
		case COMPONENT_TYPE::COLLIDER3D:
			break;
		case COMPONENT_TYPE::ANIMATOR2D:
			pCom = new CAnimator2D;
			break;
		case COMPONENT_TYPE::ANIMATOR3D:
			break;
		case COMPONENT_TYPE::LIGHT2D:
			pCom = new CLight2D;
			break;
		case COMPONENT_TYPE::LIGHT3D:
			break;
		case COMPONENT_TYPE::TILEMAP2D:
			pCom = new CTileMap2D;
			break;
		}

		pCom->LoadFromScene(pFile);
		pObject->AddComponent(pCom);

		fread(&iComType, 4, 1, pFile);
	}

	UINT iScriptCount = 0;
	fread(&iScriptCount, 4, 1, pFile);

	for (INT i = 0; i < iScriptCount; ++i)
	{
		wstring strScriptName = LoadWString(pFile);

		// 이름을 토대로 스크립트 생성.
		CScript* pScript = CScriptMgr::GetScript(strScriptName);
		
		pScript->LoadFromScene(pFile);

		pObject->AddComponent(pScript);
	}


	UINT iChildCound = 0;
	fread(&iChildCound, 4, 1, pFile);

	for (UINT i = 0; i < iChildCound; ++i)
	{
		CGameObject* pChild = new CGameObject;
		LoadGameObject(pChild, pFile);

		pObject->AddChild(pChild);
	}

	return true;
}

bool CSaveLoadMgr::SaveResource(FILE * pFile)
{
	UINT i = 0;
	for (; i < (UINT)RES_TYPE::END; ++i)
	{
		const map<wstring, CResource*>& mapRes = CResMgr::GetInst()->GetResMap((RES_TYPE)i);

		if (i == (UINT)RES_TYPE::SHADER || i == (UINT)RES_TYPE::MESH)
			continue;

		// 리소스 타입
		fwrite(&i, 4, 1, pFile);

		// 일단 0을 저장하고 이 자리를 알아둔 이후에 다시 돌아와서 개수를 저장.
		int iResCount = 0;
		int rescountpos = ftell(pFile);
		fwrite(&iResCount, 4, 1, pFile);

		for (const auto& pair : mapRes)
		{
			if (pair.second->GetRef() > 0) // 하나라도 참조하는 경우
			{
				++iResCount;
				pair.second->SaveToScene(pFile);
			}
		}

		// 각 리소스에 대하여 개수만큼 Load를 하고 개수를 저장.
		long endpos = ftell(pFile);
		fseek(pFile, -(endpos - rescountpos), SEEK_CUR);
		fwrite(&iResCount, 4, 1, pFile);
		fseek(pFile, 0, SEEK_END);
		
	}

	// end 저장 
	fwrite(&i, 4, 1, pFile);

	return true;
}

bool CSaveLoadMgr::LoadResource(FILE * pFile)
{
	UINT i = 0;
	int iResCount = 0;

	while (true)
	{
		fread(&i, 4, 1, pFile);

		if (i == (UINT)RES_TYPE::END)
			break;

		fread(&iResCount, 4, 1, pFile);

		for (int iRes = 0; iRes < iResCount; ++iRes)
		{
			switch ((RES_TYPE)i)
			{
			case RES_TYPE::MATERIAL:
			{
				Ptr<CMaterial> pRes = new CMaterial;
				if (!pRes->LoadFromScene<CMaterial>(pFile))
				{
					delete pRes.GetPointer();
				}
				else
					CResMgr::GetInst()->AddRes<CMaterial>(pRes->GetName(), pRes);
			}
				break;

			case RES_TYPE::TEXTURE:
			{
				Ptr<CTexture> pRes = new CTexture;
				if (!pRes->LoadFromScene<CTexture>(pFile))
				{
					delete pRes.GetPointer();
				}
				else
					CResMgr::GetInst()->AddRes<CTexture>(pRes->GetName(), pRes);
			}
				break;

			case RES_TYPE::SOUND:
			{
				/*Ptr<CTexture> pRes = new CTexture;
				if (!pRes->LoadFromScene<CTexture>(pFile))
				{
					delete pRes.GetPointer();
				}
				else
					CResMgr::GetInst()->AddRes<CTexture>(pRes->GetName(), pRes);*/
			}

				break;
			}
		}

	}
	
	return true;
}
