#pragma once

#include <afxext.h>

class CMySplitterWnd :
	public CSplitterWnd
{
public:
	void SetBorder(CPoint _ptBorder, CPoint _ptBar)
	{
		m_cxBorder = _ptBorder.x;
		m_cyBorder = _ptBorder.y;

		m_cxSplitter = _ptBar.x;
		m_cySplitter = _ptBar.y;

		m_cxSplitter = _ptBar.x;
		m_cySplitter = _ptBar.y;

		m_cxSplitterGap = _ptBar.x;
		m_cySplitterGap = _ptBar.y;
	}


public:
	CMySplitterWnd();
	virtual ~CMySplitterWnd();
};


