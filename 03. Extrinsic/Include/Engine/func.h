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

// 각 배열의 요소(포인터)에 동적할당이 이루어져 있는 경우에 사용할 수 있다.
template <typename T, int iSize>
void Safe_Delete_Array(T*(&arr)[iSize])
{
	// T* iSize만큼의 공간을 가진 배열 array를 인자로 받는다.

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

// Vector3를 넣어주면, 길이를 구하는 함수이다.

float GetLength(float x, float y, float z);
float GetLength(const Vec3& vVec);
float GetLength(float x, float y);
float GetLength(float x1, float y1, float x2, float y2);
// Vec의 길이를 구해주는 함수이다.

// Src에서 Dest로 향하는 각도를 구하는 함수이다.
float GetAngle(const Vec2& tSrc, const Vec2& tDest);



void SaveWString(FILE* pFile, const wstring& str);

wchar_t* LoadWString(FILE* pFile);
