#include "stdafx.h"
#include "RenderMgr.h"

#include "Device.h"
#include "BlendState.h"
#include "ConstBuffer.h"
#include "SceneMgr.h"
#include "TimeMgr.h"
#include "DepthStencilState.h"

#include "Camera.h"


//
#include "Scene.h"
#include "Layer.h"

CRenderMgr::CRenderMgr() :
	m_pBlendStateArray{},
	m_pDepthStencilStateArray{},
	m_pSampler0(nullptr),
	m_pSampler1(nullptr)
{
}


CRenderMgr::~CRenderMgr()
{
	Safe_Delete_Array(m_pBlendStateArray);
	Safe_Delete_Array(m_pDepthStencilStateArray);

	SAFE_RELEASE(m_pSampler0);
	SAFE_RELEASE(m_pSampler1);
}

bool CRenderMgr::Init()
{
	CreateSamplerState();

	CreateBlendState();

	CreateDepthStencilState();

	return true;
}

void CRenderMgr::Render()
{
	float arrColor[4] = { 0.6f, 0.6f, 0.6f, 1.f };
	//float arrColor[4] = { 0.f, 0.f, 0.f, 1.f };

	CDevice::GetInst()->Clear(arrColor);

	static CConstBuffer* pGlobalBuffer = CDevice::GetInst()->FindConstBuffer(L"GlobalValue");
	pGlobalBuffer->SetData(&g_tValue);
	pGlobalBuffer->UpdateData(ST_ALL);

	// 광원 정보 업데이트
	UpdateLight2D();

	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->Render();
	}

	/*CScene* pScene = CSceneMgr::GetInst()->GetScene();
	for (size_t i = 0; i < MAX_LAYER; ++i)
	{
		pScene->GetLayer(i)->Render();
	}*/

	// FPS
	CTimeMgr::GetInst()->Render();

	// 출력.
	CDevice::GetInst()->Present();

	m_iLight2DCount = 0;
}

void CRenderMgr::RenderTool()
{
	// 초기화.
	float arrColor[4] = { 0.6f, 0.6f, 0.6f, 1.f };
	CDevice::GetInst()->Clear(arrColor);

	static CConstBuffer* pGlobalBuffer = CDevice::GetInst()->FindConstBuffer(L"GlobalValue");
	pGlobalBuffer->SetData(&g_tValue);
	pGlobalBuffer->UpdateData(ST_ALL);

	// 광원 정보 업데이트
	UpdateLight2D();
	m_iLight2DCount = 0;

	/*
	툴을 이용하는 경우에는 Tool Camera를 사용한다.
	따라서, 물체를 그리는 순서는 
	렌더타겟에 오브젝트를 그리고 Present함수를 호출하여 Wnd에 그리게 되는데,
	Engine에서 Present함수를 호출한 이후에 ToolCamera를 통해서 RenderTarget에 그려봐야
	이미 Wnd에 그린 이후라서 적용이 되지 않는다. 
	*/
}

void CRenderMgr::CreateSamplerState()
{
	// Sampler 하나 만들기

	D3D11_SAMPLER_DESC tSamDesc = {};

	tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.Filter = D3D11_FILTER_ANISOTROPIC;

	// wrap : 바둑판 모양으로 UV 좌표를 처리한다.

	DEVICE->CreateSamplerState(&tSamDesc, &m_pSampler0);
	
	CONTEXT->VSSetSamplers(0, 1, &m_pSampler0); // 0번 레지스터를 사용해서 Sampler를 전달하겠다.
	CONTEXT->HSSetSamplers(0, 1, &m_pSampler0);
	CONTEXT->DSSetSamplers(0, 1, &m_pSampler0);
	CONTEXT->GSSetSamplers(0, 1, &m_pSampler0);
	CONTEXT->CSSetSamplers(0, 1, &m_pSampler0);
	CONTEXT->PSSetSamplers(0, 1, &m_pSampler0);


	tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	// 픽셀 느낌을 살려주는 역할의 Sampler이다.


	DEVICE->CreateSamplerState(&tSamDesc, &m_pSampler1);

	CONTEXT->VSSetSamplers(1, 1, &m_pSampler1); // 1번 레지스터를 사용해서 Sampler를 전달하겠다.
	CONTEXT->HSSetSamplers(1, 1, &m_pSampler1);
	CONTEXT->DSSetSamplers(1, 1, &m_pSampler1);
	CONTEXT->GSSetSamplers(1, 1, &m_pSampler1);
	CONTEXT->CSSetSamplers(1, 1, &m_pSampler1);
	CONTEXT->PSSetSamplers(1, 1, &m_pSampler1);


}

