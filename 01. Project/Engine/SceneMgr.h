#pragma once

class CScene;
class CCamera;
class CGameObject;

class CSceneMgr
{
	DECLARE_SINGLE(CSceneMgr);

private:
	CScene* m_pScene;
	CScene* m_pNextScene;

public:
	bool Init();
	void Update();
	void UpdateTool();
	void ChangeScene(CScene* pNextScene);

	CScene* GetScene() const { return m_pScene; }
	CScene* GetNextScene() const { return m_pNextScene; }

	void FindObjByTag(const wstring& strName, vector<CGameObject*>& vecObj);
	Vec3 FindObjByPoint(POINT tPoint, vector<CGameObject*>& vecObj, CCamera* pToolCamera = nullptr);

};

