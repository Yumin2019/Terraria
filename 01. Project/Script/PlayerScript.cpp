#include "stdafx.h"
#include "PlayerScript.h"
#include "ItemScript.h"

#include <Engine/Camera.h>
#include <Engine/Layer.h>
#include <Engine/TileMap2D.h>

#include "TileMapMgr.h"
#include "UtilMgr.h"

#include "JumpScript.h"
#include "GravityScript.h"
#include "FlyawayScript.h"
#include "InventoryScript.h"
#include "MouseScript.h"
#include "ArmorScript.h"
#include "MainCameraScript.h"
#include "CraftingScript.h"

bool CPlayerScript::g_bBench;

CPlayerScript::CPlayerScript() :
	m_pTorch(nullptr),
	m_pItem(nullptr),
	m_pInven(nullptr),
	m_pGravity(nullptr),
	m_pJump(nullptr),
	m_pFly(nullptr),
	m_pHelmet(nullptr),
	m_pArmor(nullptr),
	m_pLeggings(nullptr),
	m_fSpeed(200.f),
	CDirectionScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT),
	m_iArmor(0),
	m_iHP(100),
	m_iHPMax(100),
	m_bAlpha(false),
	m_bAttacked(false),
	m_bTorch(false),
	m_fAttacked(0.f),
	m_fAlphaTime(0.f),
	m_fAlphaPart(0.f),
	m_fAlphaLimitTime(1.5f),
	m_fAlpha(1.f)
{
}


CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Attacked(int iDamage)
{
	m_fAlphaLimitTime = 1.5f;
	m_fAlpha = 1.f;
	m_fAttacked = 0.f;
	m_fAlphaTime = 0.f;
	m_fAlphaPart = 0.f;
	m_bAlpha = true;
	m_bAttacked = true;

	// Damage - iArmor
	iDamage -= m_iArmor;

	if (iDamage <= 0)
		iDamage = 1;

	// Damage에 대한 Font 출력
	Vec3 vCenter = GetTransform()->GetLocalPos();

	Vec2 vStart = Vec2(vCenter);

	vStart.x += 5.f * (rand() % 3 - 2);
	vStart.y += 10.f * (rand() % 3 + 2);

	Vec2 vEnd = vStart + Vec2(15.f * (rand() % 7 - 4), 70.f);

	Vec4 vStartColor = Vec4(255.f, 0.f, 0.f, 255.f);
	Vec4 vEndColor = vStartColor;
	vEndColor.w = 50.f;
	
	float fPlayTime = 0.5f;
	float fStartScale = 25.f + (float)(rand() % 6); // 25 ~ 30

	CUtilMgr::GetInst()->AddFont(to_wstring(iDamage), fPlayTime, vStart, vEnd,
		fStartScale, fStartScale + 3.f, vStartColor, vEndColor);

	CResMgr::GetInst()->Play(L"Player_Hit_" + to_wstring(rand() % 3), CHANNEL_TYPE::EFFECT_0, EFFECT_VOLUME);

	m_iHP -= iDamage;

	if (m_iHP <= 0)
	{
		m_iHP = m_iHPMax;
		// 죽지 않습니다.
	}
}

int CPlayerScript::Attack()
{
	if (GetHandleItem())
		return GetItemDamage(GetHandleItem()->GetItem());

	return 0;
}

bool CPlayerScript::IsSwayItem()
{
	if (m_pItem == nullptr)
		return false;
	
	return m_pItem->IsSway();
}

void CPlayerScript::PlayAnimation(const wstring & strName)
{
	GetAnimator2D()->PlayAnimation(strName);

	if (m_pHelmet != nullptr)
	{
		m_pHelmet->PlayAnimation(strName);
	}

	if (m_pArmor != nullptr)
	{
		m_pArmor->PlayAnimation(strName);
	}

	if (m_pLeggings != nullptr)
	{
		m_pLeggings->PlayAnimation(strName);
	}


}

void CPlayerScript::SetArmor()
{
	int iArmor = 0;
	m_fAlpha = 1.f;

	if (m_pHelmet != nullptr)
	{
		iArmor += m_pHelmet->GetArmor();

		m_pHelmet->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_2, &m_fAlpha);
	}

	if (m_pArmor != nullptr)
	{
		iArmor += m_pArmor->GetArmor();

		m_pArmor->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_2, &m_fAlpha);

	}

	if (m_pLeggings != nullptr)
	{
		iArmor += m_pLeggings->GetArmor();

		m_pLeggings->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_2, &m_fAlpha);

	}

	m_iArmor = iArmor;
}

