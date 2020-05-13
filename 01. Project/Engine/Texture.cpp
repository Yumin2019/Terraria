#include "stdafx.h"
#include "Texture.h"
#include "Device.h"

CTexture::CTexture() :
	CResource(RES_TYPE::TEXTURE)
{
}


CTexture::~CTexture()
{
	/*ScratchImage				m_Image;
	ID3D11Texture2D*			m_pTex2D;

	ID3D11ShaderResourceView*	m_pSRV;
	ID3D11RenderTargetView*		m_pRTV;
	ID3D11DepthStencilView*		m_pDSV;*/

	SAFE_RELEASE(m_pTex2D);
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pRTV);
	SAFE_RELEASE(m_pDSV);

	// m_Image 객체를 멤버로 받아서 처리하는 까닭은.
	// 정적 라이브러리의 경우에는 코드 자체가 
	// 포함이 되는 개념이기 때문에 사용하는 메모리
	// 체계가 동일하다. 
	// 따라서, 객체의 소멸함수를 사용하지 않아도 된다.

}

void CTexture::Load(const wstring & strFullPath)
{
	wchar_t szExt[50] = L"";
	_wsplitpath_s(strFullPath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50);

	wstring strExt = szExt;

	HRESULT hResult = S_OK;

	if (strExt == L".dds" || strExt == L".DDS")
	{
		hResult = LoadFromDDSFile(strFullPath.c_str(), DDS_FLAGS_NONE, nullptr, m_Image);
	}

	else if (strExt == L".tag" || strExt == L".TGA")
	{
		hResult = LoadFromTGAFile(strFullPath.c_str(), nullptr, m_Image);
	}

	else // png, jpg, jpeg, bmp 등의 파일은 WIC 파일로 하나의 함수로 읽어올 수 있다.
	{
		hResult = LoadFromWICFile(strFullPath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);
	}

	if (hResult == S_FALSE)
		assert(nullptr);

	CreateShaderResourceView(DEVICE, m_Image.GetImages(), m_Image.GetImageCount(), m_Image.GetMetadata(), &m_pSRV);
	m_pSRV->GetResource((ID3D11Resource**)&m_pTex2D);


}

void CTexture::UpdateData(UINT iShaderType, UINT iRegisterNum)
{
	if ((UINT)SHADER_TYPE::ST_VERTEX &  iShaderType)
		CONTEXT->VSSetShaderResources(iRegisterNum, 1, &m_pSRV);
	if ((UINT)SHADER_TYPE::ST_HULL &  iShaderType)
		CONTEXT->HSSetShaderResources(iRegisterNum, 1, &m_pSRV);
	if ((UINT)SHADER_TYPE::ST_DOMAIN &  iShaderType)
		CONTEXT->DSSetShaderResources(iRegisterNum, 1, &m_pSRV);
	if ((UINT)SHADER_TYPE::ST_GEOMETRY & iShaderType)
		CONTEXT->GSSetShaderResources(iRegisterNum, 1, &m_pSRV);
	if ((UINT)SHADER_TYPE::ST_COMPUTE &  iShaderType)
		CONTEXT->CSSetShaderResources(iRegisterNum, 1, &m_pSRV);
	if ((UINT)SHADER_TYPE::ST_PIXEL &  iShaderType)
		CONTEXT->PSSetShaderResources(iRegisterNum, 1, &m_pSRV);
}

void CTexture::ClearData(UINT iShaderType, UINT iRegisterNum)
{
	ID3D11ShaderResourceView* pSRV = nullptr;

	if ((UINT)SHADER_TYPE::ST_VERTEX &  iShaderType)
		CONTEXT->VSSetShaderResources(iRegisterNum, 1, &pSRV);
	if ((UINT)SHADER_TYPE::ST_HULL &  iShaderType)
		CONTEXT->HSSetShaderResources(iRegisterNum, 1, &pSRV);
	if ((UINT)SHADER_TYPE::ST_DOMAIN &  iShaderType)
		CONTEXT->DSSetShaderResources(iRegisterNum, 1, &pSRV);
	if ((UINT)SHADER_TYPE::ST_GEOMETRY & iShaderType)
		CONTEXT->GSSetShaderResources(iRegisterNum, 1, &pSRV);
	if ((UINT)SHADER_TYPE::ST_COMPUTE &  iShaderType)
		CONTEXT->CSSetShaderResources(iRegisterNum, 1, &pSRV);
	if ((UINT)SHADER_TYPE::ST_PIXEL &  iShaderType)
		CONTEXT->PSSetShaderResources(iRegisterNum, 1, &pSRV);
}
