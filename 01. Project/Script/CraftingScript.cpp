#include "stdafx.h"
#include "CraftingScript.h"

#include <Engine/Core.h>

#include "InventoryScript.h"
#include "PanelScript.h"
#include "PrototypeMgr.h"
#include "UIItemScript.h" 
#include "MouseScript.h"
#include "ItemScript.h"
#include "PlayerScript.h"

Crafting CCraftingScript::g_CraftInfo[(UINT)ITEM::CRAFTING_END - (UINT)ITEM::WORKBENCH];


CCraftingScript::CCraftingScript() :
	CScript((UINT)SCRIPT_TYPE::CRAFTINGSCRIPT),
	m_pInven(nullptr),
	m_pPanel(nullptr),
	m_pCraftObj(nullptr),
	m_pMouse(nullptr),
	m_pUIItemArr{},
	m_iCount(0),
	m_iCollIdx(0),
	m_iShowIdx(0),
	m_iShowCount(0)
{
}


CCraftingScript::~CCraftingScript()
{
}

CUIItemScript* CCraftingScript::FindPrevItem(const wstring & strItem)
{
	auto iter = m_mapItem.find(strItem);

	if (iter == m_mapItem.end())
		assert(false);
	// ���� �� ���� ���ǿ� ���� Ű�� ã�ƿ��� ����..

	return iter->second;
}

void CCraftingScript::SetItemPos(CUIItemScript * pUIScript)
{
	if (pUIScript == nullptr)
		return;

	static Vec2 tPanelSize = m_pPanel->GetPanelSize();
	static Vec2 tPos = Vec2(GetTransform()->GetLocalPos());

	Vec2 tStartPos = tPos + Vec2(0.f, -tPanelSize.y * m_iShowCount);
	CGameObject* pUIObj = pUIScript->GetObj();
	pUIObj->SetEnable(true);

	pUIObj->GetTransform()->SetLocalPos(Vec3(tStartPos.x + tPanelSize.x / 2.f, tStartPos.y - tPanelSize.y / 2.f, 10.f));

	++m_iShowCount;
}

void CCraftingScript::CraftingClick()
{
	if (KEYDOWN(KEY_TYPE::KEY_LBTN))
	{
		if (m_iCollIdx != -1)
		{
			int iNum = m_iCollIdx / 10;

			// �浹 �ε����� �ش��ϴ� �������� �����ؾ� �Ѵ�. 
			if (m_pUIItemArr[m_iShowIdx + iNum] != nullptr)
			{

				if (m_pUIItemArr[m_iShowIdx + iNum]->GetItem() == ITEM::NONE)
					assert(false);
				// ������ ���� ���� ����̴�. Type..
				// �������� ���̴� ���� ������ ������ ���̴�.

				// ���� ������ �����ۿ� �����Ͽ�, ���չ��� ���´�.
				const Crafting& tCraft = g_CraftInfo[GetIdx(m_pUIItemArr[m_iShowIdx + iNum]->GetItem())];

				// ��ᰡ ������ ��� ����ó��. (��ᰡ ������ �����ϱ� map�� �ش� ������ �� �ִٴ� �Ҹ��� �Ǿ���)

				// �������� ������ ���̰� ����� �۾�.
				for (UINT i = 0; i < 3; ++i)
				{
					if (tCraft.m_eItemArr[i] == ITEM::NONE)
						break;

					// ��Ḧ ����ϴ� ��쿡�� �� ����� ��뷮��ŭ�� �κ��丮���� ���ҽ�Ų��.
					m_pInven->UsedMtrl(tCraft.m_eItemArr[i], tCraft.m_iItemArr[i]);
				}

				// ��ᰡ �������ٸ� �̰��� �ݿ�.
				ShowItem();

				m_pInven->AddItem(tCraft.m_eResult, tCraft.m_iResult);

				CResMgr::GetInst()->Play(L"Grab", CHANNEL_TYPE::GRAB, EFFECT_VOLUME);
			}
		}
	}
}