void CPlayerScript::SetTorch(ITEM eItem)
{
	Vec3 vColor;

	switch (eItem)
	{
	case ITEM::TORCH:
	case ITEM::WHITE_TORCH:
		vColor = Vec3(1.f, 1.f, 1.f); //

		break;

	case ITEM::VIOLET_TORCH:
		vColor = Vec3(185.f / 255.f, 115.f / 255.f, 255.f / 255.f); //


		break;
	case ITEM::YELLOW_TORCH:
		vColor = Vec3(1.f, 1.f, 0.4f); //


		break;
	case ITEM::BLUE_TORCH:
		vColor = Vec3(0.3f, 1.f, 1.f);//


		break;
	case ITEM::GREEN_TORCH:
		vColor = Vec3(0.5f, 1.f, 0.5f); //


		break;
	case ITEM::RED_TORCH:
		vColor = Vec3(1.f, 0.015f, 0.015f); //


		break;
	case ITEM::ORANGE_TORCH:
		vColor = Vec3(1.f, 0.5f, 0.f); //


		break;
	case ITEM::PINK_TORCH:
		vColor = Vec3(1.f, 0.4f, 1.f);//

		break;

	case ITEM::ICE_TORCH:
		vColor = Vec3(0.5f, 1.f, 1.f); //

		break;

	default:
		assert(false);
		break;
	}

	m_pTorch->GetLight2D()->SetLightColor(vColor);
	m_bTorch = true;

	m_pTorch->SetEnable(true);
}

bool CPlayerScript::IsDigable(ITEM eType)
{
	// 해당 도구로 이 블럭을 캘 수 있는지 판단하는 함수이다.

	if (m_pItem == nullptr)
		return false;

	switch (eType)
	{
	case ITEM::NONE:
		break;


		// 횃불류 그냥 캐지도록.
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

		return true;

	case ITEM::DIRT:
	case ITEM::STONE:

		if (m_pItem->GetType() == ITEM_TYPE::PICKAXE)
			return true;

		// 곡괭이
		break;


	case ITEM::WOOD_DOOR:
	case ITEM::CACTUS_DOOR:

	case ITEM::WOOD:
	case ITEM::CACTUS:

	case ITEM::WORKBENCH:
	case ITEM::CACTUS_WORKBENCH:

		if (m_pItem->GetType() == ITEM_TYPE::AXE)
			return true;

		// 도끼.
		break;

	case ITEM::CACTUS_WALL:
	case ITEM::WOOD_WALL:
	case ITEM::STONE_WALL:
	case ITEM::DIRT_WALL:

		if (m_pItem->GetType() == ITEM_TYPE::HAMMER)
			return true;

		break;
	}

	return false;
}

bool CPlayerScript::IsFall() const
{
	// 점프 모션인지를 확인하여 Fall이라고 인식한다.
	if (GetAnimator2D()->GetCurAnim() == L"PlayerJump")
		return true;

	return false;
}

void CPlayerScript::SetHelmet(CArmorScript * pHelmet)
{
	m_pHelmet = pHelmet;

	SetDir(GetDir());
	SetArmor();
}

void CPlayerScript::SetArmor(CArmorScript * pArmor)
{
	m_pArmor = pArmor;

	SetDir(GetDir());
	SetArmor();

}

void CPlayerScript::SetLeggings(CArmorScript * pLeggings)
{
	m_pLeggings = pLeggings;

	SetDir(GetDir());
	SetArmor();
}

void CPlayerScript::SetDir(int iDir)
{
	CDirectionScript::SetDir(iDir);

	if (m_pHelmet != nullptr)
	{
		m_pHelmet->SetDir(-iDir);
	}

	if (m_pArmor != nullptr)
	{
		m_pArmor->SetDir(-iDir);
	}

	if (m_pLeggings != nullptr)
	{
		m_pLeggings->SetDir(-iDir);
	}
}

bool CPlayerScript::IsJump() const
{
	return m_pJump->IsJumping();
}

