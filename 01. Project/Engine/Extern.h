#pragma once

// Link 시점에서 다른 cpp 파일에서 선언된 전역 변수를 연결해주는 역할을 한다.
extern _tagTransform g_tTransform;
extern _tagGlobalValue g_tValue;

extern const wchar_t* RES_TYPE_NAME[(UINT)RES_TYPE::END];
extern const wchar_t* SHADER_PARAM_NAME[(UINT)SHADER_PARAM::TEX_END];
extern const wchar_t* ANIMATION_OPTION_NAME[(UINT)ANIMATION_OPTION::AO_END];
extern const wchar_t* ITEM_NAME[(UINT)ITEM::CRAFTING_END];

