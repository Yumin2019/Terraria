#include "stdafx.h"
#include "ConstBuffer.h"
#include "Device.h"
#include "Core.h"

CConstBuffer::CConstBuffer() :
	m_pBuffer(nullptr),
	m_iRegisterNum(0),
	m_iShader(0),
	m_iSize(0),
	m_iCurSize(0),
	m_pSystem(nullptr)
{
	
}


CConstBuffer::~CConstBuffer()
{
	SAFE_RELEASE(m_pBuffer);
	SAFE_DELETE(m_pSystem);
}


void CConstBuffer::Create(UINT iByteWidth, UINT iRegisterNum, const wstring & strName)
{
	SetName(strName);

	m_iSize = iByteWidth;
	m_iRegisterNum = iRegisterNum;

	// 상수 버퍼 생성
	D3D11_BUFFER_DESC tBufferDesc = {};

	tBufferDesc.ByteWidth = m_iSize;
	tBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // const buffer
	tBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	DEVICE->CreateBuffer(&tBufferDesc, nullptr, &m_pBuffer);

	if (m_pBuffer == nullptr)
		assert(false);

	m_pSystem = (BYTE*)malloc(iByteWidth);
	// 상수버퍼를 생성하는 순간에 동일한 크기를 할당해둔다.
	
}

void CConstBuffer::SetData(void * pSrc)
{
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	
	CONTEXT->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, pSrc, m_iSize);
	CONTEXT->Unmap(m_pBuffer, 0);

	// 해당 주소를 넘겨주면, 저장된 사이즈를 토대로하여, Buffer에 넘겨주는 함수이다.
	
	m_iCurSize += m_iSize;

	// 데이터를 셋팅할 때 마다, Size를 더해서 현재 위치를 구해둔다.
	
	// 단순히 SetData(void*) 함수를 사용하면, 처음에 셋팅해준 사이즈를 토대로 Buffer에 넘겨주는 처리를 하고
	// SetData(void*, size_t)함수를 사용하면 다른 처리가 된다.
}

void CConstBuffer::SetData(void * pSrc, size_t iSize)
{
	memcpy(m_pSystem + m_iCurSize, pSrc, iSize);
	m_iCurSize += iSize;

	// Size(전체 크기 만큼 할당시켜둔 공간)을 기준으로 하여,
	// 우리가 넣어준 일부 Size 만큼을 메모리 복사해둔다.
	// 이렇게 모든 Size가 다 채워지는 경우에는 Shader 함수로 보내주는 처리를 한다.

	if (m_iCurSize > m_iSize)
		assert(false);

	if (m_iCurSize == m_iSize)
	{
		D3D11_MAPPED_SUBRESOURCE tSub = {};

		CONTEXT->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
		memcpy(tSub.pData, m_pSystem, m_iCurSize);
		CONTEXT->Unmap(m_pBuffer, 0);

		m_iCurSize = 0;
	}

}

void CConstBuffer::UpdateData(UINT iShaderType)
{
	//iShader UINT 값을 각 Shader의 flag와 &연산을 하여, true를 의미한다면 
	// 해당 비트가 켜져있다는 것을 의미하므로 해당 Shader에서 상수버퍼를 넘겨주면 된다.

	// m_iShader 는 안 쓰고, iShaderType을 이용하고 있다.

	if ((UINT)SHADER_TYPE::ST_VERTEX & iShaderType)
		CONTEXT->VSSetConstantBuffers(m_iRegisterNum, 1, &m_pBuffer);
	if ((UINT)SHADER_TYPE::ST_HULL & iShaderType)
		CONTEXT->HSSetConstantBuffers(m_iRegisterNum, 1, &m_pBuffer);
	if ((UINT)SHADER_TYPE::ST_DOMAIN & iShaderType)
		CONTEXT->DSSetConstantBuffers(m_iRegisterNum, 1, &m_pBuffer);
	if ((UINT)SHADER_TYPE::ST_GEOMETRY & iShaderType)
		CONTEXT->GSSetConstantBuffers(m_iRegisterNum, 1, &m_pBuffer);
	if ((UINT)SHADER_TYPE::ST_COMPUTE & iShaderType)
		CONTEXT->CSSetConstantBuffers(m_iRegisterNum, 1, &m_pBuffer);
	if ((UINT)SHADER_TYPE::ST_PIXEL & iShaderType)
		CONTEXT->PSSetConstantBuffers(m_iRegisterNum, 1, &m_pBuffer);
}

