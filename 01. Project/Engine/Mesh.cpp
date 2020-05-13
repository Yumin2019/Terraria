#include "stdafx.h"
#include "Mesh.h"
#include "Device.h"


CMesh::CMesh() :
	CResource(RES_TYPE::MESH),
	m_pVB(nullptr),
	m_pIB(nullptr),
	m_pLayout(nullptr),
	m_iVtxSize(0),
	m_iVtxCount(0),
	m_eVtxUsage(D3D11_USAGE::D3D11_USAGE_DEFAULT),
	m_eIdxFormat(DXGI_FORMAT::DXGI_FORMAT_R32_UINT),
	m_iIdxCount(0),
	m_eIdxUsage(D3D11_USAGE::D3D11_USAGE_DEFAULT),
	m_pVtxSysMem(nullptr),
	m_pIdxSysMem(nullptr)
{
}


CMesh::~CMesh()
{
	SAFE_DELETE(m_pIdxSysMem);
	SAFE_DELETE(m_pVtxSysMem);


	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pLayout);
	
}

void CMesh::Create(void * pVtxMem, UINT iVtxSize, UINT iVtxCount, D3D11_USAGE eVtxUsage, 
	void * pIdxMem, DXGI_FORMAT eIdxFormat, UINT iIdxCount, D3D11_USAGE eIdxUsage,
	D3D11_PRIMITIVE_TOPOLOGY eTopology)
{
	m_iVtxSize = iVtxSize;
	m_iVtxCount = iVtxCount;

	m_eVtxUsage = eVtxUsage;
	m_eIdxUsage = eIdxUsage;

	m_iIdxCount = iIdxCount;
	m_eIdxFormat = eIdxFormat;

	m_eTopology = eTopology;


	// 동적할당 된 메모리가 들어올지, 전역 변수의 메모리가 들어올지 알 수 없으므로 Deep Copy
	m_pVtxSysMem = malloc(m_iVtxSize * m_iVtxCount);
	memcpy(m_pVtxSysMem, pVtxMem, m_iVtxSize * m_iVtxCount); 

	m_pIdxSysMem = malloc(GetSizeofFormat(m_eIdxFormat) * m_iIdxCount);
	memcpy(m_pIdxSysMem, pIdxMem, GetSizeofFormat(m_eIdxFormat) * m_iIdxCount);

	// Vertex Buffer
	D3D11_BUFFER_DESC tBufferDesc = {};

	tBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	tBufferDesc.Usage = eVtxUsage;

	// DYNAMIC 옵션이라면 쓰기 옵션 지정.
	if (tBufferDesc.Usage == D3D11_USAGE_DYNAMIC)
		tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	
	tBufferDesc.ByteWidth = iVtxSize * iVtxCount;

	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = pVtxMem;

	DEVICE->CreateBuffer(&tBufferDesc, &tSub, &m_pVB);

	// Index Buffer
	tBufferDesc = {};
	tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tBufferDesc.Usage = eIdxUsage;

	if (tBufferDesc.Usage == D3D11_USAGE_DYNAMIC)
		tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

	tBufferDesc.ByteWidth = GetSizeofFormat(eIdxFormat) * iIdxCount;
	
	tSub = {};
	tSub.pSysMem = pIdxMem;

	DEVICE->CreateBuffer(&tBufferDesc, &tSub, &m_pIB);

	
}

void CMesh::Render()
{
	UINT	iStride = m_iVtxSize;
	UINT	iOffset = 0;
	
	// IA 단계에서 사용할 정보를 설정.
	CONTEXT->IASetVertexBuffers(0, 1, &m_pVB, &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(m_pIB, m_eIdxFormat, 0);	
	CONTEXT->IASetPrimitiveTopology(m_eTopology);
	
	
	// Rendering PipeLine
	CONTEXT->DrawIndexed(m_iIdxCount, 0, 0);
	//CONTEXT->DrawIndexedInstanced(m_iIdxCount, 4, 0, 0, 0);
}

