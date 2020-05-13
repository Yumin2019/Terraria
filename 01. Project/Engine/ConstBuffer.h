#pragma once
#include "Entity.h"
class CConstBuffer :
	public CEntity
{

private:
	ID3D11Buffer*	m_pBuffer; // Constant Buffer�� �ѱ�� ���� ����.
	UINT			m_iRegisterNum; // ��� ���۸� �ѱ�µ� ����� �������� ��ȣ.
	UINT			m_iShader; // � Shader���� ������۸� �ѱ����� �����ϴ� UINT	 ??
	UINT			m_iSize; // �����Ϸ��� ����� ������

	UINT			m_iCurSize;
	BYTE*			m_pSystem;

public:
	void Create(UINT iByteWidth, UINT iRegisterNum, const wstring& strName = L"");
	// ��� ���۸� �����ϴ� �Լ��̴�.
	// ���������δ� ����� ������ / �������� ��ȣ�� �������ְ� �ִ�.

	void SetData(void* pSrc);
	// �ѱ���� �ϴ� ����� �ּҸ� �Ѱ��ָ�, ����� �ּҿ��� Size��ŭ�� �ű��, map
	// �ű� �����Ϳ� ���ؼ�, �ٽ� Unmap�� ���ְ� �ִ�.
	void SetData(void* pSrc, size_t iSize);

	void UpdateData(UINT iShaderType);
	// Ư�� Shader���� ��� ���۸� �ѱ�� �Ϳ� ���� ó���� ���ְ� �ִ�.
	// m_iShader��  bit�� ������ flag�� �����صξ���, �̰��� ������� 
	// �� �ڸ� ��Ʈ���� &������ ���ؼ� Ư�� ���̴����� ��� ���۸� �ѱ� ���� �ٽ� �������ְ� �ִ�.
public:
	CConstBuffer();
	virtual ~CConstBuffer();
};

