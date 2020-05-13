#include "stdafx.h"
#include "BlendState.h"

#include "Device.h"

CBlendState::CBlendState()
{
}


CBlendState::~CBlendState()
{
	SAFE_RELEASE(m_pState);
}

void CBlendState::Create(const D3D11_BLEND_DESC & tDesc)
{
	m_tDesc = tDesc; 
	// BLEND_STATE�� �ɼ��� �޾Ƶδ� �뵵�̴�.

	//D3D11_DEPTH_STENCIL_DESC tDest;
	//DEVICE->CreateDepthStencilState()
	DEVICE->CreateBlendState(&tDesc, &m_pState);
}

void CBlendState::UpdateData()
{
	CONTEXT->OMSetBlendState(m_pState, nullptr, 0xffffffff);
}