bool CPlayerScript::IsBench(CTileMap2D * pTileMap) const
{
	if (pTileMap == nullptr)
		return false;

	vector<BundleTile> vecBundle;
	CTileMapMgr::GetInst()->GetBundleTile(pTileMap, ITEM_TYPE::BENCH, vecBundle);

	Vec2 vPos = Vec2(GetTransform()->GetLocalPos());
	// 플레이어에 해당하는 Tile위치를 구한다.

	int iIdxX = pTileMap->GetTileIdxX(vPos.x);
	int iIdxY = pTileMap->GetTileIdxY(vPos.y);

	auto iter = vecBundle.begin();
	auto iterEnd = vecBundle.end();

	for (; iter != iterEnd; ++iter)
	{
		Vec2 vBenchPos = pTileMap->GetTileLT(iter->vecIdx[0].iX, iter->vecIdx[0].iY);
		vBenchPos.x += 16.f;
		vBenchPos.y -= 8.f;

		if (Vec2::Distance(vPos, vBenchPos) <= 150.f)
			return true;
	}

	return false;
}

void CPlayerScript::SetHandleItem(CItemScript * pItem)
{
	m_pItem = pItem;

	if (m_pItem && m_pItem->GetType() == ITEM_TYPE::TORCH)
		SetTorch(m_pItem->GetItem());
	else
	{
		m_bTorch = false;
		m_pTorch->SetEnable(false);
		return;
	}

	switch (m_pItem->GetItem())
	{
	case ITEM::CACTUS_PICKAXE:
		m_pInven->GetMouse()->SetDigTime(0.2f);
		break;
	case ITEM::COPPER_AXE:
		m_pInven->GetMouse()->SetDigTime(0.6f);

		break;
	case ITEM::COPPER_PICKAXE:
		m_pInven->GetMouse()->SetDigTime(0.6f);

		break;
	case ITEM::WOOD_HAMMER:
		m_pInven->GetMouse()->SetDigTime(0.3f);

		break;
	}
}

void CPlayerScript::Update()
{

	g_bBench = IsBench(CMouseScript::g_pTileMap);
	CInventoryScript::g_pCraft->ShowItem();

	// 공격받은 이후의 시간을 체크해서 8초 이후에 true를 주고..
	// 2.5초 간격으로 3의 체력을 회복.

	if (m_bAttacked)
	{
		m_fAttacked += DT;

		if (m_fAttacked >= 8.f)
		{
			m_fAttacked = 0.f;
			m_bAttacked = false;
		}
	}
	else if(GetHP() < GetHPMax())
	{
		// 공격을 받은 이후에 5초의 시간이 흐른 이후에... 이곳으로 들어와서.
		// 2초 간격으로 체력 2가 회복된다.

		m_fAttacked += DT;

		if (m_fAttacked >= 2.5f)
		{
			AddHP(3);
			m_fAttacked -= 2.5f;
		}
	}

	// 몬스터한테 맞은 이후에 Alpha처리를 해준다.
	
	if (m_bAlpha)
	{
		m_fAlphaTime += DT;
		m_fAlphaPart += DT;

		if (m_fAlphaPart >= 0.1f)
		{
			m_fAlphaPart -= 0.1f;

			if (m_fAlpha == 1.f) 
				m_fAlpha = 0.5f;
			else 
				m_fAlpha = 1.f;

			GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_2, &m_fAlpha);

			if (m_pHelmet)
				m_pHelmet->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_2, &m_fAlpha);

			if (m_pArmor)
				m_pArmor->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_2, &m_fAlpha);

			if (m_pLeggings)
				m_pLeggings->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_2, &m_fAlpha);
		}

		if (m_fAlphaTime >= m_fAlphaLimitTime)
		{
			m_fAlpha = 1.f;
			GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_2, &m_fAlpha);
			m_bAlpha = false;

			if (m_pHelmet)
				m_pHelmet->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_2, &m_fAlpha);

			if (m_pArmor)
				m_pArmor->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_2, &m_fAlpha);

			if (m_pLeggings)
				m_pLeggings->GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_2, &m_fAlpha);
		}
	}




	if (m_pFly->IsFlyaway())
		return;

	// IDLE 기본.
	// TOOL, JUMP, WALK, SWORD

	ItemOffsetUpdate();

	Input();


	Vec3 tLocalPos = GetTransform()->GetLocalPos();
	bool bMove = false;

	if (!IsSwayItem())
	{
		if (KEYPRESS(KEY_TYPE::KEY_D))
		{
			PlayAnimation(L"PlayerWalk");

			tLocalPos.x += DT * m_fSpeed;

			SetDir(1);
			bMove = true;

			if (tLocalPos.x < CMainCameraScript::g_vMapStart.x)
				tLocalPos.x = CMainCameraScript::g_vMapStart.x;
		}

		else if (KEYPRESS(KEY_TYPE::KEY_A))
		{
			PlayAnimation(L"PlayerWalk");
			tLocalPos.x -= DT * m_fSpeed;

			SetDir(-1);
			bMove = true;

			if (tLocalPos.x > CMainCameraScript::g_vMapEnd.x)
				tLocalPos.x = CMainCameraScript::g_vMapEnd.x;
		}

		// Moving
		if (!bMove/* && !m_pGravity->IsFall()*/)
		{
			PlayAnimation(GetAnimator2D()->GetDefaultAni());
		}

		// Jumping or falling
		if (m_pJump->IsJumping() || (m_pGravity->GetSecond() >= 0.05f))
		{
			PlayAnimation(L"PlayerJump");
		}
	}
	

	if (KEYPRESS(KEY_TYPE::KEY_LBTN) && !IsFall() 
		&& !CMouseScript::g_bUIColl && !CInventoryScript::g_bSelect)
		// 인벤토리와 마우스가 충돌인 경우 무시.
	{
		
		if (m_pItem && !IsSwayItem())
		{
			switch (GetItemType(m_pItem->GetItem()))
			{
				case ITEM_TYPE::WEAPON:
				case ITEM_TYPE::AXE:
				case ITEM_TYPE::PICKAXE:
				case ITEM_TYPE::HAMMER:
				case ITEM_TYPE::BLOCK:
				case ITEM_TYPE::WALL:
				{
					int iDir = GetDir();
					m_pItem->SetDir(-iDir); // 그림이 좌우대칭임.
					m_pItem->SwayItem(0.5f);

					// 도구 사용.
					PlayAnimation(L"PlayerTool");

				}
				break;
			}
		}

	}



	GetTransform()->SetLocalPos(tLocalPos);

	// Torch Obj Update
	if (m_bTorch)
	{
		m_pTorch->GetTransform()->SetLocalPos(tLocalPos);
	}

}

