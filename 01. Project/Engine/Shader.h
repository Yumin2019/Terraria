#pragma once
#include "Resource.h"

// 이 구조체는 Param정보를 string과 enum문을 통해서 나타내는데 이 정보가 필요한 이유는 나중에
// 툴과 연동하는 상황에서 시각적으로 필요한 정보만을 수정할 수 있도록 하기 위해서 이다.

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

	// Shader 클래스를 통해서 여러 Shader를 설정하여 하나의 묶음으로 관리할 수 있는 시스템으로 만든다.

private:
	ID3D11VertexShader*			m_pVS;
	ID3D11HullShader*			m_pHS;
	ID3D11DomainShader*			m_pDS;
	ID3D11GeometryShader*		m_pGS;
	ID3D11PixelShader*			m_pPS;

	// Blob 정보는 Shader를 만드는데 사용된다.
	ID3DBlob*					m_pVSBlob; // VSBlob을 이용해서 layout의 정보를 만든다. (참고)
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

	void UpdateData(); // Shader Setting 함수.

	ID3DBlob* GetVtxBlob() const { return m_pVSBlob; }
	

	void AddShaderParam(const ShaderParam& tParam) { m_vecShaderParam.push_back(tParam); }

	void SetBlendState(CBlendState* pState) { m_pBlendState = pState; }


	
};

