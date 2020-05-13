#pragma once
#include "Entity.h"



class CBlendState :
	public CEntity
{
public:
	CBlendState();
	virtual ~CBlendState();

private:
	ID3D11BlendState*	m_pState;
	D3D11_BLEND_DESC		m_tDesc;

public:
	void Create(const D3D11_BLEND_DESC& tDesc);
	void UpdateData();


};