CUIItemScript* CCraftingScript::SetItem(size_t iIdx, CUIItemScript * pUIScript)
{
	CUIItemScript* pPrevItem = m_pUIItemArr[iIdx];
	m_pUIItemArr[iIdx] = pUIScript;


	if ((m_iShowIdx + 4) >= iIdx && m_iShowIdx <= iIdx)
	{
		// �������� �ڸ��� �������� �ʿ䰡 �ִ�. 
		// �׷���, ���� �ε�����... ShowIdx�κ���..5���� ���� �ȿ� ����� Ȯ���ؾ� �Ѵ�.
		SetItemPos(pUIScript);
	}


	return pPrevItem;
}

void CCraftingScript::OpenCrafting()
{
	m_iShowIdx = 0;
	m_iShowCount = 0;

	if (CInventoryScript::g_bOpen)
	{
		m_iCount = 0;

		vector<wstring> vecString;

		// Panel�� ����Ѵ�.
		m_pPanel->SetPanel(0, 1);
		m_pPanel->SetPanel(10, 1);
		m_pPanel->SetPanel(20, 1);
		m_pPanel->SetPanel(30, 1);
		m_pPanel->SetPanel(40, 1);

		ShowItem();
	}
	else
	{
		// Panel�� ������� �ʴ´�.
		m_pPanel->SetPanel(0, 0);
		m_pPanel->SetPanel(10, 0);
		m_pPanel->SetPanel(20, 0);
		m_pPanel->SetPanel(30, 0);
		m_pPanel->SetPanel(40, 0);

		// ������ ����ϴ� Object�� ���� ó���� ���ش�.
		auto iter = m_mapItem.begin();
		auto iterEnd = m_mapItem.end();

		for (; iter != iterEnd; ++iter)
		{
			// ó������ nullptr�� ���� ���� ����.
			iter->second->GetObj()->SetEnable(false);
		}

		memset(m_pUIItemArr, 0, sizeof(void*) * 50);
	}

	m_pCraftObj->SetEnable(CInventoryScript::g_bOpen);
}

void CCraftingScript::ShowItem()
{
	if (CInventoryScript::g_bOpen == false)
		return;

	// Open ������ ���. ���� �� �ִ� �������� �ľ��ؼ�...
	// �̰��� ����ϴ� �ڵ带 �ۼ�.
	
	vector<wstring> vecString;
	
	// ���չ��� ����, �ش� �������� ���� �� �ִ��� �Ǵ��Ѵ�.

	for (size_t iInfo = 0; iInfo < (UINT)ITEM::CRAFTING_END - (UINT)ITEM::WORKBENCH; ++iInfo)
	{
		if (g_CraftInfo[iInfo].m_eResult != ITEM::NONE)
		{
			// Bench �ʿ� ���� : false��� �׳� ó���ϰ�, true�� Bench�� true���� Ȯ���Ѵ�.
			if (g_CraftInfo[iInfo].m_bBench && !CPlayerScript::g_bBench)
				continue;

			// ���չ��� �ִ�.
			bool bCreate = true;
			for (size_t iIdx = 0; iIdx < 3; ++iIdx)
			{
				ITEM eItem = g_CraftInfo[iInfo].m_eItemArr[iIdx];

				if (eItem != ITEM::NONE)
				{
					// ������ Ȯ���Ͽ�, ���� �� �ִ��� Ȯ���Ѵ�.
					if (CInventoryScript::GetMtrlCount(eItem) < g_CraftInfo[iInfo].m_iItemArr[iIdx])
					{
						// ����� ���� < �ʿ��� ����� ��.
						bCreate = false;
						break;
					}
				}
				else
					break;

			}

			// �߰��� none Item�� ���� ��쿡�� �� �̻� ���չ��� Ȯ���� �ʿ䰡 ����.
			// Ȥ�� 3���� ��Ḧ ��� Ȯ���ߴ�.

			if (bCreate)
			{
				// �ش� �������� ���� �� �ִ� ��쿡�� ... ������Ʈ�� �����ϱ� ���ؼ� string�� �޾Ƶд�.
				vecString.push_back(ITEM_NAME[(UINT)g_CraftInfo[iInfo].m_eResult]);
			}

		}
	}



	// ���� �� �ִ� ����� ���ؼ�, Map�� �����ϰ� Object�� ��ġ�� �����Ѵ�.
	if (m_iCount != vecString.size())
	{
		m_iCount = vecString.size();
		m_iShowIdx = 0; 
		m_iShowCount = 0;

		memset(m_pUIItemArr, 0, sizeof(void*) * 50);

		auto iter = m_mapItem.begin();
		auto iterEnd = m_mapItem.end();

		for (; iter != iterEnd; ++iter)
		{
			iter->second->GetObj()->SetEnable(false);
		}

		for (size_t i = 0; i < m_iCount; ++i)
		{
			CUIItemScript* pScript = FindPrevItem(vecString[i]);
			SetItem(i, pScript);
		}
	}
	
		// �����̶� ���� �� �ִ� �������� ���� ���� ��쿡�� ShowIdx�� ShowCount�� �ʱ�ȭ���� ����.


	

}


