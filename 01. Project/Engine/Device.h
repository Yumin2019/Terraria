#pragma once

class CConstBuffer;

class CDevice
{
	SINGLE(CDevice);

public:
	HWND					m_hWnd;
	Resolution				m_tResolution;
	bool					m_bWindowed;

private:
	// DirectX 라이브러리 객체들의 특징. 주로 포인터를 이용해서 할당을 요청하는 형태가 굉장히 많다.
	// 프로젝트에서 사용하는 주소의 체계와 DirectX에서 사용하는 주소의 체계가 다를 가능성이 있기 때문에
	// 어느 쪽의 기준으로 맞추기만 하면 된다. 따라서, DirectX 라이브러리에서는 객체를 할당하는 코드가 구현이 되어 있고,
	// 또한 해제를 하는 부분까지 구현이 되어있다.
	// 따라서 우리는 DirectX 라이브러리를 통해서 할당을 받는 함수를 호출하여 객체를 사용하고, 그 이후에는 다시 라이브러리의 함수를 이용해서
	// 리소스를 반환하는 작업을 거쳐야 한다.

	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pContext;

	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView*	m_pRenderTargetView;

	ID3D11Texture2D*		m_pDepthStencilTex;
	ID3D11DepthStencilView* m_pDepthStencilView;

	D3D11_VIEWPORT			m_tViewPortDesc;
	ID3D11RasterizerState*	m_pRasterizerState[4];


	map<wstring, CConstBuffer*>		m_mapCB; // ConstBuffer* 를 map을 통해서 관리하고 있다.

public:
	HWND GetHandle() const { return m_hWnd; }

public: 
	bool Init(HWND hWnd, const Resolution& tResolution, bool bWindowed);
	ID3D11Device* GetDevice() const { return m_pDevice; }
	ID3D11DeviceContext* GetContext() const { return m_pContext; }
	

	void Clear(float(&floatArray)[4]);
	void Present() { m_pSwapChain->Present(0, 0); }

	Resolution GetResolution() const { return m_tResolution; }
	
	void CreateConstBuffer(const wstring& strKey, UINT iSize, UINT iRegisterNum);
	// Key, 상수의 크기, 상수를 넘길 레지스터의 숫자를 넘겨주면 이 데이터를 토대로
	// 상수 버퍼를 생성하고, 이것을 map에 담아두는 함수이다.
	CConstBuffer* FindConstBuffer(const wstring &strKey);

private:
	void CreateSwapChain();
	void CreateView();
	void CreateViewPort();


};

