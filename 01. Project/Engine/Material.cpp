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

			// 각 Texture마다 픽셀 쉐이더로 정보를 넘기고, 
			// i번째 Texture Register에 텍스처를 넘겨주는 것으로 한다.

			// 이때, 이 텍스처를 넘겨주는 전달 역할은 Shader Resource View.. 를 통해 이루어진다.
			// m_pSRV.. ( Texture 클래스의 멤버이다 )

			// 텍스처를 전달하는 용도로 사용하는 Register의 개수는 4개로 정해진다.
			// 따라서, 여러 오브젝트가 이걸을 돌려사용하는 형식으로 진행이 되는데,
			// 각 오브젝트에서 Render를 하기 직전에 Texture를 셋팅하고, 그것을 사용하고
			// 또 다른 오브젝트가 다시 자신의 것을 셋팅하고, 그것을 사용하는 방식으로 진행하면 된다.
		}
		else
		{
			// 텍스처가 없으면 없는대로 밀어버린다.
			CTexture::ClearData((UINT)SHADER_TYPE::ST_PIXEL, i);
			m_tParam.m_iTexArr[i] = 0;
			// i번째는 텍스처가 없다. (마젠타색 출력에 이용한다)
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
	// Material을 통해서 원하는 Shader Parameter의 값을 넘겨 줄 수 있도록 하는 함수이다.
	// 위의 Update Data함수를 통해서, m_tParam의 의 정보를 넘겨주고 있다.
	// 즉, flag를 확인하고, pData에서 그 사이즈 만큼 일어와서, m_tParam 구조체에 수정을 하고, 
	// 이렇게 수정한 것을 상수버퍼를 이용해서 넘겨주는 것이다.



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
		// 텍스트에 대한 전달은 따로 처리하고 있다.
		// 모든 데이터를 주소를 넘겨받는 처리를 하고 있다.
		// 따라서, Ptr<CTexture>의 주소가 들어올 것이다.

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
		// Stop 모드에서의 수정은 그대로 게임에 적용이 되어야 한다.
		// 따라서 이것을 Clone하여 저장이 되도록 한다.
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

	// Shader의 키값.
	wstring strName = LoadWString(pFile);
	m_pShader = CResMgr::GetInst()->FindRes<CShader>(strName); // Shader는 저장하고 읽어오는 시스템이 아직 없어서 반드시 찾아온다.

	// 쉐이더 파라미터
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

	// Shader의 키값.
	if (m_pShader != nullptr)
		SaveWString(pFile, m_pShader->GetName());
	else
		SaveWString(pFile, L"");

	// 쉐이더 파라미터
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
