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
	Shader, Mesh�� Resource Manager�� ���� ����̴�.
	*/
}

Ptr<CMaterial> CMeshRender::GetCloneMaterial()
{
	
	if (m_pMtrl == nullptr)
		return nullptr;

	// �ڽ��� �����ϰ� �ִ� Mtrl�� ���ؼ� ���縦 �Ѵ�.
	// ���⼭ 

	m_pMtrl = m_pMtrl->Clone();
	return m_pMtrl;
}


void CMeshRender::Render()
{
	// ���� �����ϴ� ��쿡�� Mesh, Mtrl�� �������� �ʴ� ��찡 �����Ѵ�.
	if (m_pMtrl == nullptr || m_pMesh == nullptr || m_pMtrl->GetShader() == nullptr)
		return;

	/*
	Game Object�� �׸��� ����� ���� MeshRender�� Render�Լ��� ���ؼ��� �̷������.


	// UpdateData�� �ϴ� ������ �ᱹ Shader ó���� �ϱ� �����̴�. ���� ��µǴ� ������Ʈ���� ����Ѵ�.
	// ����, MeshRender�� ���ؼ� ó���� �ϵ��� �Ѵ�.
	*/

	GetTransform()->UpdateData(); // ��ġ ������ ������.
	// ������Ʈ�� ���ؼ� Transform�� ���ͼ�, ��� ������ Shader�� ������.
	// ����� �ϱ� ���ؼ� Transfrom�� �����ؼ� ��ġ ������ Shader�� �Ѱ��ش�.

	if (GetAnimator2D())
		GetAnimator2D()->UpdateData(); // �ִϸ��̼� Ani2D ����ü�� ������ �����Ͽ� ������.

	if (GetTileMap2D())
		GetTileMap2D()->UpdateData();

	const vector<CScript*>& vecScript = GetObj()->GetScriptVec();

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->UpdateData();
	}


	m_pMtrl->UpdateData(); // �ؽ�ó�� �����ϰų� MaterialParam�� ������ ä���� ������.
	// ���� ������ Shader���� ������. 

	m_pMesh->Render(); // IA�ܰ迡�� ����� �����͸� �����ϰ�, PipeLine ����
	// ������ ���� ������۸� ���� �ϳ��� ������Ʈ�� ������ش�.

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
