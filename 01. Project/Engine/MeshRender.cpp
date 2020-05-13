#include "stdafx.h"
#include "MeshRender.h"
#include "Transform.h"
#include "Shader.h"
#include "Mesh.h"

#include "ResMgr.h"
#include "Animator2D.h"
#include "Script.h"
#include "TileMap2D.h"


CMeshRender::CMeshRender() :
	CComponent(COMPONENT_TYPE::MESHRENDER),
	m_pMesh(nullptr),
	m_pMtrl(nullptr)
{
}


CMeshRender::~CMeshRender()
{
	/*
	Shader, Mesh는 Resource Manager의 관리 대상이다.
	*/
}

Ptr<CMaterial> CMeshRender::GetCloneMaterial()
{
	
	if (m_pMtrl == nullptr)
		return nullptr;

	// 자신이 참조하고 있는 Mtrl에 대해서 복사를 한다.
	// 여기서 

	m_pMtrl = m_pMtrl->Clone();
	return m_pMtrl;
}


void CMeshRender::Render()
{
	// 툴과 연동하는 경우에는 Mesh, Mtrl을 적용하지 않는 경우가 존재한다.
	if (m_pMtrl == nullptr || m_pMesh == nullptr || m_pMtrl->GetShader() == nullptr)
		return;

	/*
	Game Object를 그리는 기능은 오직 MeshRender의 Render함수를 통해서만 이루어진다.


	// UpdateData를 하는 이유는 결국 Shader 처리를 하기 위함이다. 따라서 출력되는 오브젝트에서 사용한다.
	// 따라서, MeshRender를 통해서 처리를 하도록 한다.
	*/

	GetTransform()->UpdateData(); // 위치 정보를 보낸다.
	// 오브젝트를 통해서 Transform을 얻어와서, 행렬 정보를 Shader에 보낸다.
	// 출력을 하기 위해서 Transfrom에 접근해서 위치 정보를 Shader로 넘겨준다.

	if (GetAnimator2D())
		GetAnimator2D()->UpdateData(); // 애니메이션 Ani2D 구조체의 정보를 셋팅하여 보낸다.

	if (GetTileMap2D())
		GetTileMap2D()->UpdateData();

	const vector<CScript*>& vecScript = GetObj()->GetScriptVec();

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->UpdateData();
	}


	m_pMtrl->UpdateData(); // 텍스처를 셋팅하거나 MaterialParam의 정보를 채워서 보낸다.
	// 재질 정보를 Shader에게 보낸다. 

	m_pMesh->Render(); // IA단계에서 사용할 데이터를 셋팅하고, PipeLine 시작
	// 위에서 보낸 상수버퍼를 토대로 하나의 오브젝트를 출력해준다.

	// data clear
	CAnimator2D::ClearData();
}

bool CMeshRender::SaveToScene(FILE * pFile)
{
	UINT iType = (UINT)GetComponentType();
	fwrite(&iType, 4, 1, pFile);

	//Ptr<CMaterial>	m_pMtrl;
	//Ptr<CMesh>		m_pMesh;

	fwrite(&m_pMesh, sizeof(void*), 1, pFile);

	if (m_pMesh != nullptr)
	{
		SaveWString(pFile, m_pMesh->GetName());
		SaveWString(pFile, m_pMesh->GetPath());
	}

	fwrite(&m_pMtrl, sizeof(void*), 1, pFile);

	if (m_pMtrl != nullptr)
	{
		SaveWString(pFile, m_pMtrl->GetName());
		SaveWString(pFile, m_pMtrl->GetPath());
	}

	return true;
}

bool CMeshRender::LoadFromScene(FILE * pFile)
{
	void* pData = nullptr;
	fread(&pData, sizeof(void*), 1, pFile);

	if (pData)
	{
		// Mesh
		wstring strMesh = LoadWString(pFile);
		wstring strPath = LoadWString(pFile);
		

		m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(strMesh);

		if (m_pMesh == nullptr)
		{
			m_pMesh = CResMgr::GetInst()->Load<CMesh>(strMesh, strPath);
		}
	}

	fread(&pData, sizeof(void*), 1, pFile);

	if (pData)
	{
		// Mtrl
		wstring strMtrl = LoadWString(pFile);
		wstring strPath = LoadWString(pFile);
		/*
		strMtrl = L"/" + strMtrl;
		strPath = L"/" + strPath;*/

		m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(strMtrl);

		if (m_pMtrl == nullptr)
		{
			m_pMtrl = CResMgr::GetInst()->Load<CMaterial>(strMtrl, strPath);
		}
	}

	return true;
}
