#pragma once
#include <Engine/Script.h>

class CTileScript :
	public CScript
{
public:
	CTileScript();
	virtual ~CTileScript();

	CLONE(CTileScript);

private:
	Vec2 m_tImageSize;
	Vec2 m_tTileSize; 
	Vec2 m_tLeftTop; // UV ��ǥ�� ��������.
	ITEM m_eItem; // �� ���� ĳ�� ������ �������� �����Ѵ�.

public:
	void SetItem(ITEM eItem) { m_eItem = eItem; }
	void SetTileSize(const Vec2& tSize) { m_tTileSize = tSize; } // UV�� �־��ּ���.
	void SetLeftTop(const Vec2& tLeftTop) { m_tLeftTop = tLeftTop; }
	void SetImageSize(const Vec2& tImageSize) { m_tImageSize = tImageSize; }

public:
	ITEM GetItem() const { return m_eItem; }
	const Vec2& GetTileSize() const { return m_tTileSize; }
	const Vec2& GetLeftTop() const { return m_tLeftTop; }
	

public:
	virtual void Update();
	virtual void UpdateData();
};

