#pragma once
#include "Component.h"


class CLight2D :
	public CComponent
{
public:
	CLight2D();
	virtual ~CLight2D();
	CLONE(CLight2D);

private:
	Light2DInfo		m_tLight2DInfo;				// Light2DInfo 정보.

public:
	virtual void FinalUpdate();
	virtual bool SaveToScene(FILE* pFile);
	virtual bool LoadFromScene(FILE* pFile);
	void UpdateData();

	/*
	
	Vec4 tLightPos;
	Vec4 tLightColor;
	Vec4 tLightDir;
	int iLightType;
	float fRange;
	float fAngle;
	int ipadding;
	*/

	// 점광원만 있다고 하면.

public:
	void SetLightColor(const Vec3& tColor) { m_tLight2DInfo.tLightColor = tColor; Changed(); }
	void SetLightColor(float x, float y, float z) { m_tLight2DInfo.tLightColor = Vec3(x, y, z); Changed(); }
	void SetLightRange(float fRange) { m_tLight2DInfo.fRange = fRange; Changed(); }
	void SetLightType(LIGHT2D_TYPE eType) { m_tLight2DInfo.iLightType = (int)eType; Changed(); }

	const Vec3& GetColor() const { return Vec3(m_tLight2DInfo.tLightColor.x, m_tLight2DInfo.tLightColor.y, m_tLight2DInfo.tLightColor.z); } // float 그대로
	unsigned char GetR() const { return (unsigned char)(m_tLight2DInfo.tLightColor.x * 255.f); }
	unsigned char GetG() const { return (unsigned char)(m_tLight2DInfo.tLightColor.y * 255.f); }
	unsigned char GetB() const { return (unsigned char)(m_tLight2DInfo.tLightColor.z * 255.f); }
	float GetRange() const { return m_tLight2DInfo.fRange; }
};

