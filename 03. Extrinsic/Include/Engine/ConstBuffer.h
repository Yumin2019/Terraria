#pragma once
#include "Entity.h"
class CConstBuffer :
	public CEntity
{

private:
	ID3D11Buffer*	m_pBuffer; // Constant Buffer를 넘기기 위한 형태.
	UINT			m_iRegisterNum; // 상수 버퍼를 넘기는데 사용할 레지스터 번호.
	UINT			m_iShader; // 어떤 Shader에서 상수버퍼를 넘길지를 저장하는 UINT	 ??
	UINT			m_iSize; // 전달하려는 상수의 사이즈

	UINT			m_iCurSize;
	BYTE*			m_pSystem;

public:
	void Create(UINT iByteWidth, UINT iRegisterNum, const wstring& strName = L"");
	// 상수 버퍼를 생성하는 함수이다.
	// 내부적으로는 상수의 사이즈 / 레지스터 번호를 설정해주고 있다.

	void SetData(void* pSrc);
	// 넘기려고 하는 상수의 주소를 넘겨주면, 상수의 주소에서 Size만큼을 옮기고, map
	// 옮긴 데이터에 대해서, 다시 Unmap을 해주고 있다.
	void SetData(void* pSrc, size_t iSize);

	void UpdateData(UINT iShaderType);
	// 특정 Shader에서 상수 버퍼를 넘기는 것에 대한 처리를 해주고 있다.
	// m_iShader의  bit를 가지고 flag를 저장해두었고, 이것을 기반으로 
	// 각 자리 비트마다 &연산을 통해서 특정 쉐이더에서 상수 버퍼를 넘길 것을 다시 셋팅해주고 있다.
public:
	CConstBuffer();
	virtual ~CConstBuffer();
};

