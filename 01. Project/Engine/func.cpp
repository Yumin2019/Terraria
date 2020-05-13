
#pragma once
#include "func.h"
#include "stdafx.h"

_tagGlobalValue g_tValue = {};


const wchar_t* ANIMATION_OPTION_NAME[(UINT)ANIMATION_OPTION::AO_END] =
{
	L"AO_LOOP",
	L"AO_ONCE_LOOP",
	L"AO_DESTROY",
	L"AO_ONCE_RETURN"
};

const wchar_t* RES_TYPE_NAME[(UINT)RES_TYPE::END] =
{
	L"MATERIAL",
	L"MESH",
	L"TEXTURE",
	L"SOUND",
	L"SHADER"
};

const wchar_t* SHADER_PARAM_NAME[(UINT)SHADER_PARAM::TEX_END] =
{

	L"INT_0",
	L"INT_1",
	L"INT_2",
	L"INT_3",
	L"INT_END",

	L"FLOAT_0",
	L"FLOAT_1",
	L"FLOAT_2",
	L"FLOAT_3",
	L"FLOAT_END",

	L"VEC2_0",
	L"VEC2_1",
	L"VEC2_2",
	L"VEC2_3",
	L"VEC2_END",

	L"VEC4_0",
	L"VEC4_1",
	L"VEC4_2",
	L"VEC4_3",
	L"VEC4_END",

	L"MATRIX_0",
	L"MATRIX_1",
	L"MATRIX_2",
	L"MATRIX_3",
	L"MATRIX_END",

	L"TEX_0",
	L"TEX_1",
	L"TEX_2",
	L"TEX_3"
};


const wchar_t* ITEM_NAME[(UINT)ITEM::CRAFTING_END] =
{
	L"Dirt",
	L"Stone",
	L"Wood",
	L"Cactus",

	L"end",
	L"none",
	L"WorkBench",
	L"CactusWorkBench",

	// 벽 Obj
	L"WoodWall",
	L"CactusWall",
	L"StoneWall",
	L"DirtWall",

	// Door
	L"WoodDoor",
	L"CactusDoor",

	L"Gel",
	L"Torch",
	L"VioletTorch",
	L"YellowTorch",
	L"BlueTorch",
	L"GreenTorch",
	L"RedTorch",
	L"OrangeTorch",
	L"WhiteTorch",
	L"PinkTorch",
	L"IceTorch",

		
	L"num_end",

	L"CactusSword",
	L"CactusPickaxe",
	L"CactusHelmet",
	L"CactusArmor",
	L"CactusLeggings",

	L"CopperSword",
	L"CopperAxe",
	L"CopperPickaxe",
	
	L"WoodHammer",

};


ITEM_TYPE GetItemType(ITEM eItem)
{
	switch (eItem)
	{



		// Wall
	case ITEM::CACTUS_WALL:
	case ITEM::STONE_WALL:
	case ITEM::DIRT_WALL:
	case ITEM::WOOD_WALL:
		return ITEM_TYPE::WALL;

		// DOOR
	case ITEM::WOOD_DOOR:
	case ITEM::CACTUS_DOOR:
		return ITEM_TYPE::DOOR;

	case ITEM::DIRT:
	case ITEM::STONE:
	case ITEM::WOOD:
	case ITEM::CACTUS:
		return ITEM_TYPE::BLOCK;

	case ITEM::CACTUS_WORKBENCH:
	case ITEM::WORKBENCH:
		return ITEM_TYPE::BENCH;

	case ITEM::GEL:

		return ITEM_TYPE::OTHER;

	case ITEM::TORCH:
	case ITEM::VIOLET_TORCH:
	case ITEM::YELLOW_TORCH:
	case ITEM::BLUE_TORCH:
	case ITEM::GREEN_TORCH:
	case ITEM::RED_TORCH:
	case ITEM::ORANGE_TORCH:
	case ITEM::WHITE_TORCH:
	case ITEM::PINK_TORCH:
	case ITEM::ICE_TORCH:

		return ITEM_TYPE::TORCH;


	case ITEM::CRAFTING_END:
	case ITEM::NONE:
	case ITEM::END:
		return ITEM_TYPE::NONE;


	case ITEM::CACTUS_SWORD:
	case ITEM::COPPER_SWORD:
		return ITEM_TYPE::WEAPON;

	case ITEM::COPPER_AXE:
		return ITEM_TYPE::AXE;

	case ITEM::WOOD_HAMMER:
		return ITEM_TYPE::HAMMER;


	case ITEM::CACTUS_PICKAXE:
	case ITEM::COPPER_PICKAXE:
		return ITEM_TYPE::PICKAXE;

	case ITEM::CACTUS_HELMET:
		return ITEM_TYPE::HELMET;

	case ITEM::CACTUS_ARMOR:
		return ITEM_TYPE::ARMOR;

	case ITEM::CACTUS_LEGGINGS:
		return ITEM_TYPE::LEGGINGS;

	}

	return ITEM_TYPE::NONE;
}

