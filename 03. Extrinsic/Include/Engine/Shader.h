#pragma once
#include "Resource.h"

// �� ����ü�� Param������ string�� enum���� ���ؼ� ��Ÿ���µ� �� ������ �ʿ��� ������ ���߿�
// ���� �����ϴ� ��Ȳ���� �ð������� �ʿ��� �������� ������ �� �ֵ��� �ϱ� ���ؼ� �̴�.

class CBlendState;
class CDepthStencilState;

typedef struct _tagShaderParam
{
	wstring			strParam;
	SHADER_PARAM	eType;

	_tagShaderParam(const wstring& strParam, SHADER_PARAM eType) :
		strParam(strParam),
		eType(eType)
	{
	}

}ShaderParam, *PShaderParam;

class CShader :
	public CResource
{
public:
	CShader();
	virtual ~CShader(); 

public:
	static vector<D3D11_INPUT_ELEMENT_DESC> g_vecLayout;
	static int g_AlignedByteOffset;

	// Shader Ŭ������ ���ؼ� ���� Shader�� �����Ͽ� �ϳ��� �������� ������ �� �ִ� �ý������� �����.

private:
	ID3D11VertexShader*			m_pVS;
	ID3D11HullShader*			m_pHS;
	ID3D11DomainShader*			m_pDS;
	ID3D11GeometryShader*		m_pGS;
	ID3D11PixelShader*			m_pPS;

	// Blob ������ Shader�� ����µ� ���ȴ�.
	ID3DBlob*					m_pVSBlob; // VSBlob�� �̿��ؼ� layout�� ������ �����. (����)
	ID3DBlob*					m_pHSBlob;
	ID3DBlob*					m_pDSBlob;
	ID3DBlob*					m_pGSBlob;
	ID3DBlob*					m_pPSBlob;

	ID3DBlob*					m_pErrBlob;

	ID3D11ComputeShader*		m_pCS;
	ID3DBlob*					m_pCSBlob;

	ID3D11InputLayout*			m_pLayout;
	vector<ShaderParam>			m_vecShaderParam;

	CBlendState*				m_pBlendState;
	CDepthStencilState*			m_pDepthStencilState;

public:
	static void CreateLayoutDesc();

	const vector<ShaderParam>& GetShaderParamVec() const { return m_vecShaderParam; }

public:
	void CreateVertexShader(const wstring& strPath, const string& strFuncName, const string& strhlslVersion);
	void CreatePixelShader(const wstring& strPath, const string& strFuncName, const string& strhlslVersion);

	void UpdateData(); // Shader Setting �Լ�.

	ID3DBlob* GetVtxBlob() const { return m_pVSBlob; }
	

	void AddShaderParam(const ShaderParam& tParam) { m_vecShaderParam.push_back(tParam); }

	void SetBlendState(CBlendState* pState) { m_pBlendState = pState; }


	
};

