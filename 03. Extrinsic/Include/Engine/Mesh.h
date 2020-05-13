#pragma once
#include "Resource.h"
class CMesh :
	public CResource
{
public:
	CMesh();
	~CMesh();

private:
	ID3D11Buffer*	m_pVB; // ���� ������ �ѱ� ����.
	ID3D11Buffer*	m_pIB; // �ε��� ������ �ѱ� ����
	ID3D11InputLayout* m_pLayout; // �� Mesh���� �ѱ�� �;��ϴ� ������ �ٸ� ���̴�.
	// �׷���, �� ���¿� �´� layout�� �־�� �ϸ�, �� mesh�� ����ϱ� ���� VertexShader������ ���̰� ���� ���̴�.

	// Mesh�� ���� ������ �ٸ��� -> layout�� ������ �ٸ��� -> layout�� ������ ���� ����ü�� ���ڷ� �޴� shader�Լ��� 
	// �޶�����.

	UINT			m_iVtxSize;		
	UINT			m_iVtxCount;	
	D3D11_USAGE		m_eVtxUsage;	// ������ ��� ����

	DXGI_FORMAT		m_eIdxFormat;	// �ε��� ���ۿ��� ����� ũ�� ������ enum���̴�.
	UINT			m_iIdxCount;	
	D3D11_USAGE		m_eIdxUsage;	// �ε��� ��� ����

	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology; 
	vector<D3D11_INPUT_ELEMENT_DESC> m_vecLayout; // layout ����ü�� ������ �־�ΰ�, �̰��� �ּҸ� �ѱ����  vector�� ����.

	void*					m_pVtxSysMem;
	void*					m_pIdxSysMem;

public:
	void Create(void* pVtxMem, UINT iVtxSize, UINT iVtxCount, D3D11_USAGE eVtxUsage,
		void* pIdxMem, DXGI_FORMAT eIdxFormat, UINT iIdxCount, D3D11_USAGE eIdxUsage, D3D11_PRIMITIVE_TOPOLOGY eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	void Render();
	

};

