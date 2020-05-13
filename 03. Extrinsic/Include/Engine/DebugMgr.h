#pragma once


class CDebugMgr
{
	SINGLE(CDebugMgr);

private:
	vector<Debug> m_vecDebug;

public:
	void AddDebugMsg(DBG_TYPE eType, const wstring& strMsg) { m_vecDebug.push_back(Debug(eType, strMsg)); }
};

