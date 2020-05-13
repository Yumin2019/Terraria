#include "stdafx.h"
#include "Device.h"

#include "ConstBuffer.h"



CDevice::CDevice() :
	m_pDevice(nullptr),
	m_pContext(nullptr),
	m_pSwapChain(nullptr),
	m_pRenderTargetView(nullptr),
	m_pDepthStencilTex(nullptr),
	m_pDepthStencilView(nullptr)
{
}


CDevice::~CDevice()
{

	/*
	m_pDevice;
	m_pContext;
	
	m_pSwapChain;
	m_pRenderTargetView;
	
	m_pDepthStencilTex;
	m_pDepthStencilView;
	
	m_pViewPortDesc;
	m_pRasterizerState[4];
	
	m_pSamplerState;
	*/

	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pContext);

	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pRenderTargetView);

	SAFE_RELEASE(m_pDepthStencilTex);
	SAFE_RELEASE(m_pDepthStencilView);


	Safe_Delete_Map(m_mapCB);

}

bool CDevice::Init(HWND hWnd, const Resolution & tResolution, bool bWindowed)
{
	m_hWnd = hWnd;
	m_tResolution = tResolution;
	m_bWindowed = bWindowed;

	UINT iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
	// Release����� ��쿡�� flag�� ���� �ʰ�, Debug����� ��쿡�� ����� ��� ������ flag�� �ֵ��� �Ѵ�.
#endif

	D3D_FEATURE_LEVEL eLv = (D3D_FEATURE_LEVEL)0;

	/*
	HRESULT D3D11CreateDevice(
  IDXGIAdapter            *pAdapter, ��ġ�� ���� �� ����� ���� ������� ������ . �н� NULL�� ���ŵǴ� ù ��° ������Դϴ�
  D3D_DRIVER_TYPE         DriverType,  ���� ���ִ� ����̹� ������ ��Ÿ���ϴ�.
  HMODULE                 Software, ����Ʈ���� ������ �������� �����ϴ� DLL�� ���� �ڵ��Դϴ�
  UINT                    Flags, Ȱ��ȭ �� ��Ÿ�� ���� ( D3D11_CREATE_DEVICE_FLAG ���� ); ���� ��Ʈ ������ OR �� �� �ֽ��ϴ�.

  const D3D_FEATURE_LEVEL *pFeatureLevels, 
  �ۼ��Ϸ��� ��� ������ ������ �����ϴ� D3D_FEATURE_LEVEL �� �迭�� ���� ������
  . ��� pFeatureLevels�� �����Ǿ� NULL ,�� ����� ��� ������ ������ ���� �迭�� ����մϴ� 
  // �����ϰ� ���� Feature level�� �־���� �迭�� �ּҸ� �Ѱ��ָ� �ȴ�.

  UINT                    FeatureLevels, pFeatureLevels �� ��� ���Դϴ� .
  UINT                    SDKVersion, SDK ����; D3D11_SDK_VERSION�� ��� �Ͻʽÿ� .

  ID3D11Device            **ppDevice, 
  ���� �� ��ġ�� ��Ÿ���� ID3D11Device ��ü �� ���� �������� �ּҸ� ��ȯ�մϴ� . �� �Ű� ������ NULL �̸� ID3D11Device�� ��ȯ���� �ʽ��ϴ�.

  D3D_FEATURE_LEVEL       *pFeatureLevel, 
  �����ϸ� ������ pFeatureLevels �迭 ���� ù ��° D3D_FEATURE_LEVEL �� ��ȯ�մϴ� . NULL :  �����Ǵ� ��� ������ ���� �ʿ䰡���� ��� �Է��Ѵ�.
 
 �ش� ������ �ּҸ� �Ѱ��༭ �̰��� ������ ���� ���ؼ� ����� ������ �Ǿ����� Ȯ���� �� �ִ�. �׷��� eLv ������ �����Ͽ� �̰��� �ּҸ� �Ѱ��� ���̴�.

  ID3D11DeviceContext     **ppImmediateContext

  ��ġ ���ؽ�Ʈ�� ��Ÿ���� ID3D11DeviceContext ��ü �� ���� �������� �ּҸ� ��ȯ�մϴ� . 
  �� �Ű� ������ NULL �̸� ID3D11DeviceContext�� ��ȯ���� �ʽ��ϴ�.
);
	*/
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		iFlag, nullptr, 0,
		D3D11_SDK_VERSION,
		&m_pDevice,
		&eLv,
		&m_pContext);

	UINT iQualityLevel = 0;
	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &iQualityLevel);

	if (iQualityLevel == 0)
	{
		MessageBox(nullptr, L"Sample Count 4 Level ���� �ȵ�", L"Device �ʱ�ȭ ����", MB_OK);

		return false;
	}

	// SwapChain�� �����Ѵ�.
	CreateSwapChain();

	// View�� �����Ѵ�.
	CreateView();

	// ViewPort�� �����Ѵ�.
	CreateViewPort();


	return true;
}