void CPlayerScript::LateUpdate()
{
	CDirectionScript::LateUpdate();

	
}

void CPlayerScript::Awake()
{
	// Script Find
	const vector<CScript*> vecScript = GetObj()->GetScriptVec();

	for (int i = 0; i < vecScript.size(); ++i)
	{
		switch ((SCRIPT_TYPE)vecScript[i]->GetScriptType())
		{
		case SCRIPT_TYPE::BACKGROUNDSCRIPT:
			break;
		case SCRIPT_TYPE::GRAVITYSCRIPT:
			m_pGravity = (CGravityScript*)vecScript[i];

			break;

		case SCRIPT_TYPE::FLYAWAYSCRIPT:
			m_pFly = (CFlyawayScript*)vecScript[i];

			break;

		case SCRIPT_TYPE::INVENTORYSCRIPT:
			break;
		case SCRIPT_TYPE::ITEMSCRIPT:
			break;
		case SCRIPT_TYPE::PLAYERSCRIPT:
			break;
		case SCRIPT_TYPE::ZOMBIESCRIPT:
			break;

		case SCRIPT_TYPE::JUMPSCRIPT:
			m_pJump = (CJumpScript*)vecScript[i];

			break;
		}

	}

	if (m_pJump == nullptr)
		assert(false);
	else if (m_pFly == nullptr)
		assert(false);

	// UI Object가 가지고 있는 Script를 가져온다.

	CScene* pScene = CSceneMgr::GetInst()->GetScene();
	CLayer* pUILayer = pScene->FindLayer(L"UI");

	if (pUILayer == nullptr)
		assert(false);

	const vector<CGameObject*>& vecObj = pUILayer->GetParentObjVec();

	for (size_t i = 0; i < vecObj.size(); ++i)
	{
		if (vecObj[i]->GetName() == L"Inventory")
		{
			m_pInven = (CInventoryScript*)vecObj[i]->FindScript((UINT)SCRIPT_TYPE::INVENTORYSCRIPT);
			break;
		}
	}

	if (m_pInven == nullptr)
		assert(false);

	vector<CGameObject*> vecFind;
	CSceneMgr::GetInst()->FindObjByTag(L"PlayerTorch", vecFind);

	if (vecFind.empty())
		assert(false);

	m_pTorch = vecFind[0];

	if (m_pTorch == nullptr)
		assert(false);

	m_pTorch->SetEnable(false);

	float fLight = 1.f;
	GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fLight);

	m_fAlpha = 1.f;
	GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_2, &m_fAlpha);
}

void CPlayerScript::TileCollision()
{
	// 타일과 충돌이 되었다면, 플레이어의 Transform또한 달라졌다는 소리이다. 
	// ArmorScript의 처리를 다시 해주자.(움직인 자리를 다시 따라가기)

	if (m_pHelmet != nullptr)
	{
		m_pHelmet->LateUpdate();
		m_pHelmet->GetTransform()->FinalUpdate();
	}

	if (m_pArmor != nullptr)
	{
		m_pArmor->LateUpdate();
		m_pArmor->GetTransform()->FinalUpdate();

	}

	if (m_pLeggings != nullptr)
	{
		m_pLeggings->LateUpdate();
		m_pLeggings->GetTransform()->FinalUpdate();
	}

	if (m_bTorch)
	{
		m_pTorch->GetTransform()->SetLocalPos(GetTransform()->GetLocalPos());
		m_pTorch->GetTransform()->FinalUpdate();
	}
}

void CPlayerScript::Input()
{
	int iSelect = -1;

	// 점프
	if (KEYPRESS(KEY_TYPE::KEY_SPACE))
	{

		// 점프가 가능한 조건.
		if (!m_pJump->IsJumping() && !m_pGravity->IsFall())
		{
			// 만약 점프를 한 시점에서 양 옆에 대해서 붙은 경우.. 처리하지 말자.
			if (GetTransform()->IsMoveX() == false)
				m_pJump->Jump(500.f, 50.f);
		}

	}

	// Inventory On / Off
	if (KEYDOWN(KEY_TYPE::KEY_ESC))
	{
		m_pInven->SetOpen(!m_pInven->IsOpen());
	}

	// 1 ~ 0 키입력을 통해서 Inventory의 아이템을 가져온다.

	else if (KEYDOWN(KEY_TYPE::KEY_1))
		iSelect = 0;
	else if (KEYDOWN(KEY_TYPE::KEY_2))
		iSelect = 1;
	else if (KEYDOWN(KEY_TYPE::KEY_3))
		iSelect = 2;
	else if (KEYDOWN(KEY_TYPE::KEY_4))
		iSelect = 3;
	else if (KEYDOWN(KEY_TYPE::KEY_5))
		iSelect = 4;
	else if (KEYDOWN(KEY_TYPE::KEY_6))
		iSelect = 5;
	else if (KEYDOWN(KEY_TYPE::KEY_7))
		iSelect = 6;
	else if (KEYDOWN(KEY_TYPE::KEY_8))
		iSelect = 7;
	else if (KEYDOWN(KEY_TYPE::KEY_9))
		iSelect = 8;
	else if (KEYDOWN(KEY_TYPE::KEY_0))
		iSelect = 9;

	if (iSelect != -1 && !IsSwayItem())
	{
		// 아이템을 바꿔야 한다.
		SetHandleItem(m_pInven->GetItem(iSelect));
		m_pInven->SetHandleItem(iSelect, 255, 242, 0);

		CResMgr::GetInst()->Play(L"Menu_Tick", CHANNEL_TYPE::EFFECT_1, EFFECT_VOLUME2);
	}

}

bool CPlayerScript::LoadFromScene(FILE * pFile)
{
	CDirectionScript::LoadFromScene(pFile);

	return true;
}

void CPlayerScript::Flyaway(const Vec3 & tDir, const Vec3 & tSpeed, const Vec3 & tChange, float fTime)
{
	m_pFly->AddFly(tDir, tSpeed, tChange, fTime);

	// 사실상 피격.
	// 들고 있는 무기에 대한 처리를 풀어버림.
	if (m_pItem)
		m_pItem->StopSway();

	PlayAnimation(L"PlayerJump");

	// Jump가 켜져 있었다면 이것을 꺼버린다.
	m_pJump->SetJumping(false);
}

bool CPlayerScript::SaveToScene(FILE * pFile)
{
	SetDir(1);
	CDirectionScript::SaveToScene(pFile);

	return true;
}

void CPlayerScript::ItemOffsetUpdate()
{
	if (!m_pItem)
		return;

	/*m_pItem->GetTransform()->SetLocalPos(GetTransform()->GetLocalPos());*/


	Vec3 tPos = GetTransform()->GetLocalPos();
	tPos.z = m_pItem->GetTransform()->GetLocalPos().z;

	if (GetAnimator2D()->GetCurAnim() == L"PlayerTool")
	{
		Vec3 tMove;
		//m_pItem->SetDir(-GetDir());

		// Animator2D Dlg를 만든다면, 원하는 프레임을 선택하여 멈출 수 있는 기능을 추가할 것이다.
		// 이때, 해당 프레임을 정지시키고 Offset을 조절하면 될 것이다.

		// 60
		// 135
		// 200
		// 220


		if (m_pItem->GetType() != ITEM_TYPE::BLOCK)
		{
			switch (m_pItem->GetItem())
			{
			default:

				switch (GetAnimator2D()->GetCurIdx())
				{
				case 0:
					tMove = Vec3(-28.f, 0.f, 0.f);

					break;

				case 1:
					tMove = Vec3(-10.f, 0.f, 0.f);
					break;

				case 2:
					tMove = Vec3(-8.f, -17.f, 0.f);
					break;

				case 3:
					tMove = Vec3(-8.f, -20.f, 0.f);
					if (!CResMgr::GetInst()->IsPlaying(CHANNEL_TYPE::SWAYITEM))
						CResMgr::GetInst()->Play(L"SwayItem", CHANNEL_TYPE::SWAYITEM, EFFECT_VOLUME);

					break;
				}


				break;


			case ITEM::CACTUS_SWORD:

				switch (GetAnimator2D()->GetCurIdx())
				{
				case 0:
					tMove = Vec3(-28.f, 0.f, 0.f);

					break;

				case 1:
					tMove = Vec3(-10.f, 0.f, 0.f);
					break;

				case 2:
					tMove = Vec3(-8.f, -17.f, 0.f);
					break;

				case 3:
					tMove = Vec3(-8.f, -20.f, 0.f);
					if (!CResMgr::GetInst()->IsPlaying(CHANNEL_TYPE::SWAYITEM))
						CResMgr::GetInst()->Play(L"SwayItem", CHANNEL_TYPE::SWAYITEM, EFFECT_VOLUME);

					break;
				}

				tMove += Vec3(-10.f, -10.f, 0.f);

				break;
			}
			
		}
		else
		{
			// block
			switch (GetAnimator2D()->GetCurIdx())
			{
			case 0:
				tMove = Vec3(-25.f, 0.f, 0.f);
				break;

			case 1:
				tMove = Vec3(-10.f, 0.f, 0.f);
				break;

			case 2:
				tMove = Vec3(-4.f, -10.f, 0.f);
				break;

			case 3:
				tMove = Vec3(-8.f, -15.f, 0.f);
				
				if(!CResMgr::GetInst()->IsPlaying(CHANNEL_TYPE::SWAYITEM))
				CResMgr::GetInst()->Play(L"SwayItem", CHANNEL_TYPE::SWAYITEM, EFFECT_VOLUME);

				break;
			}
		}


		tMove.x *= GetDir();
		m_pItem->GetTransform()->SetLocalPos(tPos + tMove);
		m_pItem->GetMeshRender()->SetEnable(true);

		CCollider2D* pItemCollider = m_pItem->GetObj()->GetCollider2D();

		if (pItemCollider)
		{
			pItemCollider->SetEnable(true);
		}
	}

}


int GetItemDamage(ITEM eItem)
{
	int iMin = 0;
	int iMax = 0;

	switch (eItem)
	{
	default:
		return 0;

	case ITEM::CACTUS_SWORD:
		iMin = CACTUS_SWORD_DMG_MIN;
		iMax = CACTUS_SWORD_DMG_MAX;
		break;

	case ITEM::COPPER_SWORD:
		iMin = COPPER_SWORD_DMG_MIN;
		iMax = COPPER_SWORD_DMG_MAX;
		break;


	case ITEM::COPPER_AXE:
	case ITEM::COPPER_PICKAXE:
	case ITEM::WOOD_HAMMER:
	case ITEM::CACTUS_PICKAXE:
		iMin = 1;
		iMax = 3;
		break;

	}

	return rand() % (iMax - iMin + 1) + iMin;
}