void CRenderMgr::CreateBlendState()
{

	D3D11_BLEND_DESC tDesc = {};

	m_pBlendStateArray[(UINT)BLEND_TYPE::ALPHABLEND] = new CBlendState; // AlphaBlend 목적으로
	// BlendState를 하나 만들어준다.

	tDesc.IndependentBlendEnable = false; // 각 타겟 별로 Blending 방식을 설정한다. 독립적으로 적용하지 않겠다 (일괄적용)
	tDesc.AlphaToCoverageEnable = true; // 멀티 샘플링을 이용하여 정확한 알파값으로 Blending을 하겠다.

	// 알파 테스트는 0과 1로만 구분이 된다.
	// 알파 블렌딩으로 반투명한 물체를 그려 낼 수 있으나, 반투명한 물체들을 정렬해야 할 필요가 있다.
	// 블렌딩 과정에서 이전 색상을 가져오기 위해서 버퍼에 접근해야 하므로 속도가 떨어진다.

	// Alpha To Coverage 옵션을 true로 하게 되면, 이미지의 알파 부분까지 고려하여 출력하는 방식이다.
	// 깊이가 남은 a의 부분을 말한다.

	tDesc.RenderTarget[0].BlendEnable = true;
	tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; 
	// 각 픽셀의 어느 구성요소에 데이터를 쓸 수 있는지 정한다.

	// Render Target Pixel RGB 혼합 공식.
	// Pixel Shader의 출력 Color의 Alpha값을 Src의 계수로 삼고
	// 1 - Src.Alpha를 Target RGB의 계수로 삼는다. ( 배경 색상 )
	// 이렇게 한 것을 더해준다.

	tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; 
	tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	tDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA; // 1 - SRC.Alpha

	// Pixel의 Alpha값을 어떻게 처리를 할 것인지 정한다.
	// src의 Alpha를 그대로 사용하고, Dest의 Alpha는 0으로 만들어서 더한다. (Only Src.Alpha로 적용)
	// 보이는 차이는 없다. -> 결국 알파블렌딩이라는 것은 실제로 그 부분의 픽셀을 처리하지 않는 개념이 아니라,
	// 배경 색상과 이미지의 색상을 섞는 개념이기 때문이다.
	tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;  
	tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;


	m_pBlendStateArray[(UINT)BLEND_TYPE::ALPHABLEND]->Create(tDesc);
	// 할당 받은 이후에 구조체를 만들어 만들라고 요구
}

void CRenderMgr::CreateDepthStencilState()
{
}

void CRenderMgr::UpdateLight2D()
{
	static CConstBuffer* pLight2DBuffer = CDevice::GetInst()->FindConstBuffer(L"Light2D");

	int iArr[4] = { m_iLight2DCount, }; // 1개는 Light2D의 count 나머지 3개는 패딩이다.

	pLight2DBuffer->SetData(m_tLight2DArr, sizeof(Light2DInfo) * 100);
	pLight2DBuffer->SetData(iArr, 16);
	pLight2DBuffer->UpdateData((UINT)SHADER_TYPE::ST_VERTEX | (UINT)SHADER_TYPE::ST_PIXEL);

	m_iLight2DCount = 0;
}
