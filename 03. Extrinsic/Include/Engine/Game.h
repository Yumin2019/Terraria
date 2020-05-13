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

#include "define.h" // ��ũ��, enum
#include "struct.h" // ����ü ����
#include "extern.h" // extern ����
#include "func.h"   // ���� �Լ�

*/

// simd ��� ����
#define _XM_NO_INTRINSICS_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include "SimpleMath.h"

#pragma comment(lib, "d3d11")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

using namespace DirectX; // DirectX���� ����ϴ� namespace �̴�.
using namespace DirectX::PackedVector; // DirectX���� �����ϴ� namespace �� ���� namespace�� PackedVector�� �����Ѵ�.

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


// �̷� ������ �����ϸ� �ش� ǥ���� ����ϴ� ��쿡�� std::�� �ν��� �ϰ� �ȴ�.


// ����� ���� �������

#include "Define.h" // Define
#include "Flag.h" // Types
#include "Structure.h" // Structure 
#include "Extern.h" // �ܺ� ���� ����
#include "func.h" // �����Լ�
