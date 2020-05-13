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
	// 정적 멤버함수로 전역함수처럼 따로 Layout 정보를 만드는 함수를 Shader에서 정의한다.
	// 우리 Engine에서는 통합정점을 이용해서 모든 정점에 대한 Layout정보를 하나만 둘 것이다.

	// 넘기길 원하는 구조체 정보에 대한 셋팅. (layout 구조체)
	// 또한 Shader마다 layout*를 각각 셋팅할 것이다.

	D3D11_INPUT_ELEMENT_DESC tLayoutDesc = {};
	int AlignedByteOffset = 0;

	tLayoutDesc.AlignedByteOffset = AlignedByteOffset;	 // offset, 메모리 시작 위치
	tLayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT; // 포맷 enum 값으로 사이즈를 알림
	tLayoutDesc.SemanticName = "POSITION"; // Vertex Shader에서 탐색용으로 사용할 Semantic 이름
	tLayoutDesc.SemanticIndex = 0;		  // 동일한 Semantic 이름일 경우 증가하는 인덱스 정보
	tLayoutDesc.InputSlot = 0;
	tLayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tLayoutDesc.InstanceDataStepRate = 0;

	g_vecLayout.push_back(tLayoutDesc);

	AlignedByteOffset += GetSizeofFormat(tLayoutDesc.Format);

	tLayoutDesc.AlignedByteOffset = AlignedByteOffset;				 // offset, 메모리 시작 위치
	tLayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 포맷 enum 값으로 사이즈를 알림
	tLayoutDesc.SemanticName = "COLOR"; // Vertex Shader에서 탐색용으로 사용할 Semantic 이름
	tLayoutDesc.SemanticIndex = 0;		  // 동일한 Semantic 이름일 경우 증가하는 인덱스 정보
	tLayoutDesc.InputSlot = 0;
	tLayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tLayoutDesc.InstanceDataStepRate = 0;

	g_vecLayout.push_back(tLayoutDesc);

	AlignedByteOffset += GetSizeofFormat(tLayoutDesc.Format);


	tLayoutDesc.AlignedByteOffset = AlignedByteOffset;				 // offset, 메모리 시작 위치
	tLayoutDesc.Format = DXGI_FORMAT_R32G32_FLOAT; // 포맷 enum 값으로 사이즈를 알림
	tLayoutDesc.SemanticName = "TEXCOORD"; // Vertex Shader에서 탐색용으로 사용할 Semantic 이름
	tLayoutDesc.SemanticIndex = 0;		  // 동일한 Semantic 이름일 경우 증가하는 인덱스 정보
	tLayoutDesc.InputSlot = 0;
	tLayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tLayoutDesc.InstanceDataStepRate = 0;

	g_vecLayout.push_back(tLayoutDesc);

	AlignedByteOffset += GetSizeofFormat(tLayoutDesc.Format);
}

void CShader::CreateVertexShader(const wstring & strPath, const string & strFuncName, const string & strhlslVersion)
{

	// 얻어온 경로를 바탕으로 (상대경로) 전체 경로를 만들어서 hlsl 코드에서 함수의 진입점과 version을 설정해주고
	// blob 정보를 채운다. 
	// 이것을 바탕으로 VertexShader를 생성한다.

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
	// 내부적으로 BlendState포인터를 넣어서 OM 단계에서 
	// Blend State를 적용해준다.

	if (m_pDepthStencilState == nullptr)
		CONTEXT->OMSetDepthStencilState(nullptr, 0);
	else
		m_pDepthStencilState->UpdateData();



}