void CDevice::Clear(float(&floatArray)[4])
{
	// Render Target View �� Clear�Ѵ�.
	m_pContext->ClearRenderTargetView(m_pRenderTargetView, floatArray);

	// Depth Stencil View �� Clear�Ѵ�.
	m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	// ���� a | ���� b ���� �ٸ� ��Ʈ�� ����Ͽ� depth ���� ���ְ�, stencil���� ���ְ�...
	// ���̴� 1.f(���� �ָ��� ��� �� �տ� ��ü�� ����� �� �ִ�)
}

void CDevice::CreateConstBuffer(const wstring & strKey, UINT iSize, UINT iRegisterNum)
{
	CConstBuffer* pCB = FindConstBuffer(strKey);

	if (pCB != nullptr)
		assert(nullptr);
	// ������ �Ϸ��� �ϴµ� �̹� �ش� �̸��� ���� 
	//���۰� �����Ѵٸ� assert


	pCB = new CConstBuffer;
	pCB->Create(iSize, iRegisterNum, strKey);
	m_mapCB.insert(make_pair(strKey, pCB));
}

CConstBuffer * CDevice::FindConstBuffer(const wstring & strKey)
{
	map<wstring, CConstBuffer*>::iterator iter = m_mapCB.find(strKey);

	if(iter == m_mapCB.end())
		return nullptr;

	return iter->second;
}

