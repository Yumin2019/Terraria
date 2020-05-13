#pragma once
#include <Engine/Script.h>

/*

// 방향을 나타내는 스크립트 입니다.
// 이 스크립트는 오브젝트의 좌우에 대한 움직임을 인식하고 그 값을 Mtrl에 셋팅하는 역할을 합니다.

예를 들면, 플레이어 Script에서 코드 중에 좌우 방향을 설정하면...
DirectionScript의 Update에서는 이것을 Mtrl에 셋팅하게 됩니다.

이 Script를 가진 오브젝트는 Standard2DMtrl을 사용합니다.
*/


class CDirectionScript :
	public CScript
{
public:
	CDirectionScript(UINT iScriptType);
	CDirectionScript();
	virtual ~CDirectionScript();

	CDirectionScript* Clone() { assert(false); return nullptr; } 

	// 상속을 받는 형태라면 하위 오브젝트로 내려갈 것이다.
	// 만약 이 Script에서 Clone함수가 호출되었다는 소리는 직접 이 Script를 생성한다는 소리로 막아버리는 처리이다.

private:
	int		m_iDir; // 단순히 이동하는데 사용.

public:
	void SetDir(int iDir)
	{
		m_iDir = iDir;
	}

	int GetDir() const { return m_iDir; }
	void DirChange() { m_iDir *= -1; }

	virtual void Update() {}
	virtual void LateUpdate();
	virtual bool SaveToScene(FILE* pFile);
	virtual bool LoadFromScene(FILE* pFile);

};

