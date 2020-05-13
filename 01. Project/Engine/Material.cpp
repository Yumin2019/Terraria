#include "stdafx.h"
#include "Material.h"
#include "Device.h"
#include "ConstBuffer.h"
#include "ResMgr.h"
#include "Core.h"

CMaterial::CMaterial() :
	CResource(RES_TYPE::MATERIAL),
	m_bFileSave(true)
{
	m_tParam.m_tDiff = Vec4(0.5f, 0.5f, 0.5f, 0.5f);
	m_tParam.m_tSpec = Vec4(1.f, 1.f, 1.f, 1.f);
	m_tParam.m_tEmv = Vec4(0.f, 0.f, 0.f, 0.f);
}


CMaterial::~CMaterial()
{
}


void CMaterial::SetShader(Ptr<CShader> pShader)
{
	m_pShader = pShader;
}

void CMaterial::UpdateData()
{

	// Texture Register Update
	for (UINT i = 0; i < (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0; ++i)
	{
		if (m_pTexArray[i] != nullptr)
		{
			m_pTexArray[i]->UpdateData((UINT)SHADER_TYPE::ST_PIXEL, i);
			m_tParam.m_iTexArr[i] = 1;

			// �� Texture���� �ȼ� ���̴��� ������ �ѱ��, 
			// i��° Texture Register�� �ؽ�ó�� �Ѱ��ִ� ������ �Ѵ�.

			// �̶�, �� �ؽ�ó�� �Ѱ��ִ� ���� ������ Shader Resource View.. �� ���� �̷������.
			// m_pSRV.. ( Texture Ŭ������ ����̴� )

			// �ؽ�ó�� �����ϴ� �뵵�� ����ϴ� Register�� ������ 4���� ��������.
			// ����, ���� ������Ʈ�� �̰��� ��������ϴ� �������� ������ �Ǵµ�,
			// �� ������Ʈ���� Render�� �ϱ� ������ Texture�� �����ϰ�, �װ��� ����ϰ�
			// �� �ٸ� ������Ʈ�� �ٽ� �ڽ��� ���� �����ϰ�, �װ��� ����ϴ� ������� �����ϸ� �ȴ�.
		}
		else
		{
			// �ؽ�ó�� ������ ���´�� �о������.
			CTexture::ClearData((UINT)SHADER_TYPE::ST_PIXEL, i);
			m_tParam.m_iTexArr[i] = 0;
			// i��°�� �ؽ�ó�� ����. (����Ÿ�� ��¿� �̿��Ѵ�)
		}
	}


	static CConstBuffer* pCB = GET_SINGLE(CDevice)->FindConstBuffer(L"MaterialParameter");

	pCB->SetData(&m_tParam);
	pCB->UpdateData(ST_ALL);

	if (m_pShader != nullptr)
		m_pShader->UpdateData();

}

void CMaterial::SetData(SHADER_PARAM eParam, void * pData)
{
	// Material�� ���ؼ� ���ϴ� Shader Parameter�� ���� �Ѱ� �� �� �ֵ��� �ϴ� �Լ��̴�.
	// ���� Update Data�Լ��� ���ؼ�, m_tParam�� �� ������ �Ѱ��ְ� �ִ�.
	// ��, flag�� Ȯ���ϰ�, pData���� �� ������ ��ŭ �Ͼ�ͼ�, m_tParam ����ü�� ������ �ϰ�, 
	// �̷��� ������ ���� ������۸� �̿��ؼ� �Ѱ��ִ� ���̴�.



	switch (eParam)
	{
	case SHADER_PARAM::INT_0:
	case SHADER_PARAM::INT_1:
	case SHADER_PARAM::INT_2:
	case SHADER_PARAM::INT_3:
		m_tParam.m_IntArray[(UINT)eParam - (UINT)SHADER_PARAM::INT_0] = *((int*)pData);
		break;
	case SHADER_PARAM::FLOAT_0:
	case SHADER_PARAM::FLOAT_1:
	case SHADER_PARAM::FLOAT_2:
	case SHADER_PARAM::FLOAT_3:
		m_tParam.m_FloatArray[(UINT)eParam - (UINT)SHADER_PARAM::FLOAT_0] = *((float*)pData);
		break;
	case SHADER_PARAM::VEC2_0:
	case SHADER_PARAM::VEC2_1:
	case SHADER_PARAM::VEC2_2:
	case SHADER_PARAM::VEC2_3:
		m_tParam.m_Vec2Array[(UINT)eParam - (UINT)SHADER_PARAM::VEC2_0] = *((Vec2*)pData);

		break;
	case SHADER_PARAM::VEC4_0:
	case SHADER_PARAM::VEC4_1:
	case SHADER_PARAM::VEC4_2:
	case SHADER_PARAM::VEC4_3:
		m_tParam.m_Vec4Array[(UINT)eParam - (UINT)SHADER_PARAM::VEC4_0] = *((Vec4*)pData);

		break;
	case SHADER_PARAM::MATRIX_0:
	case SHADER_PARAM::MATRIX_1:
	case SHADER_PARAM::MATRIX_2:
	case SHADER_PARAM::MATRIX_3:
		m_tParam.m_MatArray[(UINT)eParam - (UINT)SHADER_PARAM::MATRIX_0] = *((Matrix*)pData);

		break;
	case SHADER_PARAM::TEX_0:
	case SHADER_PARAM::TEX_1:
	case SHADER_PARAM::TEX_2:
	case SHADER_PARAM::TEX_3:
		// �ؽ�Ʈ�� ���� ������ ���� ó���ϰ� �ִ�.
		// ��� �����͸� �ּҸ� �Ѱܹ޴� ó���� �ϰ� �ִ�.
		// ����, Ptr<CTexture>�� �ּҰ� ���� ���̴�.

		if (pData == nullptr)
		{
			m_pTexArray[(UINT)eParam - (UINT)SHADER_PARAM::TEX_0] = nullptr;
		}
		else
		{
			m_pTexArray[(UINT)eParam - (UINT)SHADER_PARAM::TEX_0] = *((Ptr<CTexture>*)pData);
		}
		
		break;
	}

	if (CCore::GetInst()->GetSceneMode() == SCENE_MODE::STOP)
	{
		Save(CPathMgr::GetResPath());
	}

}

CMaterial * CMaterial::Clone() 
{
	CResource::Clone();

	CMaterial* pClone = new CMaterial(*this);

	wchar_t szAdd[50] = {};
	wsprintf(szAdd, L"_Clone_%d.mtrl", GetCloneCount());

	wstring strPath = L"/Material/";
	strPath += CPathMgr::GetFileName(GetName().c_str());
	strPath += szAdd;
	
	pClone->SetName(strPath);
	pClone->SetPath(strPath);


	if (SCENE_MODE::STOP == CCore::GetInst()->GetSceneMode())
	{
		// Stop ��忡���� ������ �״�� ���ӿ� ������ �Ǿ�� �Ѵ�.
		// ���� �̰��� Clone�Ͽ� ������ �ǵ��� �Ѵ�.
		pClone->m_bFileSave = true;
		pClone->Save(pClone->GetPath());
		CResMgr::GetInst()->AddRes<CMaterial>(strPath, pClone);
	}
	else
	{
		CResMgr::GetInst()->AddCloneRes<CMaterial>(pClone);
	}

	return pClone;
}

void CMaterial::Load(const wstring & strFullPath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFullPath.c_str(), L"rb");

	if (!pFile)
		return;

	// Shader�� Ű��.
	wstring strName = LoadWString(pFile);
	m_pShader = CResMgr::GetInst()->FindRes<CShader>(strName); // Shader�� �����ϰ� �о���� �ý����� ���� ��� �ݵ�� ã�ƿ´�.

	// ���̴� �Ķ����
	fread(&m_tParam, sizeof(MtrlParam), 1, pFile);

	UINT iMaxCount = (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0;
	for (UINT i = 0; i < iMaxCount; ++i)
	{
		UINT iExist = 0;
		fread(&iExist, 4, 1, pFile);

		if (iExist)
		{
			wstring strKey = LoadWString(pFile);
			wstring strPath = LoadWString(pFile);

			m_pTexArray[i] = CResMgr::GetInst()->FindRes<CTexture>(strKey);

			if (m_pTexArray[i] == nullptr)
			{
				m_pTexArray[i] = CResMgr::GetInst()->Load<CTexture>(strKey, strPath);
			}
		}

	}

	fclose(pFile);

}

void CMaterial::Save(const wstring & strPath)
{
	if (!m_bFileSave)
		return;

	wstring strFilePath = strPath;
	strFilePath += GetName();

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	if (!pFile)
		return;

	// Shader�� Ű��.
	if (m_pShader != nullptr)
		SaveWString(pFile, m_pShader->GetName());
	else
		SaveWString(pFile, L"");

	// ���̴� �Ķ����
	fwrite(&m_tParam, sizeof(MtrlParam), 1, pFile);

	UINT iMaxCount = (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0;
	for (UINT i = 0; i < iMaxCount; ++i)
	{
		UINT iExist = 1;
		if (m_pTexArray[i] == nullptr)
		{
			iExist = 0;
			fwrite(&iExist, 4, 1, pFile);
			continue;
		}

		fwrite(&iExist, 4, 1, pFile);
		SaveWString(pFile, m_pTexArray[i]->GetName());
		SaveWString(pFile, m_pTexArray[i]->GetPath());

	}

	fclose(pFile);

}
