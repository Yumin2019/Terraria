#pragma once

ITEM_TYPE GetItemType(ITEM eItem);
int GetSizeofFormat(DXGI_FORMAT _eFormat);

template <typename T>
void Safe_Delete_VecList(T& vec)
{
	typename T::iterator iter;
	typename T::iterator iterEnd = vec.end();

	for (iter = vec.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE(*iter);
	}

	vec.clear();
}

template <typename T>
void Safe_Release_VecList(T& vec)
{
	typename T::iterator iter;
	typename T::iterator iterEnd = vec.end();

	for (iter = vec.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(*iter);
	}

	vec.clear();
}

// �� �迭�� ���(������)�� �����Ҵ��� �̷���� �ִ� ��쿡 ����� �� �ִ�.
template <typename T, int iSize>
void Safe_Delete_Array(T*(&arr)[iSize])
{
	// T* iSize��ŭ�� ������ ���� �迭 array�� ���ڷ� �޴´�.

	for (UINT i = 0; i < iSize; ++i)
		SAFE_DELETE(arr[i]);
}

template <typename T>
void Safe_Delete_Map(T& map)
{
	typename T::iterator iter;
	typename T::iterator iterEnd = map.end();

	for (iter = map.begin(); iter != iterEnd; ++iter)
		SAFE_DELETE(iter->second);

	map.clear();
}

// Vector3�� �־��ָ�, ���̸� ���ϴ� �Լ��̴�.

float GetLength(float x, float y, float z);
float GetLength(const Vec3& vVec);
float GetLength(float x, float y);
float GetLength(float x1, float y1, float x2, float y2);
// Vec�� ���̸� �����ִ� �Լ��̴�.

// Src���� Dest�� ���ϴ� ������ ���ϴ� �Լ��̴�.
float GetAngle(const Vec2& tSrc, const Vec2& tDest);



void SaveWString(FILE* pFile, const wstring& str);

wchar_t* LoadWString(FILE* pFile);
