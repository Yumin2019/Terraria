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
	// DirectX ���̺귯�� ��ü���� Ư¡. �ַ� �����͸� �̿��ؼ� �Ҵ��� ��û�ϴ� ���°� ������ ����.
	// ������Ʈ���� ����ϴ� �ּ��� ü��� DirectX���� ����ϴ� �ּ��� ü�谡 �ٸ� ���ɼ��� �ֱ� ������
	// ��� ���� �������� ���߱⸸ �ϸ� �ȴ�. ����, DirectX ���̺귯�������� ��ü�� �Ҵ��ϴ� �ڵ尡 ������ �Ǿ� �ְ�,
	// ���� ������ �ϴ� �κб��� ������ �Ǿ��ִ�.
	// ���� �츮�� DirectX ���̺귯���� ���ؼ� �Ҵ��� �޴� �Լ��� ȣ���Ͽ� ��ü�� ����ϰ�, �� ���Ŀ��� �ٽ� ���̺귯���� �Լ��� �̿��ؼ�
	// ���ҽ��� ��ȯ�ϴ� �۾��� ���ľ� �Ѵ�.

	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pContext;

	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView*	m_pRenderTargetView;

	ID3D11Texture2D*		m_pDepthStencilTex;
	ID3D11DepthStencilView* m_pDepthStencilView;

	D3D11_VIEWPORT			m_tViewPortDesc;
	ID3D11RasterizerState*	m_pRasterizerState[4];


	map<wstring, CConstBuffer*>		m_mapCB; // ConstBuffer* �� map�� ���ؼ� �����ϰ� �ִ�.

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
	// Key, ����� ũ��, ����� �ѱ� ���������� ���ڸ� �Ѱ��ָ� �� �����͸� ����
	// ��� ���۸� �����ϰ�, �̰��� map�� ��Ƶδ� �Լ��̴�.
	CConstBuffer* FindConstBuffer(const wstring &strKey);

private:
	void CreateSwapChain();
	void CreateView();
	void CreateViewPort();


};

