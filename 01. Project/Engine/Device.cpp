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
	// Release모드인 경우에는 flag를 주지 않고, Debug모드인 경우에만 디버그 모드 전용의 flag를 주도록 한다.
#endif

	D3D_FEATURE_LEVEL eLv = (D3D_FEATURE_LEVEL)0;

	/*
	HRESULT D3D11CreateDevice(
  IDXGIAdapter            *pAdapter, 장치를 만들 때 사용할 비디오 어댑터의 포인터 . 패스 NULL을 열거되는 첫 번째 어댑터입니다
  D3D_DRIVER_TYPE         DriverType,  만들 수있는 드라이버 유형을 나타냅니다.
  HMODULE                 Software, 소프트웨어 래스터 라이저를 구현하는 DLL에 대한 핸들입니다
  UINT                    Flags, 활성화 할 런타임 계층 ( D3D11_CREATE_DEVICE_FLAG 참조 ); 값을 비트 단위로 OR 할 수 있습니다.

  const D3D_FEATURE_LEVEL *pFeatureLevels, 
  작성하려는 기능 레벨의 순서를 결정하는 D3D_FEATURE_LEVEL 의 배열에 대한 포인터
  . 경우 pFeatureLevels가 설정되어 NULL ,이 기능은 기능 수준의 다음과 같은 배열을 사용합니다 
  // 선택하고 싶은 Feature level을 넣어놓은 배열의 주소를 넘겨주면 된다.

  UINT                    FeatureLevels, pFeatureLevels 의 요소 수입니다 .
  UINT                    SDKVersion, SDK 버전; D3D11_SDK_VERSION을 사용 하십시오 .

  ID3D11Device            **ppDevice, 
  생성 된 장치를 나타내는 ID3D11Device 객체 에 대한 포인터의 주소를 반환합니다 . 이 매개 변수가 NULL 이면 ID3D11Device가 반환되지 않습니다.

  D3D_FEATURE_LEVEL       *pFeatureLevel, 
  성공하면 성공한 pFeatureLevels 배열 에서 첫 번째 D3D_FEATURE_LEVEL 을 반환합니다 . NULL :  지원되는 기능 수준을 결정 필요가없는 경우 입력한다.
 
 해당 변수의 주소를 넘겨줘서 이곳에 들어오는 값을 통해서 제대로 설정이 되었는지 확인할 수 있다. 그래서 eLv 변수를 생성하여 이것의 주소를 넘겨준 것이다.

  ID3D11DeviceContext     **ppImmediateContext

  장치 컨텍스트를 나타내는 ID3D11DeviceContext 개체 에 대한 포인터의 주소를 반환합니다 . 
  이 매개 변수가 NULL 이면 ID3D11DeviceContext가 반환되지 않습니다.
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
		MessageBox(nullptr, L"Sample Count 4 Level 지원 안됨", L"Device 초기화 실패", MB_OK);

		return false;
	}

	// SwapChain을 생성한다.
	CreateSwapChain();

	// View를 생성한다.
	CreateView();

	// ViewPort를 생성한다.
	CreateViewPort();


	return true;
}

void CDevice::Clear(float(&floatArray)[4])
{
	// Render Target View 를 Clear한다.
	m_pContext->ClearRenderTargetView(m_pRenderTargetView, floatArray);

	// Depth Stencil View 를 Clear한다.
	m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	// 조건 a | 조건 b 서로 다른 비트를 사용하여 depth 값도 없애고, stencil값도 없애고...
	// 깊이는 1.f(가장 멀리로 줘야 그 앞에 물체를 출력할 수 있다)
}

void CDevice::CreateConstBuffer(const wstring & strKey, UINT iSize, UINT iRegisterNum)
{
	CConstBuffer* pCB = FindConstBuffer(strKey);

	if (pCB != nullptr)
		assert(nullptr);
	// 생성을 하려고 하는데 이미 해당 이름을 가진 
	//버퍼가 존재한다면 assert


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



	
	// 출력 윈도우를 지정하여, 화면 갱신역할을 담당한다.
	// 2개의 버퍼를 사용하는데, 출력하고 있는 버퍼를 Front 출력하고 있지 않은 것을 Back을 설정한다.
	// 만약에 출력할 버퍼를 변경해야 하는 순간이 오지 않았다면, 그대로 Front버퍼를 계속 출력한다.
	// 이후에 Front를 출력한 이후에는 이 버퍼가 Back으로 변하고 Back은 Front버퍼가 되어
	// 역할이 바뀌게 된다.

	DXGI_SWAP_CHAIN_DESC tDesc = {};

	tDesc.BufferCount = 1; // 1을 넣어준다. 

	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 버퍼의 픽셀 포멧을 지정한다. RGBA 4byte
	tDesc.BufferDesc.Width = (UINT)m_tResolution.fWidth;
	tDesc.BufferDesc.Height = (UINT)m_tResolution.fHeight; // 버퍼의 해상도를 의미한다. 일반적으로 1 : 1 의 비율을 맞추며
	// 이것이 맞지 않는 경우에 확대시에 픽셀이 깨지는 현상이 일어난다.

	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED; // 확대 하거나 축소한다. (기본값)
	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // 픽셀의 스캔 라인의 순서를 의미한다.
	// 어느 방향부터 픽셀을 시작할 것인지 위에서 부터 아래에서 부터 ... 이런 정보를 설정하는 것이다. (기본값)

	tDesc.BufferDesc.RefreshRate.Denominator = 60;
	tDesc.BufferDesc.RefreshRate.Numerator = 1;
	// 화면의 갱신 비율을 의미한다. 이렇게 설정하면 1초에 60번 처리를 하겠다는 의미이다.

	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 출력 타겟의 용도로 버퍼를 만든다.
	tDesc.Flags = 0;

	tDesc.OutputWindow = m_hWnd; // 출력 윈도우를 넣어준다.
	tDesc.Windowed = m_bWindowed; // 윈도우의 모드 설정.

	// 멀티 샘플 : 픽셀하나를 검사하기 위해서 주변 픽셀을 검사하는 것이다. 이 수치가 높을 수록 주변에서 많은 수의 픽셀을 
	// 검사하여 많은 자원을 사용하게 된다. (그 만큼 정확하긴 하겠지만.. 굳이 할 필요는 없으니 하지 않는다 = 1 로 설정한다)
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD; // Front와 Back을 스왑하는 경우에 이전에 사용하던 프레임에 정보를 어떻게 할 것인지
	// 정한다.  버리는 것으로 설정.

	IDXGIDevice* pDXGIDevice = nullptr; // IDXGIDevice의 인터페이스는 다른 DXGI 개체에 대한 액세스를 필요로 DXGI 오브젝트에 의해 사용하도록 설계되었습니다. 
	IDXGIAdapter* pAdapter = nullptr;
	IDXGIFactory* pFactory = nullptr;

	// SwapChain을 생성하는 함수를 호출하기 위해서 IDXGIDevice -> IDXGIApapter -> IDXGIFactory의 과정을 거친다.

	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice); // Device 를 통해서 IDXGIDevice를 생성한다.
	pDXGIDevice->GetAdapter(&pAdapter); // IDXGIDevice 를 통해서 Adapter를 얻어온다.
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory); // Adapter -> Factory

	pFactory->CreateSwapChain(m_pDevice, &tDesc, &m_pSwapChain);

	// 스왑체인을 생성한 이후에는 IDXGI .. 들의 ref값을 감소시킨다.

	SAFE_RELEASE(pDXGIDevice);
	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pFactory);

	

}

void CDevice::CreateView()
{
	// Output-Merger 단계 (출력 병합기)
	// OM(출력 병합기) 단계는 다양한 유형의 출력 데이터(픽셀 셰이더 값, 깊이 및 스텐실 정보)를
	//렌더링 대상 및 깊이/스텐실 버퍼의 내용과 결합하여 최종 파이프라인 결과를 생성합니다.
	// Render Target View + Depth Stencil View


	// 주로 Buffer의 형태로 GPU에 정보를 전달하는데, Output Merger 단계에서 View라는 형태를 가지고 처리를 하게 된다.
	// View는 전달자의 개념이다. 리소스를 가리켜서 전달하는 용도로 사용한다.

	// Render Target View

	// BackBuffer는 SwapChain이 가지고 있다.
	ID3D11Texture2D* pTex2D = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pTex2D); // 1
	SAFE_RELEASE(pTex2D); // 0

	// Device를 가지고 RenderTargetView를 생성하며, Buffer(SwapChain's BackBuffer) 를 가리키도록 한다
	m_pDevice->CreateRenderTargetView(pTex2D, nullptr, &m_pRenderTargetView);



	// Depth Stencil View 
	
	// 깊이 스텐실 버퍼는 물체의 깊이 값을 저장하는 버퍼이다. 
	// 그리기 전에 자신의 위치에 출력된 물체와 깊이 값을 비교하여 그리는 처리를 한다.
	// 깊이 버퍼에서는 거리에 따른, 숨김 효과를 제어하며,
	// 스텐실 버퍼에서는 특수효과에 관한 처리를 한다고 한다.

	D3D11_TEXTURE2D_DESC tDesc = {};

	// Depth Stencil 용도로 Texture2D를 이용한다.

	/*
	typedef struct D3D11_TEXTURE2D_DESC {
  UINT             Width;
  UINT             Height;
  UINT             MipLevels; 텍스처에서 최대 밉맵 레벨 수입니다
  UINT             ArraySize; 텍스처 배열의 텍스처 수
  DXGI_FORMAT      Format;
  DXGI_SAMPLE_DESC SampleDesc;
  D3D11_USAGE      Usage; 텍스처를 읽고 쓰는 방법
  UINT             BindFlags;
  UINT             CPUAccessFlags;
  UINT             MiscFlags;
} D3D11_TEXTURE2D_DESC;
	*/

	tDesc.ArraySize = 1;
	tDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL; // Depth Stencil 용도로 사용한다.

	tDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	tDesc.CPUAccessFlags = 0;

	tDesc.Width = (UINT)m_tResolution.fWidth;
	tDesc.Height = (UINT)m_tResolution.fHeight;

	tDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tDesc.MipLevels = 1; 

	tDesc.SampleDesc.Count = 1; // 주변 픽셀 검사 안 함.
	tDesc.SampleDesc.Quality = 0;
	
	m_pDevice->CreateTexture2D(&tDesc, nullptr, &m_pDepthStencilTex); 
	// 위에서 설정한 Texture 정보를 통해서 초기값 없이 Depth Stencil Texture를 만들어낸다.

	// 해당 Buffer를 가리키는 View ( Depth Stencil View를 만든다)

	m_pDevice->CreateDepthStencilView(m_pDepthStencilTex, nullptr, &m_pDepthStencilView);
	
	m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
}

