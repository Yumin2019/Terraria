#pragma once
#include "Resource.h"

#include "Ptr.h" // 스마트 포인터를 사용하는 쪽에서 include 
#include "Shader.h"
#include "Texture.h"

class CMaterial :
	public CResource
{
public:
	CMaterial();
	virtual ~CMaterial();


private:
	MtrlParam		m_tParam;
	Ptr<CShader>	m_pShader;

	Ptr<CTexture>	m_pTexArray[(UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0];
	bool			m_bFileSave;


public:
	void SetShader(Ptr<CShader> pShader);
	void UpdateData();
	void SetData(SHADER_PARAM eParam, void* pData);
	void DisableFileSave() { m_bFileSave = false; }

	virtual CMaterial* Clone();

	MtrlParam& GetMtrlData() { return m_tParam; }
	Ptr<CTexture> GetTexture(int idx) { return m_pTexArray[idx]; }

	virtual void Load(const wstring& strFullPath);
	virtual void Save(const wstring& strPath);

public:
	Ptr<CShader> GetShader() const { return m_pShader; }

	
};

