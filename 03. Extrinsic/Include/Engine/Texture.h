#pragma once
#include "Resource.h"

#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex/DirectXTex.lib")
#endif

class CTexture :
	public CResource
{
public:
	CTexture();
	virtual ~CTexture();


private:
	ScratchImage				m_Image;
	ID3D11Texture2D*			m_pTex2D;

	ID3D11ShaderResourceView*	m_pSRV;
	ID3D11RenderTargetView*		m_pRTV;
	ID3D11DepthStencilView*		m_pDSV;

	// 텍스처를 사용하는 용도에 따라서,
	// Shader의 리소스로 사용이 되거나,
	// RenderTarget으로 사용이되고,
	// 깊이 스텐실 용도로 사용이 될 가능성이 있다.

public:
	virtual void Load(const wstring& strFullPath);
	float GetWidth() const { return (float)m_Image.GetMetadata().width; }
	float GetHeight() const { return (float)m_Image.GetMetadata().height; }
	const Vec2 GetLength() const { return Vec2(GetWidth(), GetHeight()); }



	void UpdateData(UINT iShaderType, UINT iRegisterNum);
	static void ClearData(UINT iShaderType, UINT iRegisterNum);
};

