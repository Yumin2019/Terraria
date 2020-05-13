#include "stdafx.h"
#include "ItemObjScript.h"
#include "UIItemScript.h"
#include "ItemScript.h"
#include "FlyawayScript.h"
#include "GravityScript.h"
#include "InventoryScript.h"
#include "PlayerScript.h"
#include "CraftingScript.h"

#include "PrototypeMgr.h"
#include "PoolMgr.h"
#include "UtilMgr.h"



CItemObjScript::CItemObjScript() :
	CScript((UINT)SCRIPT_TYPE::ITEMOBJSCRIPT),
	m_eItem(ITEM::NONE),
	m_bFly(false),
	m_bJump(false),
	m_pFly(nullptr),
	m_pGravity(nullptr),
	m_pPlayer(nullptr),
	m_iCount(1)

{
}


CItemObjScript::~CItemObjScript()
{
}

void CItemObjScript::Awake()
{
	const vector<CScript*>& vecScript = GetObj()->GetScriptVec();

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		if (vecScript[i]->GetScriptType() == (UINT)SCRIPT_TYPE::FLYAWAYSCRIPT)
		{
			m_pFly = (CFlyawayScript*)vecScript[i];
			
		}
		else if (vecScript[i]->GetScriptType() == (UINT)SCRIPT_TYPE::GRAVITYSCRIPT)
		{
			m_pGravity = (CGravityScript*)vecScript[i];
		}
	}

	if (m_pFly == nullptr || m_pGravity == nullptr)
		assert(false);

	vector<CGameObject*> vecObj;
	CSceneMgr::GetInst()->FindObjByTag(L"Player", vecObj);

	if (vecObj.empty())
		assert(false);

	m_pPlayer = vecObj[0];



	wstring str = GetObj()->GetName();
	str = str.substr(0, str.length() - 3);

	for (UINT i = 0; i < (UINT)ITEM::CRAFTING_END; ++i)
	{
		if (str == ITEM_NAME[i])
		{
			m_eType = GetItemType((ITEM)i);
			m_eItem = (ITEM)i;
			break;
		}
	}

	if (m_eType == ITEM_TYPE::NONE)
		assert(false);


	float fLight = 1.f;
	GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fLight);


	if (m_eType == ITEM_TYPE::WALL)
	{
		switch (m_eItem)
		{
		case ITEM::WOOD_WALL:
		case ITEM::STONE_WALL:
			fLight = 0.6f;
			break;

		case ITEM::DIRT_WALL:
			fLight = 0.35f;
			break;

		case ITEM::CACTUS_WALL: 
			fLight = 0.4f;

			break;

		default:
			assert(false);
			break;
		}
	}
	else
		fLight = 0.8f;
	
	GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_1, &fLight);
}

void CItemObjScript::Start()
{
	m_bFly = false;
	m_bJump = false;

	m_pFly->Clear();
	m_pGravity->SetEnable(true);
	GetCollider2D()->SetEnable(true);
}

void CItemObjScript::Update()
{
	// NOT Jump && TileCollision 
	if (!m_bJump)
	{
		// 적절한 위치에 셋팅이 되었다는 가정하에 !
		m_pFly->AddFly(Vec3(0.f, 1.f, 0.f), Vec3(0.f, 500.f, 0.f), Vec3(0.f, 50.f, 0.f), 1.f);
		m_bJump = true;
	}


	Vec2 tPlayerPos = Vec2(m_pPlayer->GetTransform()->GetLocalPos());
	Vec2 tItemPos = Vec2(GetTransform()->GetLocalPos());
	float fDist = Vec2::Distance(tPlayerPos, tItemPos);

	if (m_bFly == false)
	{
		if (fDist < 70.f)
		{
			// 일정 거리에 가까워 지는 경우에 플레이어에게 다가가면서, 중력의 처리를 받지 않고,
			// Tile 충돌도 꺼진다. (그냥, Layer를 옮겨버려 ?) 처리하지 말게...
			m_pGravity->SetEnable(false); // 중력을 꺼버린다. 더 이상 떨어지지 않음.
			GetCollider2D()->SetEnable(false); // 더 이상 Tile의 영향을 받지 않음.

			m_bFly = true;
		}
	}
	else
	{

		// Fly To Player
		float fTime = DT;

		// Item에서 Player로 가는 Vec2를 구한다.
		Vec2 tVec = tPlayerPos - tItemPos;

		// 이 벡터를 정규화한다.
		tVec.Normalize();

		// 해당 벡터의 방향으로 50의 속도로 이동.
		GetTransform()->SetLocalPos(tItemPos + tVec * 500.f * DT);



		if (fDist <= 10.f)
		{
			// 플레이어의 인벤토리 스크립트
			CPlayerScript* pPlayer = (CPlayerScript*)m_pPlayer->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT);
			CInventoryScript* pInven = pPlayer->GetInven();
			pInven->AddItem(m_eItem, m_iCount);

			// 무기 / 방어구 등의 아이템은 하나의 아이템이 하나의 UI를 담당하게 된다.
			// 아직은 지원하지 않음.

			switch (m_eItem)
			{
			case ITEM::GEL:
			case ITEM::CACTUS:
			case ITEM::WOOD:
				GetTransform()->SetLocalPos(Vec3(0.f, 0.f, 90.f));
				CPoolMgr::GetInst()->ReturnObj(GetListItem(m_eItem), this);

				break;

			default:
				CScript::DeleteObject(GetObj());
				// 아이템이 없어지는 처리를 한다. (ITEMOBJ)

				GetObj()->SetEnable(false); // 관련된 모든 처리를 끄겠다.
				break;
			}


			if(!CResMgr::GetInst()->IsPlaying(CHANNEL_TYPE::GRAB))
				CResMgr::GetInst()->Play(L"Grab", CHANNEL_TYPE::GRAB, EFFECT_VOLUME);


			// UtilMgr에게 ItemFont를 생성하도록 요청한다.
			//Vec2 vOffset = Vec2(rand() % 3 - 1 * 50.f, );
			tPlayerPos.x -= 25.f;
			CUtilMgr::GetInst()->AddItem(m_eItem, m_iCount, tPlayerPos);

		}

	}


}
