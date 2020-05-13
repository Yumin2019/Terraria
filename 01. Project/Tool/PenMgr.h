#pragma once

class CPenMgr
{
	SINGLE(CPenMgr);





private:
	HPEN	m_hMagentaPen;

	// Tile�� ���� ������ ���� ����.
	
	HPEN	m_hTilePen[(UINT)TILE::END];
	HBRUSH  m_hTileBrush[(UINT)TILE::END];


public:
	bool Init();

public:
	HPEN GetMagentaPen() const { return m_hMagentaPen; }
	HPEN GetTilePen(int idx) { return m_hTilePen[idx]; }
	HBRUSH GetTileBrush(int idx) { return m_hTileBrush[idx]; }
};

