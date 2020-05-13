#pragma once

#include "Resource.h"

/*
nullptr != m_pRes
m_pRes != nullptr 의 차이는 전자의 경우 멤버함수 operator를 통해서 인식하지 못하고
후자는 인식할 수 있다는 것이다. (순서의 영향을 받아서, 따라서 이런 결과를 없애주기 위해서 
전역함수 operator를 선언하도록 한다.
Ptr<T> ptr의 m_pRes에 접근할 수 있도록, 함수를 friend처리를 하도록 한다.
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

	// 스마트포인터를 통해서 리소스를 관리한다.
	// 따라서, 스마트 포인터 끼리의 데입 / 복사 생성자 등을 참조로 인식해야 한다.
	// 스마트 포인터를 통해서 자동으로 Ref 값을 관리하는 시스템을 구현한다.

	Ptr(T* pRes) :
		m_pRes(pRes)
	{
		// Ptr<Resource> ptr = pRes; 복사 생성자 호출시
		// Ptr<Resource> ptr(pRes);
		

		if (m_pRes != nullptr)
			m_pRes->AddRef();
	}

	// 하나 더 구현해둠.
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
	// 데입이 일어나는 경우에도 관리를 해줘야 한다.

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