void CDevice::CreateSwapChain()
{



	
	// ��� �����츦 �����Ͽ�, ȭ�� ���ſ����� ����Ѵ�.
	// 2���� ���۸� ����ϴµ�, ����ϰ� �ִ� ���۸� Front ����ϰ� ���� ���� ���� Back�� �����Ѵ�.
	// ���࿡ ����� ���۸� �����ؾ� �ϴ� ������ ���� �ʾҴٸ�, �״�� Front���۸� ��� ����Ѵ�.
	// ���Ŀ� Front�� ����� ���Ŀ��� �� ���۰� Back���� ���ϰ� Back�� Front���۰� �Ǿ�
	// ������ �ٲ�� �ȴ�.

	DXGI_SWAP_CHAIN_DESC tDesc = {};

	tDesc.BufferCount = 1; // 1�� �־��ش�. 

	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // ������ �ȼ� ������ �����Ѵ�. RGBA 4byte
	tDesc.BufferDesc.Width = (UINT)m_tResolution.fWidth;
	tDesc.BufferDesc.Height = (UINT)m_tResolution.fHeight; // ������ �ػ󵵸� �ǹ��Ѵ�. �Ϲ������� 1 : 1 �� ������ ���߸�
	// �̰��� ���� �ʴ� ��쿡 Ȯ��ÿ� �ȼ��� ������ ������ �Ͼ��.

	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED; // Ȯ�� �ϰų� ����Ѵ�. (�⺻��)
	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // �ȼ��� ��ĵ ������ ������ �ǹ��Ѵ�.
	// ��� ������� �ȼ��� ������ ������ ������ ���� �Ʒ����� ���� ... �̷� ������ �����ϴ� ���̴�. (�⺻��)

	tDesc.BufferDesc.RefreshRate.Denominator = 60;
	tDesc.BufferDesc.RefreshRate.Numerator = 1;
	// ȭ���� ���� ������ �ǹ��Ѵ�. �̷��� �����ϸ� 1�ʿ� 60�� ó���� �ϰڴٴ� �ǹ��̴�.

	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ��� Ÿ���� �뵵�� ���۸� �����.
	tDesc.Flags = 0;

	tDesc.OutputWindow = m_hWnd; // ��� �����츦 �־��ش�.
	tDesc.Windowed = m_bWindowed; // �������� ��� ����.

	// ��Ƽ ���� : �ȼ��ϳ��� �˻��ϱ� ���ؼ� �ֺ� �ȼ��� �˻��ϴ� ���̴�. �� ��ġ�� ���� ���� �ֺ����� ���� ���� �ȼ��� 
	// �˻��Ͽ� ���� �ڿ��� ����ϰ� �ȴ�. (�� ��ŭ ��Ȯ�ϱ� �ϰ�����.. ���� �� �ʿ�� ������ ���� �ʴ´� = 1 �� �����Ѵ�)
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD; // Front�� Back�� �����ϴ� ��쿡 ������ ����ϴ� �����ӿ� ������ ��� �� ������
	// ���Ѵ�.  ������ ������ ����.

	IDXGIDevice* pDXGIDevice = nullptr; // IDXGIDevice�� �������̽��� �ٸ� DXGI ��ü�� ���� �׼����� �ʿ�� DXGI ������Ʈ�� ���� ����ϵ��� ����Ǿ����ϴ�. 
	IDXGIAdapter* pAdapter = nullptr;
	IDXGIFactory* pFactory = nullptr;

	// SwapChain�� �����ϴ� �Լ��� ȣ���ϱ� ���ؼ� IDXGIDevice -> IDXGIApapter -> IDXGIFactory�� ������ ��ģ��.

	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice); // Device �� ���ؼ� IDXGIDevice�� �����Ѵ�.
	pDXGIDevice->GetAdapter(&pAdapter); // IDXGIDevice �� ���ؼ� Adapter�� ���´�.
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory); // Adapter -> Factory

	pFactory->CreateSwapChain(m_pDevice, &tDesc, &m_pSwapChain);

	// ����ü���� ������ ���Ŀ��� IDXGI .. ���� ref���� ���ҽ�Ų��.

	SAFE_RELEASE(pDXGIDevice);
	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pFactory);

	

}

