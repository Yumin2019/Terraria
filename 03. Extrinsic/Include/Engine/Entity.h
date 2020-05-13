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
		// wstring& ���·� ��ȯ. �޸��� ���� ���.
		// const �ܺ�/���������� ���� �ٲ� �� ����.
	}

	/*wstring GetName() const
	{
		return m_strName; // m_strName�� ������ wstring �� ��ȯ.
	}*/

	// ������迡 ������� �θ�� �����ص� Ŭ����
};

