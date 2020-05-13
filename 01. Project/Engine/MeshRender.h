#pragma once
#include "Component.h"

#include "Ptr.h"
#include "Material.h"
#include "Mesh.h"

/*
Mesh Render Component는 Mesh 를 Render하는 역할을 한다.
자신이 그려야 하는 Mesh를 가지고 있고, 어떤 Shader를 가지고 그릴지에 대한 정보를 가지고 있다.
*/
class CMeshRender :
	public CComponent
{
public:
	CMeshRender();
	virtual ~CMeshRender();
	CLONE(CMeshRender);

private:
	Ptr<CMaterial>	m_pMtrl;
	Ptr<CMesh>		m_pMesh;

public:
	void SetMaterial(Ptr<CMaterial> pMtrl) { m_pMtrl = pMtrl; Changed(); }
	void SetMesh(Ptr<CMesh> pMesh) { m_pMesh = pMesh; Changed(); }

	Ptr<CMaterial> GetSharedMaterial() { return m_pMtrl; }
	Ptr<CMaterial> GetCloneMaterial();
	Ptr<CMesh> GetMesh() { return m_pMesh; }

public:
	void Render();

	virtual bool SaveToScene(FILE* pFile);
	virtual bool LoadFromScene(FILE* pFile);
};