//
//	// ���� �� �ִ� ���.
//	
//
//	// �ش� �������� �����ؼ� �Ҵ��ϰ�, ������ �迭�� �־��ش�.
//	UINT iSize = vecString.size();
//
//	for (UINT i = 0; i < iSize; ++i)
//	{
//		// �ش� �������� ���� �� �ִ��� ���θ� �޾ƿ´�.
//		CUIItemScript* pItem = FindPrevItem(vecString[iSize]);
//
//		// ���� �� �մµ�, �� �������� ������ ������ �� �������̶��,  pass
//		if (pItem != nullptr)
//			continue;
//
//		// ���� �� �ִµ�, �� �������� ������ �����Ǿ� ���� �ʴٸ�, �����ϰ� Value�� ����..
//		CGameObject* pUIObj = CPrototypeMgr::GetInst()->CreateCloneObj(vecString[i] + L"UI", L"UIItem");
//		pUIObj->SetEnable(false);
//
//		if (pUIObj == nullptr)
//			continue;
//
//		m_mapItem.erase(vecString[iSize]);
//		m_mapItem.insert(make_pair(vecString[iSize], pItem));
//	}
//
//}

void CCraftingScript::Awake()
{
	// Script
	m_pPanel = (CPanelScript*)GetObj()->FindScript((UINT)SCRIPT_TYPE::PANELSCRIPT);

	if (m_pPanel == nullptr)
		assert(false);

	for (size_t i = 0; i < 50; ++i)
	{
		m_pPanel->SetPanel(i, 0);
	}

	vector<CGameObject*> vecFind;
	CSceneMgr::GetInst()->FindObjByTag(L"MouseUIObj", vecFind);

	if (vecFind.empty())
		assert(false);

	m_pPanel->SetTarget(vecFind[0]);

	// CraftObj
	vecFind.clear();
	CSceneMgr::GetInst()->FindObjByTag(L"CraftingObj", vecFind);

	if (vecFind.empty())
		assert(false);

	m_pCraftObj = vecFind[0];
	m_pCraftObj->SetEnable(CInventoryScript::g_bOpen);

	// Mouse Script
	vecFind.clear();
	CSceneMgr::GetInst()->FindObjByTag(L"Mouse", vecFind);

	if (vecFind.empty())
		assert(false);

	m_pMouse = (CMouseScript*)vecFind[0]->FindScript((UINT)SCRIPT_TYPE::MOUSESCRIPT);

	if (m_pMouse == nullptr)
		assert(false);


	vecFind.clear();
	CSceneMgr::GetInst()->FindObjByTag(L"Inventory", vecFind);

	if (vecFind.empty())
		assert(false);

	m_pInven = (CInventoryScript*)vecFind[0]->FindScript((UINT)SCRIPT_TYPE::INVENTORYSCRIPT);

	// ���չ� �߰�.

	g_CraftInfo[GetIdx(ITEM::WORKBENCH)] = Crafting(ITEM::WORKBENCH, 1, false, ITEM::WOOD, 5);
	g_CraftInfo[GetIdx(ITEM::CACTUS_WORKBENCH)] = Crafting(ITEM::CACTUS_WORKBENCH, 1, false, ITEM::CACTUS, 5);
	g_CraftInfo[GetIdx(ITEM::CACTUS_SWORD)] = Crafting(ITEM::CACTUS_SWORD, 1, true, ITEM::CACTUS, 3);
	g_CraftInfo[GetIdx(ITEM::CACTUS_PICKAXE)] = Crafting(ITEM::CACTUS_PICKAXE, 1, true, ITEM::CACTUS, 3);

	// ��
	g_CraftInfo[GetIdx(ITEM::CACTUS_HELMET)] = Crafting(ITEM::CACTUS_HELMET, 1, true, ITEM::CACTUS, 3);
	g_CraftInfo[GetIdx(ITEM::CACTUS_ARMOR)] = Crafting(ITEM::CACTUS_ARMOR, 1, true, ITEM::CACTUS, 3);
	g_CraftInfo[GetIdx(ITEM::CACTUS_LEGGINGS)] = Crafting(ITEM::CACTUS_LEGGINGS, 1, true, ITEM::CACTUS, 3);

	// ȶ��
	g_CraftInfo[GetIdx(ITEM::TORCH)] =			 Crafting(ITEM::TORCH, 3, false, ITEM::WOOD, 1, ITEM::GEL, 1);
	g_CraftInfo[GetIdx(ITEM::VIOLET_TORCH)] =	 Crafting(ITEM::VIOLET_TORCH, 3, false, ITEM::WOOD, 1, ITEM::GEL, 1);
	g_CraftInfo[GetIdx(ITEM::YELLOW_TORCH)] =	 Crafting(ITEM::YELLOW_TORCH, 3, false, ITEM::WOOD, 1, ITEM::GEL, 1);
	g_CraftInfo[GetIdx(ITEM::BLUE_TORCH)] =		 Crafting(ITEM::BLUE_TORCH, 3, false, ITEM::WOOD, 1, ITEM::GEL, 1);
	g_CraftInfo[GetIdx(ITEM::GREEN_TORCH)] =	 Crafting(ITEM::GREEN_TORCH, 3, false, ITEM::WOOD, 1, ITEM::GEL, 1);
	g_CraftInfo[GetIdx(ITEM::RED_TORCH)] =		 Crafting(ITEM::RED_TORCH, 3, false, ITEM::WOOD, 1, ITEM::GEL, 1);
	g_CraftInfo[GetIdx(ITEM::ORANGE_TORCH)] =	 Crafting(ITEM::ORANGE_TORCH, 3, false, ITEM::WOOD, 1, ITEM::GEL, 1);
	g_CraftInfo[GetIdx(ITEM::WHITE_TORCH)] =	 Crafting(ITEM::WHITE_TORCH, 3, false, ITEM::WOOD, 1, ITEM::GEL, 1);
	g_CraftInfo[GetIdx(ITEM::PINK_TORCH)] =		 Crafting(ITEM::PINK_TORCH, 3, false, ITEM::WOOD, 1, ITEM::GEL, 1);
	g_CraftInfo[GetIdx(ITEM::ICE_TORCH)] =		 Crafting(ITEM::ICE_TORCH, 3, false, ITEM::WOOD, 1, ITEM::GEL, 1);

	// WALL
	g_CraftInfo[GetIdx(ITEM::DIRT_WALL)] = Crafting(ITEM::DIRT_WALL, 4, true, ITEM::DIRT, 1);
	g_CraftInfo[GetIdx(ITEM::STONE_WALL)] = Crafting(ITEM::STONE_WALL, 4, true, ITEM::STONE, 1);
	g_CraftInfo[GetIdx(ITEM::WOOD_WALL)] = Crafting(ITEM::WOOD_WALL, 4, true, ITEM::WOOD, 1);
	g_CraftInfo[GetIdx(ITEM::CACTUS_WALL)] = Crafting(ITEM::CACTUS_WALL, 4, true, ITEM::CACTUS, 1);

	// DOOR
	g_CraftInfo[GetIdx(ITEM::WOOD_DOOR)] = Crafting(ITEM::WOOD_DOOR, 1, true, ITEM::WOOD, 6);
	g_CraftInfo[GetIdx(ITEM::CACTUS_DOOR)] = Crafting(ITEM::CACTUS_DOOR, 1, true, ITEM::CACTUS, 6);

	// hammer
	g_CraftInfo[GetIdx(ITEM::WOOD_HAMMER)] = Crafting(ITEM::WOOD_HAMMER, 1, true, ITEM::WOOD, 4);


	m_mapItem.clear();

	// ���� �� �ִ� �����ۿ� ���� ������ �̸� �����ؼ� ��Ƶ���.
	// GameObject - UIItemScript
	for (UINT i = (UINT)ITEM::WORKBENCH; i < (UINT)ITEM::CRAFTING_END; ++i)
	{
		// ���ϱ� �����ϰ� ������ FLAG�̴�.
		if (i == (UINT)ITEM::GEL)
			continue;

		wstring strName = ITEM_NAME[i];
		CGameObject* pObj = CPrototypeMgr::GetInst()->CreateCloneObj(strName + L"UI", L"UIItem");

		// �������� �ʴ� ��� OR ������ flag
		if (pObj != nullptr)
		{
			pObj->SetEnable(false);
			CUIItemScript* pUIItem = (CUIItemScript*)pObj->FindScript((UINT)SCRIPT_TYPE::UIITEMSCRIPT);

			if (pUIItem == nullptr)
				assert(false);

			m_mapItem.insert(make_pair(ITEM_NAME[i], pUIItem));
		}
	}

}

