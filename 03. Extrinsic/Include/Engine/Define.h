#pragma once

#define SAFE_DELETE(p) if(p) {delete p; p = nullptr;}
// 배열로 동적할당을 한 경우에 []
#define SAFE_DELETE_ARRAY(p) if(p) {delete[] p; p = nullptr;}
#define SAFE_RELEASE(p) if(p) {p->Release();}

// Dynamic Allocation Sigleton
#define DECLARE_SINGLE(Type) \
private: \
Type(); \
~Type(); \
private: \
static Type* m_pInst; \
public: \
static Type* GetInst() \
{ \
	if(!m_pInst) \
		m_pInst = new Type; \
		return m_pInst; \
} \
static void DestroyInst() \
{\
	SAFE_DELETE(m_pInst); \
}\

#define DEFINITION_SINGLE(Type) Type* Type::m_pInst = nullptr
#define GET_SINGLE(Type) Type::GetInst()
#define DESTROY_SINGLE(Type) Type::DestroyInst() 

// Static Singleton
#define SINGLE(type) private: type(); ~type();\
public:\
static type* GetInst()\
{\
	static type mgr;\
	return &mgr;\
}

#define DEVICE GET_SINGLE(CDevice)->GetDevice()
#define CONTEXT GET_SINGLE(CDevice)->GetContext()

#define KEY(Key, State) (CKeyMgr::GetInst()->GetKeyState(Key) == State)
#define KEYPRESS(Key) KEY(Key, KEY_STATE::STATE_HOLD)
#define KEYDOWN(Key) KEY(Key, KEY_STATE::STATE_TAB)
#define KEYUP(Key) KEY(Key, KEY_STATE::STATE_AWAY)
#define KEYNONE(Key) KEY(Key, KEY_STATE::STATE_NONE)

#define DT CTimeMgr::GetInst()->GetDeltaTime()

#define KEY_TAB KEYDOWN
#define KEY_HOLD KEYPRESS
#define KEY_AWAY KEYUP
#define KEY_NONE KEYNONE

#define CLONE(Type) public: Type* Clone() {return new Type(*this);} // 복사 생성자 Macro
#define CLONE_DISABLE(Type) Type* Clone() { assert(false); return nullptr;} \
									Type(const Type& obj) { assert(false);}
// 복사 생성자가 호출이 되면 안 되는 객체에서 사용하는 macro

#define DBG_ERROR(str) CDebugMgr::GetInst()->AddDebugMsg(DBG_TYPE::DT_ERROR, str)
#define DBG_WARNNING(str) CDebugMgr::GetInst()->AddDebugMsg(DBG_TYPE::DT_WARNNING, str)
#define DBG_MSG(str) CDebugMgr::GetInst()->AddDebugMsg(DBG_TYPE::DT_MSG, str)


typedef DirectX::SimpleMath::Vector2 Vec2;
typedef DirectX::SimpleMath::Vector3 Vec3;
typedef DirectX::SimpleMath::Vector4 Vec4;
using DirectX::SimpleMath::Matrix;


#define MAX_LAYER 32

#define ST_ALL (UINT)SHADER_TYPE::ST_VERTEX | (UINT)SHADER_TYPE::ST_HULL\
 | (UINT)SHADER_TYPE::ST_DOMAIN | (UINT)SHADER_TYPE::ST_GEOMETRY | \
(UINT)SHADER_TYPE::ST_COMPUTE | (UINT)SHADER_TYPE::ST_PIXEL

#define RS_WIDTH	 1280.f
#define RS_HEIGHT	 720.f

#define TILE_LAYER_IDX 1