void CDevice::CreateViewPort()
{

	// ViewPort 

	// 그려질 것을 감안해서 그리는 설정을한다. 수식을 잡아주는 기능이다.
	// 완성을 한 것을 축소하는 것이 아닌, 이것을 토대로 축소한 것을 그리는 것이다.

	/*
	뷰포트(viewport)는 컴퓨터 그래픽스에서 영역을 표시하는 폴리곤이다.
	뷰포트는 렌더링 장치에 특화된 좌표로 표현되는 영역으로(일반적으로 직사각형), 
	이를테면 화면 좌표의 화소로 이야기할 수 있으며 이 안에서 관심 대상의 객체가 렌더링 대상이 된다.

	*/
	m_tViewPortDesc.TopLeftX = 0.f;
	m_tViewPortDesc.TopLeftY = 0.f;
	m_tViewPortDesc.Width = m_tResolution.fWidth;
	m_tViewPortDesc.Height = m_tResolution.fHeight;
	m_tViewPortDesc.MinDepth = 0.f;
	m_tViewPortDesc.MaxDepth = 1.f;

	// 뷰포트의 지정을 화면크기로 설정.
	// Min Max 깊이 값은 0.f ~ 1.f 로 고정한다.

	m_pContext->RSSetViewports(1, &m_tViewPortDesc);


}
