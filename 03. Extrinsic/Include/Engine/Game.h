#pragma once

/*
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

#pragma comment(lib, "dxguid")
using namespace DirectX;
using namespace DirectX::PackedVector;

#include "define.h" // 매크로, enum
#include "struct.h" // 구조체 설계
#include "extern.h" // extern 변수
#include "func.h"   // 전역 함수

*/

// simd 사용 안함
#define _XM_NO_INTRINSICS_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include "SimpleMath.h"

#pragma comment(lib, "d3d11")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

using namespace DirectX; // DirectX에서 사용하는 namespace 이다.
using namespace DirectX::PackedVector; // DirectX에서 제공하는 namespace 의 내부 namespace인 PackedVector에 접근한다.

#include <string>
#include <vector>
#include <list>
#include <map>

using std::vector;
using std::list;
using std::map;
using std::string;
using std::wstring;
using std::make_pair;


// 이런 식으로 설정하면 해당 표현을 사용하는 경우에는 std::로 인식을 하게 된다.


// 사용자 정의 헤더파일

#include "Define.h" // Define
#include "Flag.h" // Types
#include "Structure.h" // Structure 
#include "Extern.h" // 외부 전역 변수
#include "func.h" // 전역함수
