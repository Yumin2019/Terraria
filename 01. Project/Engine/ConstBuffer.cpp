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

	// ��� ���� ����
	D3D11_BUFFER_DESC tBufferDesc = {};

	tBufferDesc.ByteWidth = m_iSize;
	tBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // const buffer
	tBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	DEVICE->CreateBuffer(&tBufferDesc, nullptr, &m_pBuffer);

	if (m_pBuffer == nullptr)
		assert(false);

	m_pSystem = (BYTE*)malloc(iByteWidth);
	// ������۸� �����ϴ� ������ ������ ũ�⸦ �Ҵ��صд�.
	
}

void CConstBuffer::SetData(void * pSrc)
{
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	
	CONTEXT->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, pSrc, m_iSize);
	CONTEXT->Unmap(m_pBuffer, 0);

	// �ش� �ּҸ� �Ѱ��ָ�, ����� ����� �����Ͽ�, Buffer�� �Ѱ��ִ� �Լ��̴�.
	
	m_iCurSize += m_iSize;

	// �����͸� ������ �� ����, Size�� ���ؼ� ���� ��ġ�� ���صд�.
	
	// �ܼ��� SetData(void*) �Լ��� ����ϸ�, ó���� �������� ����� ���� Buffer�� �Ѱ��ִ� ó���� �ϰ�
	// SetData(void*, size_t)�Լ��� ����ϸ� �ٸ� ó���� �ȴ�.
}

void CConstBuffer::SetData(void * pSrc, size_t iSize)
{
	memcpy(m_pSystem + m_iCurSize, pSrc, iSize);
	m_iCurSize += iSize;

	// Size(��ü ũ�� ��ŭ �Ҵ���ѵ� ����)�� �������� �Ͽ�,
	// �츮�� �־��� �Ϻ� Size ��ŭ�� �޸� �����صд�.
	// �̷��� ��� Size�� �� ä������ ��쿡�� Shader �Լ��� �����ִ� ó���� �Ѵ�.

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
	//iShader UINT ���� �� Shader�� flag�� &������ �Ͽ�, true�� �ǹ��Ѵٸ� 
	// �ش� ��Ʈ�� �����ִٴ� ���� �ǹ��ϹǷ� �ش� Shader���� ������۸� �Ѱ��ָ� �ȴ�.

	// m_iShader �� �� ����, iShaderType�� �̿��ϰ� �ִ�.

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

