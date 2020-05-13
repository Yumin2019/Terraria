#include "stdafx.h"
#include "Shader.h"
#include "PathMgr.h"
#include "Device.h"

#include "BlendState.h"
#include "DepthStencilState.h"

vector<D3D11_INPUT_ELEMENT_DESC> CShader::g_vecLayout;
int CShader::g_AlignedByteOffset = 0;

CShader::CShader() :
	CResource(RES_TYPE::SHADER),
	m_pVS(nullptr),
	m_pHS(nullptr),
	m_pDS(nullptr),
	m_pGS(nullptr),
	m_pPS(nullptr),
	m_pVSBlob(nullptr),
	m_pHSBlob(nullptr),
	m_pDSBlob(nullptr),
	m_pGSBlob(nullptr),
	m_pPSBlob(nullptr),
	m_pErrBlob(nullptr),
	m_pCS(nullptr),
	m_pCSBlob(nullptr),
	m_pDepthStencilState(nullptr)
{
}


CShader::~CShader()
{
	SAFE_RELEASE(m_pVS);
	SAFE_RELEASE(m_pHS);
	SAFE_RELEASE(m_pDS);
	SAFE_RELEASE(m_pGS);
	SAFE_RELEASE(m_pPS);
	SAFE_RELEASE(m_pVSBlob);
	SAFE_RELEASE(m_pHSBlob);
	SAFE_RELEASE(m_pDSBlob);
	SAFE_RELEASE(m_pGSBlob);
	SAFE_RELEASE(m_pPSBlob);
	SAFE_RELEASE(m_pErrBlob);
	SAFE_RELEASE(m_pCS);
	SAFE_RELEASE(m_pCSBlob);
	SAFE_RELEASE(m_pLayout);
}

void CShader::CreateLayoutDesc()
{
	// ���� ����Լ��� �����Լ�ó�� ���� Layout ������ ����� �Լ��� Shader���� �����Ѵ�.
	// �츮 Engine������ ���������� �̿��ؼ� ��� ������ ���� Layout������ �ϳ��� �� ���̴�.

	// �ѱ�� ���ϴ� ����ü ������ ���� ����. (layout ����ü)
	// ���� Shader���� layout*�� ���� ������ ���̴�.

	D3D11_INPUT_ELEMENT_DESC tLayoutDesc = {};
	int AlignedByteOffset = 0;

	tLayoutDesc.AlignedByteOffset = AlignedByteOffset;	 // offset, �޸� ���� ��ġ
	tLayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT; // ���� enum ������ ����� �˸�
	tLayoutDesc.SemanticName = "POSITION"; // Vertex Shader���� Ž�������� ����� Semantic �̸�
	tLayoutDesc.SemanticIndex = 0;		  // ������ Semantic �̸��� ��� �����ϴ� �ε��� ����
	tLayoutDesc.InputSlot = 0;
	tLayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tLayoutDesc.InstanceDataStepRate = 0;

	g_vecLayout.push_back(tLayoutDesc);

	AlignedByteOffset += GetSizeofFormat(tLayoutDesc.Format);

	tLayoutDesc.AlignedByteOffset = AlignedByteOffset;				 // offset, �޸� ���� ��ġ
	tLayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // ���� enum ������ ����� �˸�
	tLayoutDesc.SemanticName = "COLOR"; // Vertex Shader���� Ž�������� ����� Semantic �̸�
	tLayoutDesc.SemanticIndex = 0;		  // ������ Semantic �̸��� ��� �����ϴ� �ε��� ����
	tLayoutDesc.InputSlot = 0;
	tLayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tLayoutDesc.InstanceDataStepRate = 0;

	g_vecLayout.push_back(tLayoutDesc);

	AlignedByteOffset += GetSizeofFormat(tLayoutDesc.Format);


	tLayoutDesc.AlignedByteOffset = AlignedByteOffset;				 // offset, �޸� ���� ��ġ
	tLayoutDesc.Format = DXGI_FORMAT_R32G32_FLOAT; // ���� enum ������ ����� �˸�
	tLayoutDesc.SemanticName = "TEXCOORD"; // Vertex Shader���� Ž�������� ����� Semantic �̸�
	tLayoutDesc.SemanticIndex = 0;		  // ������ Semantic �̸��� ��� �����ϴ� �ε��� ����
	tLayoutDesc.InputSlot = 0;
	tLayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tLayoutDesc.InstanceDataStepRate = 0;

	g_vecLayout.push_back(tLayoutDesc);

	AlignedByteOffset += GetSizeofFormat(tLayoutDesc.Format);
}

void CShader::CreateVertexShader(const wstring & strPath, const string & strFuncName, const string & strhlslVersion)
{

	// ���� ��θ� �������� (�����) ��ü ��θ� ���� hlsl �ڵ忡�� �Լ��� �������� version�� �������ְ�
	// blob ������ ä���. 
	// �̰��� �������� VertexShader�� �����Ѵ�.

	int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	wstring strFullPath = CPathMgr::GetResPath();
	strFullPath += strPath;

	char* pErr = nullptr;

	if (FAILED(D3DCompileFromFile(strFullPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		strFuncName.c_str(), strhlslVersion.c_str(), iFlag, 0, &m_pVSBlob, &m_pErrBlob)))
	{
		pErr = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErr, "Shader Error", MB_OK);
	}

	DEVICE->CreateVertexShader(m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), nullptr, &m_pVS);

	DEVICE->CreateInputLayout(&g_vecLayout[0], g_vecLayout.size(), m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), &m_pLayout);
}

void CShader::CreatePixelShader(const wstring & strPath, const string & strFuncName, const string & strhlslVersion)
{
	int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	wstring strFullPath = CPathMgr::GetResPath();
	strFullPath += strPath;

	char* pErr = nullptr;

	if (FAILED(D3DCompileFromFile(strFullPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		strFuncName.c_str(), strhlslVersion.c_str(), iFlag, 0, &m_pPSBlob, &m_pErrBlob)))
	{
		pErr = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErr, "Shader Error", MB_OK);
	}

	DEVICE->CreatePixelShader(m_pPSBlob->GetBufferPointer(), m_pPSBlob->GetBufferSize(), nullptr, &m_pPS);
}

void CShader::UpdateData()
{
	CONTEXT->VSSetShader(m_pVS, nullptr, 0);
	CONTEXT->HSSetShader(m_pHS, nullptr, 0);
	CONTEXT->DSSetShader(m_pDS, nullptr, 0);
	CONTEXT->GSSetShader(m_pGS, nullptr, 0);
	CONTEXT->CSSetShader(m_pCS, nullptr, 0);
	CONTEXT->PSSetShader(m_pPS, nullptr, 0);

	CONTEXT->IASetInputLayout(m_pLayout);

	if (m_pBlendState == nullptr)
		CONTEXT->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	else
		m_pBlendState->UpdateData(); 
	// ���������� BlendState�����͸� �־ OM �ܰ迡�� 
	// Blend State�� �������ش�.

	if (m_pDepthStencilState == nullptr)
		CONTEXT->OMSetDepthStencilState(nullptr, 0);
	else
		m_pDepthStencilState->UpdateData();



}
