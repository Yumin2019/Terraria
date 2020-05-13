#pragma once

#include "Resource.h"

/*
nullptr != m_pRes
m_pRes != nullptr �� ���̴� ������ ��� ����Լ� operator�� ���ؼ� �ν����� ���ϰ�
���ڴ� �ν��� �� �ִٴ� ���̴�. (������ ������ �޾Ƽ�, ���� �̷� ����� �����ֱ� ���ؼ� 
�����Լ� operator�� �����ϵ��� �Ѵ�.
Ptr<T> ptr�� m_pRes�� ������ �� �ֵ���, �Լ��� friendó���� �ϵ��� �Ѵ�.
*/

template <typename T>
bool operator == (void* pRes, Ptr<T> ptr)
{
	return pRes == ptr.m_pRes;
}

template <typename T>
bool operator != (void* pRes, Ptr<T> ptr)
{
	return pRes != ptr.m_pRes;
}


template <typename T>
class Ptr
{
public:
	Ptr() :
		m_pRes(nullptr)
	{

	}

	// ����Ʈ�����͸� ���ؼ� ���ҽ��� �����Ѵ�.
	// ����, ����Ʈ ������ ������ ���� / ���� ������ ���� ������ �ν��ؾ� �Ѵ�.
	// ����Ʈ �����͸� ���ؼ� �ڵ����� Ref ���� �����ϴ� �ý����� �����Ѵ�.

	Ptr(T* pRes) :
		m_pRes(pRes)
	{
		// Ptr<Resource> ptr = pRes; ���� ������ ȣ���
		// Ptr<Resource> ptr(pRes);
		

		if (m_pRes != nullptr)
			m_pRes->AddRef();
	}

	// �ϳ� �� �����ص�.
	Ptr(const Ptr<T>& ptr) :
		m_pRes(ptr.m_pRes)
	{

		// Ptr<Resource> ptr = ptr2;
		// Ptr<Resource> ptr(ptr2);

		if (m_pRes != nullptr)
			m_pRes->AddRef();
	}

	~Ptr()
	{
		if (m_pRes != nullptr)
			m_pRes->Release();
	}

private:
	T*	m_pRes;


	

public:
	// ������ �Ͼ�� ��쿡�� ������ ����� �Ѵ�.

	void operator = (T* pRes)
	{
		// ptr = pRes;

		if (m_pRes != nullptr)
			m_pRes->Release();

		m_pRes = pRes;

		if (m_pRes != nullptr)
			m_pRes->AddRef();
	}

	void operator = (Ptr<T> ptr)
	{
		// ptr = ptr2;

		if (m_pRes != nullptr)
			m_pRes->Release();

		m_pRes = ptr.m_pRes;

		if (m_pRes != nullptr)
			m_pRes->AddRef();
	}

	T* operator -> () const
	{
		return m_pRes;
	}

	T* GetPointer() const
	{
		return m_pRes;
	}

	
	bool operator == (const T* pRes)
	{
		return m_pRes == pRes;
	}

	bool operator != (const T* pRes)
	{
		return m_pRes != pRes;
	}

	template <typename T>
	friend bool operator == (void*, Ptr<T>);

	template <typename T>
	friend bool operator != (void*, Ptr<T>);
};
