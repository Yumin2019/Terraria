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
		// ������ ��ġ�� ������ �Ǿ��ٴ� �����Ͽ� !
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
			// ���� �Ÿ��� ����� ���� ��쿡 �÷��̾�� �ٰ����鼭, �߷��� ó���� ���� �ʰ�,
			// Tile �浹�� ������. (�׳�, Layer�� �Űܹ��� ?) ó������ ����...
			m_pGravity->SetEnable(false); // �߷��� ��������. �� �̻� �������� ����.
			GetCollider2D()->SetEnable(false); // �� �̻� Tile�� ������ ���� ����.

			m_bFly = true;
		}
	}
	else
	{

		// Fly To Player
		float fTime = DT;

		// Item���� Player�� ���� Vec2�� ���Ѵ�.
		Vec2 tVec = tPlayerPos - tItemPos;

		// �� ���͸� ����ȭ�Ѵ�.
		tVec.Normalize();

		// �ش� ������ �������� 50�� �ӵ��� �̵�.
		GetTransform()->SetLocalPos(tItemPos + tVec * 500.f * DT);



		if (fDist <= 10.f)
		{
			// �÷��̾��� �κ��丮 ��ũ��Ʈ
			CPlayerScript* pPlayer = (CPlayerScript*)m_pPlayer->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT);
			CInventoryScript* pInven = pPlayer->GetInven();
			pInven->AddItem(m_eItem, m_iCount);

			// ���� / �� ���� �������� �ϳ��� �������� �ϳ��� UI�� ����ϰ� �ȴ�.
			// ������ �������� ����.

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
				// �������� �������� ó���� �Ѵ�. (ITEMOBJ)

				GetObj()->SetEnable(false); // ���õ� ��� ó���� ���ڴ�.
				break;
			}


			if(!CResMgr::GetInst()->IsPlaying(CHANNEL_TYPE::GRAB))
				CResMgr::GetInst()->Play(L"Grab", CHANNEL_TYPE::GRAB, EFFECT_VOLUME);


			// UtilMgr���� ItemFont�� �����ϵ��� ��û�Ѵ�.
			//Vec2 vOffset = Vec2(rand() % 3 - 1 * 50.f, );
			tPlayerPos.x -= 25.f;
			CUtilMgr::GetInst()->AddItem(m_eItem, m_iCount, tPlayerPos);

		}

	}


}
