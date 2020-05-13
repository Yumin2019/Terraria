#pragma once
#include <Engine/Script.h>

enum class SELECT_TYPE
{
	NONE, // ����
	ONE, // Ÿ��
	TWO_W, // ���� ������
	TWO_H,
	FOUR,
	TWO_W_THREE_H
};

class CSelectScript :
	public CScript
{
public:
	CSelectScript();
	virtual ~CSelectScript();
	CLONE(CSelectScript);

private:
	SELECT_TYPE		m_eSelect; // Select Obj�� Type���̴�. �� ���� ����, �浹ü�� ����� �޶�����.


public:
	SELECT_TYPE GetSelectType() const { return m_eSelect; }
	void ChangeSelectType(SELECT_TYPE eType);
	bool IsIdx(int iIdx) { return (iIdx >= 0 && iIdx < 4000); }

	void OnCollisionTileEnter(CTileMap2D* pTileMap, int iX, int iY);
	void OnCollisionTileExit(CTileMap2D* pTileMap, int iX, int iY);
	// SELECT_TYPE�� ����, �־��� Ÿ���� ��ġ�� LT�� �����ϰ� TILE�� �浹������ ī������ �Ѵ�.


public:
	virtual void Update() {}
};

