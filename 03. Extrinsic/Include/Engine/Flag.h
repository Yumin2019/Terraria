#pragma once

enum class RES_TYPE
{
	MATERIAL,
	MESH,
	TEXTURE,
	SOUND,
	SHADER,
	END
};

enum class SHADER_TYPE
{
	ST_VERTEX = 0x01,
	ST_HULL = 0x02,
	ST_DOMAIN = 0x04,
	ST_GEOMETRY = 0x08,
	ST_COMPUTE = 0x10,
	ST_PIXEL = 0x20,
	ST_END

	// 각 Shader를 비트 단위로 처리하기 위한 준비
};

enum class COMPONENT_TYPE
{
	TRANSFORM,
	MESHRENDER,
	CAMERA,
	COLLIDER2D,
	COLLIDER3D,
	ANIMATOR2D,
	ANIMATOR3D,
	LIGHT2D,
	LIGHT3D,
	TILEMAP2D,
	TILEMAP3D,
	END,
	SCRIPT,

};


enum class ITEM
{

	DIRT,
	STONE,
	WOOD,
	CACTUS,

	


	END,
	NONE,

	WORKBENCH,
	CACTUS_WORKBENCH,

	// 벽 Obj
	WOOD_WALL,
	CACTUS_WALL,
	STONE_WALL,
	DIRT_WALL,

	// DOOR
	WOOD_DOOR,
	CACTUS_DOOR,

	GEL,
	TORCH,
	VIOLET_TORCH,
	YELLOW_TORCH,
	BLUE_TORCH,
	GREEN_TORCH,
	RED_TORCH,
	ORANGE_TORCH,
	WHITE_TORCH,
	PINK_TORCH,
	ICE_TORCH,

	// 다양한 토치들 추가.

	NUM_END,
	// 만들 수 있는 아이템의 목록.

	CACTUS_SWORD,
	CACTUS_PICKAXE,
	CACTUS_HELMET,
	CACTUS_ARMOR,
	CACTUS_LEGGINGS,

	// 기본템.
	COPPER_SWORD,
	COPPER_AXE,
	COPPER_PICKAXE,

	WOOD_HAMMER,


	CRAFTING_END
};

enum class ITEM_TYPE
{
	NONE,
	WEAPON,
	HELMET,
	ARMOR,
	LEGGINGS,
	AXE,
	PICKAXE,
	HAMMER,
	BLOCK,
	WALL,
	OTHER, // 들지 못하고 재료로 사용되는 아이템으로 할 것이다.
	TORCH,
	DOOR,
	BENCH,

	END
	// 곡괭이

};

enum class SCENE_MODE
{
	PLAY,
	STOP,
	PAUSE
};

enum class COLLIDER2D_TYPE
{
	RECT,
	CIRCLE,
	END
};


enum class SHADER_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,
	INT_END,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,
	FLOAT_END,


	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,
	VEC2_END,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,
	VEC4_END,


	MATRIX_0,
	MATRIX_1,
	MATRIX_2,
	MATRIX_3,
	MATRIX_END,

	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_END,

};

enum class BLEND_TYPE
{
	DEFAULT,
	ALPHABLEND,
	ONEBLEND,
	END,
};


enum class DEPTH_STENCIL_STATE
{
	DEFAULT, // 더 적은 것
	LESS_EQUAL, // 더 적은 것을 우선으로 하고, 같은 것 포함.
	GRATER,
	GRATER_EQUAL,
	END,
};

enum class DBG_TYPE
{
	DT_ERROR,
	DT_WARNNING,
	DT_MSG,
	DT_END
};

enum class ANIMATION_OPTION
{
	AO_LOOP,
	AO_ONCE_LOOP,
	AO_DESTROY,
	AO_ONCE_RETURN,
	AO_END
};

enum class EVENT_TYPE
{
	CREATE_OBJECT, // wParam : GemeObject	lParam : Layer Idx
	DELETE_OBJECT, // wParam : GameObject 
	ADD_CHILD,     // wParam : ParentObject lParam : ChildObject
	CLEAR_PARENT,  // wParam : TargetObject
	TRANSFER_LAYER,// wParam : TargetObject lParam : (HIWORD)Layer Idx (LOWRD)bMoveAll
	TRANSFER_SCENE,// wParam : 

	ENABLE_GAMEOBJECT, 	// wParam : GameObject
	DISABLE_GAMEOBJECT,	// wParam : GameObject

	ENABLE_COMPONENT,	// wParam : Comonent
	DISABLE_COMPONENT,	// wParam : Component

	END
};

enum class LIGHT2D_TYPE
{
	DIR,
	POINT,
	SPOT,
	END
};

enum class CHANNEL_TYPE
{
	BGM,
	EFFECT_0,
	EFFECT_1,
	SWAYITEM,
	DIG,
	GRAB,
	END
};

// CAMERA MOVE
enum class MOVE_DIR_VERTICAL
{
	MD_NONE,
	MD_UP,
	MD_DOWN
};

enum class MOVE_DIR_HORIZON
{
	MD_NONE,
	MD_RIGHT,
	MD_LEFT
};

enum class SCENE_TYPE
{
	TITLE,
	INGAME,
	NONE,
};

typedef struct _tagEvent
{
	EVENT_TYPE eType;
	DWORD_PTR wParam;
	DWORD_PTR lParam;

}Event, *PEvent;