int GetSizeofFormat(DXGI_FORMAT _eFormat)
{
	int iRetByte = 0;
	switch (_eFormat)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		iRetByte = 128;
		break;

	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		iRetByte = 96;
		break;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
	case DXGI_FORMAT_R32G32_TYPELESS:
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		iRetByte = 64;
		break;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R11G11B10_FLOAT:
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
	case DXGI_FORMAT_R16G16_TYPELESS:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_SINT:
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R32_UINT:
	case DXGI_FORMAT_R32_SINT:
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
		iRetByte = 32;
		break;
	case DXGI_FORMAT_R8G8_TYPELESS:
	case DXGI_FORMAT_R8G8_UNORM:
	case DXGI_FORMAT_R8G8_UINT:
	case DXGI_FORMAT_R8G8_SNORM:
	case DXGI_FORMAT_R8G8_SINT:
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_R16_FLOAT:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
	case DXGI_FORMAT_R16_UINT:
	case DXGI_FORMAT_R16_SNORM:
	case DXGI_FORMAT_R16_SINT:
	case DXGI_FORMAT_B5G6R5_UNORM:
	case DXGI_FORMAT_B5G5R5A1_UNORM:
		iRetByte = 16;
		break;
	case DXGI_FORMAT_R8_TYPELESS:
	case DXGI_FORMAT_R8_UNORM:
	case DXGI_FORMAT_R8_UINT:
	case DXGI_FORMAT_R8_SNORM:
	case DXGI_FORMAT_R8_SINT:
	case DXGI_FORMAT_A8_UNORM:
		iRetByte = 8;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
		iRetByte = 128;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_R1_UNORM:
	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC4_TYPELESS:
	case DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
		iRetByte = 64;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		iRetByte = 32;
		break;
		// These are compressed, but bit-size information is unclear.
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
		iRetByte = 32;
		break;
	case DXGI_FORMAT_UNKNOWN:
	default:
		return -1;
	}

	return iRetByte / 8;
}

float GetLength(float x, float y, float z)
{
	return sqrt(x * x + y * y + z * z);
}

float GetLength(const Vec3 & vVec)
{
	return sqrt(vVec.x * vVec.x + vVec.y * vVec.y + vVec.z * vVec.z);
}

float GetLength(float x, float y)
{
	return sqrt(x * x + y * y);
}

float GetLength(float x1, float y1, float x2, float y2)
{
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

float GetAngle(const Vec2 & tSrc, const Vec2 & tDest)
{
	float fDist = Vec2::Distance(tSrc, tDest);
	float fWidth = tDest.x - tSrc.x;

	// cos 밑변 / 빗변 
	// 코사인 값을 구한다.
	float fAngle = fWidth / fDist;

	// 코사인 값을 이용하여 각도를 구한다.
	fAngle = acosf(fAngle);

	if (tDest.y > tSrc.y)
		fAngle = XM_2PI - fAngle;

	return fAngle;
}

void SaveWString(FILE * pFile, const wstring & str)
{
	BYTE c = str.length();
	fwrite(&c, 1, 1, pFile);
	fwrite(str.c_str(), 2, c, pFile);
}

wchar_t * LoadWString(FILE * pFile)
{
	static wchar_t szStr[255] = {};

	BYTE c = 0;
	fread(&c, 1, 1, pFile);
	fread(szStr, 2, c, pFile);
	szStr[c] = 0;

	return szStr;
}
