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
	// 만들 수 없는 물건에 대한 키를 찾아오고 있음..

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

			// 충돌 인덱스에 해당하는 아이템이 존재해야 한다. 
			if (m_pUIItemArr[m_iShowIdx + iNum] != nullptr)
			{

				if (m_pUIItemArr[m_iShowIdx + iNum]->GetItem() == ITEM::NONE)
					assert(false);
				// 설정이 되지 않은 경우이다. Type..
				// 아이템이 보이는 경우는 조합이 가능한 것이다.

				// 현재 선택한 아이템에 접근하여, 조합법을 얻어온다.
				const Crafting& tCraft = g_CraftInfo[GetIdx(m_pUIItemArr[m_iShowIdx + iNum]->GetItem())];

				// 재료가 부족한 경우 예외처리. (재료가 무조건 있으니까 map에 해당 정보가 들어가 있다는 소리로 되야지)

				// 아이템의 개수를 줄이고 만드는 작업.
				for (UINT i = 0; i < 3; ++i)
				{
					if (tCraft.m_eItemArr[i] == ITEM::NONE)
						break;

					// 재료를 사용하는 경우에는 이 재료의 사용량만큼을 인벤토리에서 감소시킨다.
					m_pInven->UsedMtrl(tCraft.m_eItemArr[i], tCraft.m_iItemArr[i]);
				}

				// 재료가 떨어졌다면 이것을 반영.
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
		// 아이템의 자리를 정렬해줄 필요가 있다. 
		// 그런데, 현재 인덱스가... ShowIdx로부터..5개의 범위 안에 드는지 확인해야 한다.
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

		// Panel을 출력한다.
		m_pPanel->SetPanel(0, 1);
		m_pPanel->SetPanel(10, 1);
		m_pPanel->SetPanel(20, 1);
		m_pPanel->SetPanel(30, 1);
		m_pPanel->SetPanel(40, 1);

		ShowItem();
	}
	else
	{
		// Panel을 출력하지 않는다.
		m_pPanel->SetPanel(0, 0);
		m_pPanel->SetPanel(10, 0);
		m_pPanel->SetPanel(20, 0);
		m_pPanel->SetPanel(30, 0);
		m_pPanel->SetPanel(40, 0);

		// 이전에 출력하던 Object를 끄는 처리를 해준다.
		auto iter = m_mapItem.begin();
		auto iterEnd = m_mapItem.end();

		for (; iter != iterEnd; ++iter)
		{
			// 처음부터 nullptr인 것은 넣지 않음.
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

	// Open 상태인 경우. 만들 수 있는 아이템을 파악해서...
	// 이것을 출력하는 코드를 작성.
	
	vector<wstring> vecString;
	
	// 조합법을 보고, 해당 아이템을 만들 수 있는지 판단한다.

	for (size_t iInfo = 0; iInfo < (UINT)ITEM::CRAFTING_END - (UINT)ITEM::WORKBENCH; ++iInfo)
	{
		if (g_CraftInfo[iInfo].m_eResult != ITEM::NONE)
		{
			// Bench 필요 여부 : false라면 그냥 처리하고, true면 Bench가 true인지 확인한다.
			if (g_CraftInfo[iInfo].m_bBench && !CPlayerScript::g_bBench)
				continue;

			// 조합법이 있다.
			bool bCreate = true;
			for (size_t iIdx = 0; iIdx < 3; ++iIdx)
			{
				ITEM eItem = g_CraftInfo[iInfo].m_eItemArr[iIdx];

				if (eItem != ITEM::NONE)
				{
					// 개수를 확인하여, 만들 수 있는지 확인한다.
					if (CInventoryScript::GetMtrlCount(eItem) < g_CraftInfo[iInfo].m_iItemArr[iIdx])
					{
						// 재료의 개수 < 필요한 재료의 수.
						bCreate = false;
						break;
					}
				}
				else
					break;

			}

			// 중간에 none Item이 나온 경우에는 더 이상 조합법을 확인할 필요가 없다.
			// 혹은 3개의 재료를 모두 확인했다.

			if (bCreate)
			{
				// 해당 아이템을 만들 수 있는 경우에는 ... 오브젝트를 생성하기 위해서 string을 받아둔다.
				vecString.push_back(ITEM_NAME[(UINT)g_CraftInfo[iInfo].m_eResult]);
			}

		}
	}



	// 만들 수 있는 목록을 통해서, Map에 접근하고 Object의 위치를 설정한다.
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
	
		// 기존이랑 만들 수 있는 아이템의 수가 같은 경우에는 ShowIdx와 ShowCount를 초기화하지 말자.


	

}


//
//	// 나올 수 있는 경우.
//	
//
//	// 해당 아이템을 설정해서 할당하고, 아이템 배열에 넣어준다.
//	UINT iSize = vecString.size();
//
//	for (UINT i = 0; i < iSize; ++i)
//	{
//		// 해당 아이템을 만들 수 있는지 여부를 받아온다.
//		CUIItemScript* pItem = FindPrevItem(vecString[iSize]);
//
//		// 만들 수 잇는데, 이 아이템이 기존에 생성이 된 아이템이라면,  pass
//		if (pItem != nullptr)
//			continue;
//
//		// 만들 수 있는데, 이 아이템이 기존에 생성되어 있지 않다면, 생성하고 Value를 변경..
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

	// 조합법 추가.

	g_CraftInfo[GetIdx(ITEM::WORKBENCH)] = Crafting(ITEM::WORKBENCH, 1, false, ITEM::WOOD, 5);
	g_CraftInfo[GetIdx(ITEM::CACTUS_WORKBENCH)] = Crafting(ITEM::CACTUS_WORKBENCH, 1, false, ITEM::CACTUS, 5);
	g_CraftInfo[GetIdx(ITEM::CACTUS_SWORD)] = Crafting(ITEM::CACTUS_SWORD, 1, true, ITEM::CACTUS, 3);
	g_CraftInfo[GetIdx(ITEM::CACTUS_PICKAXE)] = Crafting(ITEM::CACTUS_PICKAXE, 1, true, ITEM::CACTUS, 3);

	// 방어구
	g_CraftInfo[GetIdx(ITEM::CACTUS_HELMET)] = Crafting(ITEM::CACTUS_HELMET, 1, true, ITEM::CACTUS, 3);
	g_CraftInfo[GetIdx(ITEM::CACTUS_ARMOR)] = Crafting(ITEM::CACTUS_ARMOR, 1, true, ITEM::CACTUS, 3);
	g_CraftInfo[GetIdx(ITEM::CACTUS_LEGGINGS)] = Crafting(ITEM::CACTUS_LEGGINGS, 1, true, ITEM::CACTUS, 3);

	// 횃불
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

	// 만들 수 있는 아이템에 대한 정보를 미리 생성해서 담아두자.
	// GameObject - UIItemScript
	for (UINT i = (UINT)ITEM::WORKBENCH; i < (UINT)ITEM::CRAFTING_END; ++i)
	{
		// 보니까 유일하게 버리는 FLAG이다.
		if (i == (UINT)ITEM::GEL)
			continue;

		wstring strName = ITEM_NAME[i];
		CGameObject* pObj = CPrototypeMgr::GetInst()->CreateCloneObj(strName + L"UI", L"UIItem");

		// 지원하지 않는 경우 OR 버리는 flag
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
	// Crafting Click에 대한 처리를 한다.
	CraftingClick();

	// Crafting  시스템에서 만들 수 있는 조합 목록을 내리는 처리를 한다.

	if (CInventoryScript::g_bOpen && m_iCount)
	{
		// 열린 상태 && 만들 수 있는 것 이 존재함.

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

			// 만들 수 있는게.. 5개야. 그러면.. Idx = 0이 최대지..
			// 2개.. = 0이 최대.
			// 12개. 7이 최대. 나머지는 공백이고 OK ?

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
		// CraftingObj 위에 글씨
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


		if (m_iCollIdx != -1 && !m_pMouse->GetEquipItem()) // 보이는 영역에 출력이 되어있고, 이것은 m_iShowIdx ~이다.
		{
			CMouseScript::g_bUIColl = true;
			int iNum = m_iCollIdx / 10; // n번째.

			// 마우스와 충돌한 Item 
			if (m_pUIItemArr[m_iShowIdx + iNum] != nullptr)
			{
				// 이름 출력
				lstrcpyW(str, m_pUIItemArr[m_iShowIdx + iNum]->GetObj()->GetName().c_str());
				CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, str, tOutputPos.x, tOutputPos.y, 20.f, FONT_RGBA(255, 255, 255, 255));

				// 제작하고자 하는 Item의 Type을 가지고 인덱스를 구한다.
				int iItem = GetIdx(m_pUIItemArr[m_iShowIdx + iNum]->GetItem());

				// 재료를 출력해준다.
				for (int i = 0; i < 3; ++i)
				{
					if (g_CraftInfo[iItem].m_eItemArr[i] == ITEM::NONE)
						break;

					// 어떤 재료를 얼만큼 필요로 하는지 출력.
					wsprintf(str, L"%s : %d", ITEM_NAME[(UINT)g_CraftInfo[iItem].m_eItemArr[i]], g_CraftInfo[iItem].m_iItemArr[i]);
					CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, str, tOutputPos.x, tOutputPos.y + (1 + i) * 20.f, 20.f, FONT_RGBA(255, 255, 255, 255));
				}

			}
		}

		// CraftingVec =  CraftingLT - 카메라의 LT
		static Vec2 tVec = Vec2(GetTransform()->GetLocalPos()) -
			Vec2(-CCore::GetInst()->GetRes().fWidth / 2.f, CCore::GetInst()->GetRes().fHeight / 2.f);
		static Vec2 tPanelSize = Vec2(GetTransform()->GetLocalScale()) / Vec2(10.f, 5.f);

		// 지금 보여지고 있는 아이템에 대해서 제작 개수를 출력하도록 한다.
		for (int i = 0; i < 5; ++i)
		{
			if (!m_pUIItemArr[m_iShowIdx + i])
				break;

			// 해당 자리 Item의 Type을 가지고 인덱스를 구한다.
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
