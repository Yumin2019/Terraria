#pragma once
#include "Component.h"



enum class PROJ_TYPE
{
	ORTHGRAPHIC, // 직교
	PERSPECTIVE // 원근
};

class CCamera :
	public CComponent
{
public:
	CCamera();
	virtual ~CCamera();
	
	CLONE(CCamera);
	// Camera는 기본 복사 생성자를 이용한다.

private:
	float		m_fNear;
	float		m_fFar;
	// View Space의 시야 범위

	float		m_fFOV; // 원근 투영 시 시야각
	float		m_fScale; // 직교 투영 시 투영 범위

	Matrix		m_matView; // View Matrix
	Matrix		m_matProj;  // Projection Matrix

	PROJ_TYPE	m_eType;

	MOVE_DIR_VERTICAL	m_eVertical;
	MOVE_DIR_HORIZON	m_eHorizon;

	UINT				m_iLayerCheck; // 카메라가 찍을 Layer를 표시하기 위한 bit값

public:
	virtual void FinalUpdate();
	virtual bool SaveToScene(FILE* pFile);
	virtual bool LoadFromScene(FILE* pFile);
	void Render();

public:
	void SetLayerCheck(int iLayerIdx, bool bAdd)
	{
		if (iLayerIdx == -1)
			assert(false);

		if (bAdd)
			m_iLayerCheck |= (1 << iLayerIdx);
		else
			m_iLayerCheck &= ~(1 << iLayerIdx);

		Changed();
	}
	void SetLayerCheckAll() { m_iLayerCheck = 0xffffffff; Changed();}
	void SetLayerUnCheckAll() { m_iLayerCheck = 0; Changed(); }
	bool IsLayerCheck(int iLayerIdx)
	{
		return m_iLayerCheck &= (1 << iLayerIdx);
	}

	bool IsLayerAllCheck() const { return m_iLayerCheck == 0xffffffff; }

public:

	void SetHorizon(MOVE_DIR_HORIZON eHorizon) { m_eHorizon = eHorizon; }
	void SetVertical(MOVE_DIR_VERTICAL eVertical) { m_eVertical = eVertical; }


	MOVE_DIR_HORIZON GetHorizonMove() const { return m_eHorizon; }
	MOVE_DIR_VERTICAL GetVerticalMove() const { return m_eVertical; }

	// script end

public:

	const Matrix& GetViewMatrix() const { return m_matView; }
	const Matrix& GetProjMatrix() const { return m_matProj; }

	void SetFar(float fFar) { m_fFar = fFar; Changed(); }
	void SetFOV(float fFOV) { m_fFOV = fFOV; Changed(); }
	void SetScale(float fScale) { m_fScale = fScale;  Changed();}
	void SetProjType(PROJ_TYPE eType) { m_eType = eType;  Changed();}

	float GetFar() const { return m_fFar; }
	float GetFOV() const { return m_fFOV; }
	float GetScale() const { return m_fScale; }
	PROJ_TYPE GetProjType() const { return m_eType; }


};