void CCraftingScript::Update()
{
	// Crafting Click�� ���� ó���� �Ѵ�.
	CraftingClick();

	// Crafting  �ý��ۿ��� ���� �� �ִ� ���� ����� ������ ó���� �Ѵ�.

	if (CInventoryScript::g_bOpen && m_iCount)
	{
		// ���� ���� && ���� �� �ִ� �� �� ������.

		if (KEYDOWN(KEY_TYPE::KEY_DOWN))
		{
			
			// --Idx
			--m_iShowIdx;

			if (m_iShowIdx < 0)
				m_iShowIdx = 0;
			else
				CResMgr::GetInst()->Play(L"Menu_Tick", CHANNEL_TYPE::EFFECT_1, EFFECT_VOLUME2);


			m_iShowCount = 0;

			for (int i = 0; i < m_iCount; ++i)
			{
				m_pUIItemArr[i]->GetObj()->SetEnable(false);
			}

			for (int i = 0; i < 5; ++i)
			{
				SetItemPos(m_pUIItemArr[m_iShowIdx + i]);
			}
		}
		else if (KEYDOWN(KEY_TYPE::KEY_UP))
		{
			// ++Idx

			// max

			// ���� �� �ִ°�.. 5����. �׷���.. Idx = 0�� �ִ���..
			// 2��.. = 0�� �ִ�.
			// 12��. 7�� �ִ�. �������� �����̰� OK ?

			if (m_iCount <= 5)
			{
				m_iShowIdx = 0;
			}
			else
			{
				++m_iShowIdx;

				if (m_iShowIdx > m_iCount - 5)
				{
					m_iShowIdx = m_iCount - 5;
				}
				else
					CResMgr::GetInst()->Play(L"Menu_Tick", CHANNEL_TYPE::EFFECT_1, EFFECT_VOLUME2);

			}

			m_iShowCount = 0;
			for (int i = 0; i < m_iCount; ++i)
			{
				m_pUIItemArr[i]->GetObj()->SetEnable(false);
			}

			for (int i = 0; i < 5; ++i)
			{
				SetItemPos(m_pUIItemArr[m_iShowIdx + i]);
			}

		}
	}
}

