#pragma once
#include "Resource.h"
class CMesh :
	public CResource
{
public:
	CMesh();
	~CMesh();

private:
	ID3D11Buffer*	m_pVB; // 정점 정보를 넘길 버퍼.
	ID3D11Buffer*	m_pIB; // 인덱스 정보를 넘길 버퍼
	ID3D11InputLayout* m_pLayout; // 각 Mesh에서 넘기고 싶어하는 정보가 다를 것이다.
	// 그러면, 그 형태에 맞는 layout이 있어야 하며, 이 mesh를 출력하기 위한 VertexShader에서도 차이가 생길 것이다.

	// Mesh의 전달 정보가 다르다 -> layout의 정보가 다르다 -> layout의 정보에 맞춘 구조체를 인자로 받는 shader함수가 
	// 달라진다.

	UINT			m_iVtxSize;		
	UINT			m_iVtxCount;	
	D3D11_USAGE		m_eVtxUsage;	// 정점의 사용 권한

	DXGI_FORMAT		m_eIdxFormat;	// 인덱스 버퍼에서 사용할 크기 설정의 enum문이다.
	UINT			m_iIdxCount;	
	D3D11_USAGE		m_eIdxUsage;	// 인덱스 사용 권한

	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology; 
	vector<D3D11_INPUT_ELEMENT_DESC> m_vecLayout; // layout 구조체를 여러개 넣어두고, 이것의 주소를 넘기려고  vector로 선언.

	void*					m_pVtxSysMem;
	void*					m_pIdxSysMem;

public:
	void Create(void* pVtxMem, UINT iVtxSize, UINT iVtxCount, D3D11_USAGE eVtxUsage,
		void* pIdxMem, DXGI_FORMAT eIdxFormat, UINT iIdxCount, D3D11_USAGE eIdxUsage, D3D11_PRIMITIVE_TOPOLOGY eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	void Render();
	

};

