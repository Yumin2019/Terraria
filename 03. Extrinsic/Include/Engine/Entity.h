#pragma once
class CEntity
{
public:
	CEntity();
	virtual ~CEntity();

private:
	wstring			m_strName;

public:
	void SetName(const wstring& strName)
	{
		m_strName = strName;
	}

	const wstring& GetName() const
	{
		return m_strName;
		// wstring& 형태로 반환. 메모리의 복사 비용.
		// const 외부/내부적에서 값을 바꿀 수 없음.
	}

	/*wstring GetName() const
	{
		return m_strName; // m_strName을 복사한 wstring 을 반환.
	}*/

	// 연결관계에 상관없이 부모로 지정해둘 클래스
};

