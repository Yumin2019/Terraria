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

	// ���� ���� ������Ʈ
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

	// ���.
	CDevice::GetInst()->Present();

	m_iLight2DCount = 0;
}

void CRenderMgr::RenderTool()
{
	// �ʱ�ȭ.
	float arrColor[4] = { 0.6f, 0.6f, 0.6f, 1.f };
	CDevice::GetInst()->Clear(arrColor);

	static CConstBuffer* pGlobalBuffer = CDevice::GetInst()->FindConstBuffer(L"GlobalValue");
	pGlobalBuffer->SetData(&g_tValue);
	pGlobalBuffer->UpdateData(ST_ALL);

	// ���� ���� ������Ʈ
	UpdateLight2D();
	m_iLight2DCount = 0;

	/*
	���� �̿��ϴ� ��쿡�� Tool Camera�� ����Ѵ�.
	����, ��ü�� �׸��� ������ 
	����Ÿ�ٿ� ������Ʈ�� �׸��� Present�Լ��� ȣ���Ͽ� Wnd�� �׸��� �Ǵµ�,
	Engine���� Present�Լ��� ȣ���� ���Ŀ� ToolCamera�� ���ؼ� RenderTarget�� �׷�����
	�̹� Wnd�� �׸� ���Ķ� ������ ���� �ʴ´�. 
	*/
}

void CRenderMgr::CreateSamplerState()
{
	// Sampler �ϳ� �����

	D3D11_SAMPLER_DESC tSamDesc = {};

	tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.Filter = D3D11_FILTER_ANISOTROPIC;

	// wrap : �ٵ��� ������� UV ��ǥ�� ó���Ѵ�.

	DEVICE->CreateSamplerState(&tSamDesc, &m_pSampler0);
	
	CONTEXT->VSSetSamplers(0, 1, &m_pSampler0); // 0�� �������͸� ����ؼ� Sampler�� �����ϰڴ�.
	CONTEXT->HSSetSamplers(0, 1, &m_pSampler0);
	CONTEXT->DSSetSamplers(0, 1, &m_pSampler0);
	CONTEXT->GSSetSamplers(0, 1, &m_pSampler0);
	CONTEXT->CSSetSamplers(0, 1, &m_pSampler0);
	CONTEXT->PSSetSamplers(0, 1, &m_pSampler0);


	tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	// �ȼ� ������ ����ִ� ������ Sampler�̴�.


	DEVICE->CreateSamplerState(&tSamDesc, &m_pSampler1);

	CONTEXT->VSSetSamplers(1, 1, &m_pSampler1); // 1�� �������͸� ����ؼ� Sampler�� �����ϰڴ�.
	CONTEXT->HSSetSamplers(1, 1, &m_pSampler1);
	CONTEXT->DSSetSamplers(1, 1, &m_pSampler1);
	CONTEXT->GSSetSamplers(1, 1, &m_pSampler1);
	CONTEXT->CSSetSamplers(1, 1, &m_pSampler1);
	CONTEXT->PSSetSamplers(1, 1, &m_pSampler1);


}

void CRenderMgr::CreateBlendState()
{

	D3D11_BLEND_DESC tDesc = {};

	m_pBlendStateArray[(UINT)BLEND_TYPE::ALPHABLEND] = new CBlendState; // AlphaBlend ��������
	// BlendState�� �ϳ� ������ش�.

	tDesc.IndependentBlendEnable = false; // �� Ÿ�� ���� Blending ����� �����Ѵ�. ���������� �������� �ʰڴ� (�ϰ�����)
	tDesc.AlphaToCoverageEnable = true; // ��Ƽ ���ø��� �̿��Ͽ� ��Ȯ�� ���İ����� Blending�� �ϰڴ�.

	// ���� �׽�Ʈ�� 0�� 1�θ� ������ �ȴ�.
	// ���� �������� �������� ��ü�� �׷� �� �� ������, �������� ��ü���� �����ؾ� �� �ʿ䰡 �ִ�.
	// ���� �������� ���� ������ �������� ���ؼ� ���ۿ� �����ؾ� �ϹǷ� �ӵ��� ��������.

	// Alpha To Coverage �ɼ��� true�� �ϰ� �Ǹ�, �̹����� ���� �κб��� ����Ͽ� ����ϴ� ����̴�.
	// ���̰� ���� a�� �κ��� ���Ѵ�.

	tDesc.RenderTarget[0].BlendEnable = true;
	tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; 
	// �� �ȼ��� ��� ������ҿ� �����͸� �� �� �ִ��� ���Ѵ�.

	// Render Target Pixel RGB ȥ�� ����.
	// Pixel Shader�� ��� Color�� Alpha���� Src�� ����� ���
	// 1 - Src.Alpha�� Target RGB�� ����� ��´�. ( ��� ���� )
	// �̷��� �� ���� �����ش�.

	tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; 
	tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	tDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA; // 1 - SRC.Alpha

	// Pixel�� Alpha���� ��� ó���� �� ������ ���Ѵ�.
	// src�� Alpha�� �״�� ����ϰ�, Dest�� Alpha�� 0���� ���� ���Ѵ�. (Only Src.Alpha�� ����)
	// ���̴� ���̴� ����. -> �ᱹ ���ĺ����̶�� ���� ������ �� �κ��� �ȼ��� ó������ �ʴ� ������ �ƴ϶�,
	// ��� ����� �̹����� ������ ���� �����̱� �����̴�.
	tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;  
	tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;


	m_pBlendStateArray[(UINT)BLEND_TYPE::ALPHABLEND]->Create(tDesc);
	// �Ҵ� ���� ���Ŀ� ����ü�� ����� ������ �䱸
}

void CRenderMgr::CreateDepthStencilState()
{
}

void CRenderMgr::UpdateLight2D()
{
	static CConstBuffer* pLight2DBuffer = CDevice::GetInst()->FindConstBuffer(L"Light2D");

	int iArr[4] = { m_iLight2DCount, }; // 1���� Light2D�� count ������ 3���� �е��̴�.

	pLight2DBuffer->SetData(m_tLight2DArr, sizeof(Light2DInfo) * 100);
	pLight2DBuffer->SetData(iArr, 16);
	pLight2DBuffer->UpdateData((UINT)SHADER_TYPE::ST_VERTEX | (UINT)SHADER_TYPE::ST_PIXEL);

	m_iLight2DCount = 0;
}