void CDevice::CreateView()
{
	// Output-Merger �ܰ� (��� ���ձ�)
	// OM(��� ���ձ�) �ܰ�� �پ��� ������ ��� ������(�ȼ� ���̴� ��, ���� �� ���ٽ� ����)��
	//������ ��� �� ����/���ٽ� ������ ����� �����Ͽ� ���� ���������� ����� �����մϴ�.
	// Render Target View + Depth Stencil View


	// �ַ� Buffer�� ���·� GPU�� ������ �����ϴµ�, Output Merger �ܰ迡�� View��� ���¸� ������ ó���� �ϰ� �ȴ�.
	// View�� �������� �����̴�. ���ҽ��� �����Ѽ� �����ϴ� �뵵�� ����Ѵ�.

	// Render Target View

	// BackBuffer�� SwapChain�� ������ �ִ�.
	ID3D11Texture2D* pTex2D = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pTex2D); // 1
	SAFE_RELEASE(pTex2D); // 0

	// Device�� ������ RenderTargetView�� �����ϸ�, Buffer(SwapChain's BackBuffer) �� ����Ű���� �Ѵ�
	m_pDevice->CreateRenderTargetView(pTex2D, nullptr, &m_pRenderTargetView);



	// Depth Stencil View 
	
	// ���� ���ٽ� ���۴� ��ü�� ���� ���� �����ϴ� �����̴�. 
	// �׸��� ���� �ڽ��� ��ġ�� ��µ� ��ü�� ���� ���� ���Ͽ� �׸��� ó���� �Ѵ�.
	// ���� ���ۿ����� �Ÿ��� ����, ���� ȿ���� �����ϸ�,
	// ���ٽ� ���ۿ����� Ư��ȿ���� ���� ó���� �Ѵٰ� �Ѵ�.

	D3D11_TEXTURE2D_DESC tDesc = {};

	// Depth Stencil �뵵�� Texture2D�� �̿��Ѵ�.

	/*
	typedef struct D3D11_TEXTURE2D_DESC {
  UINT             Width;
  UINT             Height;
  UINT             MipLevels; �ؽ�ó���� �ִ� �Ӹ� ���� ���Դϴ�
  UINT             ArraySize; �ؽ�ó �迭�� �ؽ�ó ��
  DXGI_FORMAT      Format;
  DXGI_SAMPLE_DESC SampleDesc;
  D3D11_USAGE      Usage; �ؽ�ó�� �а� ���� ���
  UINT             BindFlags;
  UINT             CPUAccessFlags;
  UINT             MiscFlags;
} D3D11_TEXTURE2D_DESC;
	*/

	tDesc.ArraySize = 1;
	tDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL; // Depth Stencil �뵵�� ����Ѵ�.

	tDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	tDesc.CPUAccessFlags = 0;

	tDesc.Width = (UINT)m_tResolution.fWidth;
	tDesc.Height = (UINT)m_tResolution.fHeight;

	tDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tDesc.MipLevels = 1; 

	tDesc.SampleDesc.Count = 1; // �ֺ� �ȼ� �˻� �� ��.
	tDesc.SampleDesc.Quality = 0;
	
	m_pDevice->CreateTexture2D(&tDesc, nullptr, &m_pDepthStencilTex); 
	// ������ ������ Texture ������ ���ؼ� �ʱⰪ ���� Depth Stencil Texture�� ������.

	// �ش� Buffer�� ����Ű�� View ( Depth Stencil View�� �����)

	m_pDevice->CreateDepthStencilView(m_pDepthStencilTex, nullptr, &m_pDepthStencilView);
	
	m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
}

void CDevice::CreateViewPort()
{

	// ViewPort 

	// �׷��� ���� �����ؼ� �׸��� �������Ѵ�. ������ ����ִ� ����̴�.
	// �ϼ��� �� ���� ����ϴ� ���� �ƴ�, �̰��� ���� ����� ���� �׸��� ���̴�.

	/*
	����Ʈ(viewport)�� ��ǻ�� �׷��Ƚ����� ������ ǥ���ϴ� �������̴�.
	����Ʈ�� ������ ��ġ�� Ưȭ�� ��ǥ�� ǥ���Ǵ� ��������(�Ϲ������� ���簢��), 
	�̸��׸� ȭ�� ��ǥ�� ȭ�ҷ� �̾߱��� �� ������ �� �ȿ��� ���� ����� ��ü�� ������ ����� �ȴ�.

	*/
	m_tViewPortDesc.TopLeftX = 0.f;
	m_tViewPortDesc.TopLeftY = 0.f;
	m_tViewPortDesc.Width = m_tResolution.fWidth;
	m_tViewPortDesc.Height = m_tResolution.fHeight;
	m_tViewPortDesc.MinDepth = 0.f;
	m_tViewPortDesc.MaxDepth = 1.f;

	// ����Ʈ�� ������ ȭ��ũ��� ����.
	// Min Max ���� ���� 0.f ~ 1.f �� �����Ѵ�.

	m_pContext->RSSetViewports(1, &m_tViewPortDesc);


}
