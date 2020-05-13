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
	// BLEND_STATE의 옵션을 받아두는 용도이다.

	//D3D11_DEPTH_STENCIL_DESC tDest;
	//DEVICE->CreateDepthStencilState()
	DEVICE->CreateBlendState(&tDesc, &m_pState);
}

void CBlendState::UpdateData()
{
	CONTEXT->OMSetBlendState(m_pState, nullptr, 0xffffffff);
}
