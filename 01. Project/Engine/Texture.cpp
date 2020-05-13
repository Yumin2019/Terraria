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

	// m_Image ��ü�� ����� �޾Ƽ� ó���ϴ� �����.
	// ���� ���̺귯���� ��쿡�� �ڵ� ��ü�� 
	// ������ �Ǵ� �����̱� ������ ����ϴ� �޸�
	// ü�谡 �����ϴ�. 
	// ����, ��ü�� �Ҹ��Լ��� ������� �ʾƵ� �ȴ�.

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

	else // png, jpg, jpeg, bmp ���� ������ WIC ���Ϸ� �ϳ��� �Լ��� �о�� �� �ִ�.
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
