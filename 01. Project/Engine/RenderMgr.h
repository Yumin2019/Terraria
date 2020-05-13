#pragma once

class CDepthStencilState;
class CBlendState;
class CCamera;

class CRenderMgr
{
	SINGLE(CRenderMgr);

private:
	CBlendState*			m_pBlendStateArray[(UINT)BLEND_TYPE::END];
	CDepthStencilState*		m_pDepthStencilStateArray[(UINT)DEPTH_STENCIL_STATE::END];

	ID3D11SamplerState*		m_pSampler0;
	ID3D11SamplerState*		m_pSampler1;

	Light2DInfo				m_tLight2DArr[100];
	unsigned int			m_iLight2DCount;

	vector<CCamera*>		m_vecCam;

public:
	bool Init();
	void Render();
	void RenderTool();


private:
	void CreateSamplerState();
	void CreateBlendState();
	void CreateDepthStencilState();

	void UpdateLight2D(); // Light2DArr의 정보를 버퍼에 넘겨준다.


public:
	CBlendState* GetBlendState(BLEND_TYPE eType) const { return m_pBlendStateArray[(UINT)eType]; }
	CDepthStencilState* GetDepthStencilState(DEPTH_STENCIL_STATE eType) const
	{
		return m_pDepthStencilStateArray[(UINT)eType];
	}

	void RegisterLight2D(const Light2DInfo& tInfo)
	{
		// 더 이상 받을 수 없다.
		if (m_iLight2DCount >= 100)
			return; 

		m_tLight2DArr[m_iLight2DCount] = tInfo;
		++m_iLight2DCount;
	}

	CCamera* GetCamera(int iIdx) { return m_vecCam[iIdx]; }
	void RegisterCamera(CCamera* pCam) { 
		m_vecCam.push_back(pCam); 
	}
	void ClearCamera() { m_vecCam.clear(); }

};

