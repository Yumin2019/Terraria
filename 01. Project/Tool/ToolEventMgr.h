#pragma once

class CGameObject;

class CToolEventMgr
{
	SINGLE(CToolEventMgr);

	vector<Event>			m_vecEvent;
	vector<CGameObject*>	m_vecDead;


public:
	void Update();
	void Clear() { m_vecEvent.clear(); }
	void AddEvent(const Event& tEvent) { m_vecEvent.push_back(tEvent); }

	const vector<Event>& GetEventVec() const { return m_vecEvent; }

private:
	void Execute(const Event& tEvent);
};
