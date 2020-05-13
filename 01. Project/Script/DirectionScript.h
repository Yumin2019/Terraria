#pragma once
#include <Engine/Script.h>

/*

// ������ ��Ÿ���� ��ũ��Ʈ �Դϴ�.
// �� ��ũ��Ʈ�� ������Ʈ�� �¿쿡 ���� �������� �ν��ϰ� �� ���� Mtrl�� �����ϴ� ������ �մϴ�.

���� ���, �÷��̾� Script���� �ڵ� �߿� �¿� ������ �����ϸ�...
DirectionScript�� Update������ �̰��� Mtrl�� �����ϰ� �˴ϴ�.

�� Script�� ���� ������Ʈ�� Standard2DMtrl�� ����մϴ�.
*/


class CDirectionScript :
	public CScript
{
public:
	CDirectionScript(UINT iScriptType);
	CDirectionScript();
	virtual ~CDirectionScript();

	CDirectionScript* Clone() { assert(false); return nullptr; } 

	// ����� �޴� ���¶�� ���� ������Ʈ�� ������ ���̴�.
	// ���� �� Script���� Clone�Լ��� ȣ��Ǿ��ٴ� �Ҹ��� ���� �� Script�� �����Ѵٴ� �Ҹ��� ���ƹ����� ó���̴�.

private:
	int		m_iDir; // �ܼ��� �̵��ϴµ� ���.

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