void CCraftingScript::Render()
{
	static wchar_t str[50] = {};

	if (CInventoryScript::g_bOpen)
	{
		// CraftingObj ���� �۾�
		lstrcpyW(str, L"Crafting");
		CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, str, 85.f, 435.f, 24.f, FONT_RGBA(255, 255, 255, 255));
	}
	
	if (m_pPanel != nullptr)
	{
		m_iCollIdx = m_pPanel->GetCollIdx();

#ifdef _DEBUG
		wsprintf(str, L"CollIdx : %d", m_pPanel->GetCollIdx());
		CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, str, 50.f, 600.f, 20.f, FONT_RGBA(255, 255, 255, 255));
#endif

		Vec2 tOutputPos = m_pMouse->GetOutputPos();


		if (m_iCollIdx != -1 && !m_pMouse->GetEquipItem()) // ���̴� ������ ����� �Ǿ��ְ�, �̰��� m_iShowIdx ~�̴�.
		{
			CMouseScript::g_bUIColl = true;
			int iNum = m_iCollIdx / 10; // n��°.

			// ���콺�� �浹�� Item 
			if (m_pUIItemArr[m_iShowIdx + iNum] != nullptr)
			{
				// �̸� ���
				lstrcpyW(str, m_pUIItemArr[m_iShowIdx + iNum]->GetObj()->GetName().c_str());
				CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, str, tOutputPos.x, tOutputPos.y, 20.f, FONT_RGBA(255, 255, 255, 255));

				// �����ϰ��� �ϴ� Item�� Type�� ������ �ε����� ���Ѵ�.
				int iItem = GetIdx(m_pUIItemArr[m_iShowIdx + iNum]->GetItem());

				// ��Ḧ ������ش�.
				for (int i = 0; i < 3; ++i)
				{
					if (g_CraftInfo[iItem].m_eItemArr[i] == ITEM::NONE)
						break;

					// � ��Ḧ ��ŭ �ʿ�� �ϴ��� ���.
					wsprintf(str, L"%s : %d", ITEM_NAME[(UINT)g_CraftInfo[iItem].m_eItemArr[i]], g_CraftInfo[iItem].m_iItemArr[i]);
					CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, str, tOutputPos.x, tOutputPos.y + (1 + i) * 20.f, 20.f, FONT_RGBA(255, 255, 255, 255));
				}

			}
		}

		// CraftingVec =  CraftingLT - ī�޶��� LT
		static Vec2 tVec = Vec2(GetTransform()->GetLocalPos()) -
			Vec2(-CCore::GetInst()->GetRes().fWidth / 2.f, CCore::GetInst()->GetRes().fHeight / 2.f);
		static Vec2 tPanelSize = Vec2(GetTransform()->GetLocalScale()) / Vec2(10.f, 5.f);

		// ���� �������� �ִ� �����ۿ� ���ؼ� ���� ������ ����ϵ��� �Ѵ�.
		for (int i = 0; i < 5; ++i)
		{
			if (!m_pUIItemArr[m_iShowIdx + i])
				break;

			// �ش� �ڸ� Item�� Type�� ������ �ε����� ���Ѵ�.
			int iItem = GetIdx(m_pUIItemArr[m_iShowIdx + i]->GetItem());
			
			if (g_CraftInfo[iItem].m_iResult == 1)
				continue;

			Vec2 tLT = tVec;
			tLT.y -= i * tPanelSize.y;
			tLT.y *= -1.f;

				wsprintf(str, L"%d", g_CraftInfo[iItem].m_iResult);
			CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, str,
				tLT.x + 9.f, tLT.y + 23.f, 20.f, FONT_RGBA(255, 255, 255, 255));
		}

	}
	
}
