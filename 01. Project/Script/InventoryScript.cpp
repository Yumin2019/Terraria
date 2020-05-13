#include "stdafx.h"
#include "InventoryScript.h"
#include "ItemScript.h"
#include "PanelScript.h"
#include "ScaleScript.h"
#include "UIItemScript.h"
#include "MouseScript.h"
#include "PlayerScript.h"
#include "CraftingScript.h"
#include "EquipmentScript.h"
#include "ArmorScript.h"

#include "PrototypeMgr.h"


#include <Engine/Scene.h>
#include <Engine/SceneMgr.h>
#include <Engine/Layer.h>
#include <Engine/GameObject.h>
#include <Engine/Core.h>

bool CInventoryScript::g_bOpen;
bool CInventoryScript::g_bSelect;
int CInventoryScript::g_arrMtrlCount[(UINT)ITEM::NUM_END] = {};
CCraftingScript* CInventoryScript::g_pCraft;

CInventoryScript::CInventoryScript() :
	m_pItemArr{},
	CScript((UINT)SCRIPT_TYPE::INVENTORYSCRIPT),
	m_iSelectItem(-1),
	m_pMouse(nullptr),
	m_pEquipment(nullptr),
	m_pPlayer(nullptr),
	m_pArmorPanel(nullptr)
{
}


CInventoryScript::~CInventoryScript()
{
}

int CInventoryScript::GetMtrlCount(ITEM eItem)
{
	return g_arrMtrlCount[(UINT)eItem];
}

void CInventoryScript::SetMtrlCount(ITEM eItem, int iCount)
{
	g_arrMtrlCount[(UINT)eItem] = iCount;
	g_pCraft->ShowItem();
}

void CInventoryScript::AddMtrlCount(ITEM eItem, int iCount)
{
	g_arrMtrlCount[(UINT)eItem] += iCount;
	g_pCraft->ShowItem();
}

void CInventoryScript::SubMtrlCount(ITEM eItem, int iCount)
{
	g_arrMtrlCount[(UINT)eItem] -= iCount;
	g_pCraft->ShowItem();
}

void CInventoryScript::UsedMtrl(ITEM eMtrl, int iUseCount)
{
	if (iUseCount == 0)
		return;

	// 애초에 조합 시스템이 만들 수 있는 경우에 아이템을 출력하는 방식이기 때문에 재료가 없는 경우에 아이템이 출력되지 않을 것이다.
	// 따라서, 아이템을 클릭할 수 있다는 소리는 해당 아이템을 만들 재료가 있다는 소리가 된다.

	// 만약에 UseCount = 5개이고, Item = dirt이다.
	// 인벤토리에 Dirt아이템이 1개가 있는 경우 : 하나에서 5개를 뺀다.
	//(여기서 음수가 나오는 경우 assert 처리를 하면 되고, 0이라면 없애는 처리를 해준다)

	InvenSlot tSlot;

	// 해당 아이템을 가지는 자리와 ItemScript* 를 가지고 있도록 한다.
	for (UINT i = 0; i < 50; ++i)
	{
		if (m_pItemArr[i] != nullptr)
		{
			if (m_pItemArr[i]->GetItem() == eMtrl)
			{
				tSlot = InvenSlot(i, m_pItemArr[i]);
				break;
			}
		}
	}

	// 아이템 슬롯은 하나라고 생각한다.

	int iCount = GetMtrlCount(eMtrl);
	iCount -= iUseCount;

	// 필요한 만큼을 깟는데, 0보다 크다. 끝. 상황종료.
	if (iCount > 0)
	{
		// 빼진 만큼을 Update해준다.
		SetMtrlCount(eMtrl, iCount);
	}

	else if (iCount == 0)
	{
		// 0인 경우.
		// 이 아이템의 개수와 필요한 재료의 개수가 동일했다.
		// 해당 아이템만 없애고 끝낸다.
		ClearItem(tSlot.iIdx);
		SetMtrlCount(eMtrl, iCount);
	}
	else
	{
		// 음수인 경우 : 나는 5개가 있는데, 6개를 빼려고 한 것이다. 동기화가 전혀 되지 않았다고 볼 수 있다.
		assert(false);
	}



}

void CInventoryScript::InventoryClick()
{
	if (CInventoryScript::g_bSelect)
	{
		m_fSecond += DT;

		if (m_fSecond >= 0.2f)
			CInventoryScript::g_bSelect = false;
	}

	if (KEYDOWN(KEY_TYPE::KEY_LBTN))
	{
		// 일단, Close 상태와 Open 상태 상관없이, 마우스로 클릭을 하는 경우에는 아이템이 셋팅이 된다.
		// Close 상태에서 Mouse가 아이템을 달고 다니는 것은 2차적인 내용이고, 일단 아이템을 셋팅한다.

		if (m_iCollIdx != -1)
		{
			
			CResMgr::GetInst()->Play(L"Grab", CHANNEL_TYPE::GRAB, EFFECT_VOLUME);

			// Open 상태에서는 Item이 마우스를 따라다니도록 처리해야 한다.
			// Mouse가 특정 아이템을 들고 있는 상태에서 Close 상태가 된 경우에 다시 풀어버리고, 자리는 바뀌게 된다.
			// 이것으로 볼 때... 마우스가 들고 있던 아이템을 Add해서 처리를 하는 것으로 보인다.
			
			/*
			1. Close 상태에서 마우스를 이용해서 아이템을 클릭한 경우 : 단순히 아이템을 장착한다. (이것이 끝이다)
			2. Open 상태에서 마우스를 이용해서 아이템을 클릭한다 : 아이템을 장착한다.
			해당 아이템의 자리는 nullptr 처리를 한다. (Mouse가 이것을 들고 있는다)



			(단지 출력만 마우스를 기준으로 하다가, 처리하는 방식이 아니라고 생각한 것이... Mouse에 아이템을 장착한
			상태로 Close 상태로 만들면, 해당 자리로 아이템이 들어가는 것이 아니라, AddItem으로 처리가 되고 있었다,
			이것은 인벤토리의 자리에서 null 처리를 하고 마우스가 이것을 들고 있는다는 것이다.
			*/
			if (g_bOpen)
			{
				// 마우스가 장착한 아이템이 없다.
				if (m_pMouse->GetEquipItem() == nullptr)
				{
					// 클릭한 곳이 아이템이 존재한다면, 이 아이템을 Mouse가 들도록 하고,
					// 해당 인벤토리의 자리를 없는 것으로 처리한다.

					m_pPlayer->SetHandleItem(m_pItemArr[m_iCollIdx]);
					SetHandleItem(m_iCollIdx, 255, 242, 0);
					CInventoryScript::g_bSelect = true; // 선택한 시점에서 흔들지 않도록 하고 싶다.
					m_fSecond = 0.f;

					// 선택한 아이템에 대한 Select Tile의 모양을 설정한다.
					if (m_pItemArr[m_iCollIdx] != nullptr)
					{
						m_pMouse->SelectItem(m_pItemArr[m_iCollIdx]->GetItem());
						m_pMouse->SetEquipItem(m_pItemArr[m_iCollIdx]->GetUIObj());
						SetItem(m_iCollIdx, nullptr);
					}
					else
					{
						m_pMouse->SelectItem(ITEM::NONE);
						m_pMouse->SetEquipItem(nullptr);
					}
					
				}
				else
				{

					// 마우스에 장착해둔 UIItemScript를 이용하여, Item에 해당하는 Script를 얻어온다.
					CUIItemScript* pUIItem = (CUIItemScript*)m_pMouse->GetEquipItem()->FindScript((UINT)SCRIPT_TYPE::UIITEMSCRIPT);
					CItemScript* pCurItem = pUIItem->GetItemScript();

					// 마우스는 장착한 아이템이 있다.

					if (m_pItemArr[m_iCollIdx])
					{
						// 아이템이 있는 곳을 클릭한 경우에는... 
						// 마우스가 가지고 있던 아이템을 해당 인벤토리의 자리에 넣고,
						// 그곳에 있던 아이템을 Mouse에 셋팅한다. (교환)

						

						if (pCurItem == nullptr)
							assert(false);

						CItemScript* pPreItem = SetItem(m_iCollIdx, pCurItem);
						m_pMouse->SetEquipItem(pPreItem->GetUIObj());

						m_pPlayer->SetHandleItem(pPreItem);
						SetHandleItem(m_iCollIdx, 255, 242, 0);
						CInventoryScript::g_bSelect = true; // 선택한 시점에서 흔들지 않도록 하고 싶다.
						m_fSecond = 0.f;

						// 선택한 아이템에 대한 Select Tile의 모양을 설정한다.
						if (pPreItem != nullptr)
							m_pMouse->SelectItem(pPreItem->GetItem());
						else
							m_pMouse->SelectItem(ITEM::NONE);

					}
					else
					{
						// 클릭한 곳에 아이템이 없다.
						// 해당 인벤토리의 자리에 Mouse가 들고 있는 아이템을 넣고, 마우스는 아무것도 들고 있지 않도록 한다.
						SetItem(m_iCollIdx, pCurItem);
						m_pMouse->SetEquipItem(nullptr);


						m_pPlayer->SetHandleItem(m_pItemArr[m_iCollIdx]);
						SetHandleItem(m_iCollIdx, 255, 242, 0);
						CInventoryScript::g_bSelect = true; // 선택한 시점에서 흔들지 않도록 하고 싶다.
						m_fSecond = 0.f;

						// 선택한 아이템에 대한 Select Tile의 모양을 설정한다.
						if (m_pItemArr[m_iCollIdx] != nullptr)
							m_pMouse->SelectItem(m_pItemArr[m_iCollIdx]->GetItem());
						else
							m_pMouse->SelectItem(ITEM::NONE);


						

					}
				}
			}
			else
			{
				m_pPlayer->SetHandleItem(m_pItemArr[m_iCollIdx]);
				SetHandleItem(m_iCollIdx, 255, 242, 0);
				CInventoryScript::g_bSelect = true; // 선택한 시점에서 흔들지 않도록 하고 싶다.
				m_fSecond = 0.f;

				// 선택한 아이템에 대한 Select Tile의 모양을 설정한다.
				if (m_pItemArr[m_iCollIdx] != nullptr)
					m_pMouse->SelectItem(m_pItemArr[m_iCollIdx]->GetItem());
				else
					m_pMouse->SelectItem(ITEM::NONE);
			}
		}
	}
	else if (g_bOpen && KEYDOWN(KEY_TYPE::KEY_RBTN))
	{
		if (m_iCollIdx != -1)
		{
			if (m_pItemArr[m_iCollIdx] != nullptr)
			{
				// 우클릭한 자리에 아이템이 있었고, 해당 아이템이 방어구에 해당하는 경우에...
				// 이 장비를 장착한다.
				// 이미 장착된 장비가 있는 경우... 지금 선택한 장비를 장착하고, 입고 있던 장비가 인벤토리로 들어온다.

				if (m_pItemArr[m_iCollIdx]->GetType() == ITEM_TYPE::HELMET)
				{
					if (m_pEquipment->GetHelmet() == nullptr)
					{
						// 아이템을 장비탭에 넘겨준다.
						// 인벤토리에서는 빠진다.
						m_pEquipment->SetItem(HELMET_IDX, m_pItemArr[m_iCollIdx]);
						SetItem(m_iCollIdx, nullptr);
					}
					else
					{
						// 이미 장착한 장비가 있는 경우.
						// 현재 선택 장비를 장착하고, 기존의 장착 장비는 인벤토리로 보낸다. (인덱스가 동일하게...)
						m_pEquipment->GetHelmet()->GetArmor()->Enable(false);
						CItemScript* pPrevItem = SetItem(m_iCollIdx, m_pEquipment->GetHelmet());
						m_pEquipment->SetItem(HELMET_IDX, pPrevItem);
					}
				}
				else if (m_pItemArr[m_iCollIdx]->GetType() == ITEM_TYPE::ARMOR)
				{
					if (m_pEquipment->GetArmor() == nullptr)
					{
						m_pEquipment->SetItem(ARMOR_IDX, m_pItemArr[m_iCollIdx]);
						SetItem(m_iCollIdx, nullptr);
					}
					else
					{
						m_pEquipment->GetArmor()->GetArmor()->Enable(false);
						CItemScript* pPrevItem = SetItem(m_iCollIdx, m_pEquipment->GetArmor());
						m_pEquipment->SetItem(ARMOR_IDX, pPrevItem);
					}
				}
				else if (m_pItemArr[m_iCollIdx]->GetType() == ITEM_TYPE::LEGGINGS)
				{
					if (m_pEquipment->GetLeggings() == nullptr)
					{
						m_pEquipment->SetItem(LEGGINGS_IDX, m_pItemArr[m_iCollIdx]);
						SetItem(m_iCollIdx, nullptr);
					}
					else
					{
						m_pEquipment->GetLeggings()->GetArmor()->Enable(false);

						CItemScript* pPrevItem = SetItem(m_iCollIdx, m_pEquipment->GetLeggings());
						m_pEquipment->SetItem(LEGGINGS_IDX, pPrevItem);
					}
				}
				
			}
		}
	}
}

void CInventoryScript::Update()
{
	// 인벤토리 클릭에 대한 처리를 담당한다.
	InventoryClick();
}

void CInventoryScript::Awake()
{
	const vector<CScript*> vecScript = GetObj()->GetScriptVec();
	
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		if (vecScript[i]->GetScriptType() == (UINT)SCRIPT_TYPE::PANELSCRIPT)
		{
			m_pPanel = (CPanelScript*)vecScript[i]; break;
		}
	}

	if (m_pPanel == nullptr)
		assert(false);

	for (size_t i = 0; i < 50; ++i)
	{
		if (i < 10)
			m_pPanel->SetPanel(i, 1);
		else
			m_pPanel->SetPanel(i, 0);
	}

	// 아이템을 끌어모으자 !
	CScene* pScene = CSceneMgr::GetInst()->GetScene();
	CLayer* pItemLayer = pScene->FindLayer(L"Item");

	if (pItemLayer == nullptr)
		assert(false);

	const vector<CGameObject*>& vecObj = pItemLayer->GetParentObjVec();

	for (size_t i = 0; i < vecObj.size(); ++i)
	{
		CScript* pScript = vecObj[i]->FindScript((UINT)SCRIPT_TYPE::ITEMSCRIPT);

		// ItemLayer에 있는데 ItemScript를 가지고 있지 않았다면 assert
		if (pScript == nullptr)
		{
			assert(false);
			continue;
		}

		AddItem((CItemScript*)pScript);
	}


	vector<CGameObject*> vecFind;
	CSceneMgr::GetInst()->FindObjByTag(L"MouseUIObj", vecFind);
	
	if (vecFind.empty())
		assert(false);

	m_pPanel->SetTarget(vecFind[0]);


	vecFind.clear();
	CSceneMgr::GetInst()->FindObjByTag(L"Mouse", vecFind);

	if (vecFind.empty())
		assert(false);

	m_pMouse = (CMouseScript*)vecFind[0]->FindScript((UINT)SCRIPT_TYPE::MOUSESCRIPT);

	if (m_pMouse == nullptr)
		assert(false);

	// PlayerScript

	vecFind.clear();
	CSceneMgr::GetInst()->FindObjByTag(L"Player", vecFind);

	if (vecFind.empty())
		assert(false);

	m_pPlayer = (CPlayerScript*)vecFind[0]->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT);

	if (m_pPlayer == nullptr)
		assert(false);

	// Crafting Script

	vecFind.clear();
	CSceneMgr::GetInst()->FindObjByTag(L"CraftingUI", vecFind);

	if (vecFind.empty())
		assert(false);

	g_pCraft = (CCraftingScript*)vecFind[0]->FindScript((UINT)SCRIPT_TYPE::CRAFTINGSCRIPT);

	if (g_pCraft == nullptr)
		assert(false);

	// Equipment
	vecFind.clear();
	CSceneMgr::GetInst()->FindObjByTag(L"Equipment", vecFind);

	if (vecFind.empty())
		assert(false);

	m_pEquipment = (CEquipmentScript*)vecFind[0]->FindScript((UINT)SCRIPT_TYPE::EQUIPMENTSCRIPT);

	if (m_pEquipment == nullptr)
		assert(false);

	vecFind.clear();
	CSceneMgr::GetInst()->FindObjByTag(L"ArmorPanel", vecFind);

	if (vecFind.empty())
		assert(false);

	m_pArmorPanel = vecFind[0];

	// 처음 시작시 Item개수 0
	memset(CInventoryScript::g_arrMtrlCount, 0, sizeof(int) * (int)ITEM::NUM_END);

	//CCore::GetInst()->GetRes();
	/*
	
	DIRT,
	STONE,
	WOOD,
	WOOD_WALL,
	CACTUS,
	*/

}

void CInventoryScript::Start()
{
	SetOpen(false);
}

int CInventoryScript::GetEmptyIdx(bool bReverse) const
{
	if (bReverse)
	{
		for (size_t i = 49; i >= 0; --i)
		{
			if (m_pItemArr[i] == nullptr)
				return i;
		}
	}
	else
	{
		for (size_t i = 0; i < 50; ++i)
		{
			if (m_pItemArr[i] == nullptr)
				return i;
		}
	}
	

	return -1;
}

int CInventoryScript::AddedItem(ITEM eItem) const
{
	// 개수를 가지지 않은 아이템.
	if ((UINT)eItem >= (UINT)ITEM::NUM_END)
		return -1;

	// 블럭이다 :  해당 아이템을 찾아서 인덱스를 반환한다.
	for (size_t i = 0; i < 5; ++i)
	{
		for (size_t j = 0; j < 10; ++j)
		{
			size_t idx = i * 5 + j;
			
			if (m_pItemArr[idx])
			{
				if (m_pItemArr[idx]->GetItem() == eItem)
					return idx;
				
			}
		}
	}
	
	return -1;
}

void CInventoryScript::SetOpen(bool bOpen)
{
	bool bChange = (g_bOpen != bOpen);
	g_bOpen = bOpen;
	// UI의 위치를 조정하고, Open, Close에 대한 처리를 한다.

	/*
		우리가 가진 정보 :
		x와 y의 인덱스
		각 Panel의 크기 x, y

	*/

	// ArmorPanel 출력.
	if (m_pArmorPanel)
		m_pArmorPanel->SetEnable(g_bOpen);

	static Vec2 tPanelSize = m_pPanel->GetPanelSize();
	static Vec2 tPos = Vec2(GetTransform()->GetLocalPos());

	Vec2 tStartPos;
	// 위치 식은 . startPos = tPos + (tPanelSize.x * x, tPanelSize.y * y)

	if (g_bOpen)
	{
		

		for (int y = 0; y < 5; ++y)
		{
			for (int x = 0; x < 10; ++x)
			{
				int idx = y * 10 + x;
				m_pPanel->SetPanel(idx, 1); // 모두 출력.

				if (m_pItemArr[idx])
				{
					// Awake에서 그 전에 예외처리를 통해서 연결이 안 되어 있는 경우에는 터트린다. (반드시 있다)
					CGameObject* pUIObj = m_pItemArr[idx]->GetUIObj();
					pUIObj->GetMeshRender()->SetEnable(true);

					tStartPos = tPos + Vec2(tPanelSize.x * x, tPanelSize.y * -y);

					pUIObj->GetTransform()->SetLocalPos(Vec3(tStartPos.x + tPanelSize.x / 2.f, tStartPos.y - tPanelSize.y / 2.f, 10.f));
					// Pivot이 중앙이므로 Panel의 중앙 위치에 넣어주면 되겠다.
				}

			}
		}

		// Close -> Open
		// 모든 Panel 파랑색
		// 선택한 아이템이 있는 경우에는 다시 크기 줄이기.

		if (m_iSelectItem != -1)
		{
			if (m_pItemArr[m_iSelectItem] && bChange)
			{
				// 해당 자리에 아이템이 존재한 경우에는 원래 크기로 돌린다.
				CGameObject* pUIObj = m_pItemArr[m_iSelectItem]->GetUIObj();

				CScaleScript* pScale = (CScaleScript*)pUIObj->FindScript((UINT)SCRIPT_TYPE::SCALESCRIPT);

				if (pScale == nullptr)
					assert(false);


				switch (m_pItemArr[m_iSelectItem]->GetType())
				{
				case ITEM_TYPE::WALL:
				case ITEM_TYPE::BENCH:
				case ITEM_TYPE::DOOR:
				case ITEM_TYPE::BLOCK:
				case ITEM_TYPE::ARMOR:
				case ITEM_TYPE::LEGGINGS:
				case ITEM_TYPE::HELMET:

					pScale->ScaleChange(5.f / 6.f, 0.2f); // return : 5 / 6
					break;

				default:
					pScale->ScaleChange(2.f / 3.f, 0.2f); // return : 2 / 3

					break;

				}
			}
		}

	}
	else
	{
	


		// Close 상태에서 마우스에 들린 아이템이 있는 경우에 이것을 다시 Inventory에 넣는 작업을 해야 한다.
		if (m_pMouse && m_pMouse->GetEquipItem())
		{
			CUIItemScript* pUIItem = (CUIItemScript*)m_pMouse->GetEquipItem()->FindScript((UINT)SCRIPT_TYPE::UIITEMSCRIPT);
			CItemScript* pCurItem = pUIItem->GetItemScript();

			if (pCurItem == nullptr)
				assert(false);

			m_pMouse->SetEquipItem(nullptr);
			AddItem(pCurItem, true);

		}


		for (int y = 0; y < 5; ++y)
		{
			for (int x = 0; x < 10; ++x)
			{
				int idx = y * 10 + x;

				if (idx < 10)
					m_pPanel->SetPanel(idx, 1); // 상단 10개 만 출력.
				else
					m_pPanel->SetPanel(idx, 0);



				if (m_pItemArr[idx])
				{
					CGameObject* pUIObj = m_pItemArr[idx]->GetUIObj();

					if (idx >= 10)
					{
						pUIObj->GetMeshRender()->SetEnable(false);
						continue;
					}

					pUIObj->GetMeshRender()->SetEnable(true);
					tStartPos = tPos + Vec2(tPanelSize.x * x, tPanelSize.y * -y);
					// 하단의 40개에 해당하는 아이템을 Y값을 높게해서 출력을 막는다.

					pUIObj->GetTransform()->SetLocalPos(Vec3(tStartPos.x + tPanelSize.x / 2.f, tStartPos.y - tPanelSize.y / 2.f, 10.f));
				}

			}
		}

		// Open -> Close 상태라면... 해당 자리 노랑색.
		// 아이템이 있는 경우에는 다시 크기 키우기.

		if (m_iSelectItem != -1)
		{
			if (m_pItemArr[m_iSelectItem] && bChange)
			{
				// 해당 자리에 아이템이 존재한 경우에는 원래 크기로 돌린다.
				CGameObject* pUIObj = m_pItemArr[m_iSelectItem]->GetUIObj();

				CScaleScript* pScale = (CScaleScript*)pUIObj->FindScript((UINT)SCRIPT_TYPE::SCALESCRIPT);

				if (pScale == nullptr)
					assert(false);

				switch (m_pItemArr[m_iSelectItem]->GetType())
				{
				case ITEM_TYPE::WALL:
				case ITEM_TYPE::BENCH:
				case ITEM_TYPE::DOOR:
				case ITEM_TYPE::BLOCK:
				case ITEM_TYPE::ARMOR:
				case ITEM_TYPE::LEGGINGS:
				case ITEM_TYPE::HELMET:

					pScale->ScaleChange(1.2f, 0.2f); // return : 5 / 6  1.2
					break;

				default:
					pScale->ScaleChange(1.5f, 0.2f); // return : 2 / 3   1.5

					break;

				}
			}

			if(m_iSelectItem < 10)
				m_pPanel->SetPanel(m_iSelectItem, 2, 255, 242, 0);

			m_pPlayer->SetHandleItem(m_pItemArr[m_iSelectItem]);

		}




	}

	if(bChange)
	{
		g_pCraft->OpenCrafting();
		m_pEquipment->OpenEquipment();

		if(IsOpen())
			CResMgr::GetInst()->Play(L"Menu_Open", CHANNEL_TYPE::EFFECT_0, EFFECT_VOLUME2);
		else
			CResMgr::GetInst()->Play(L"Menu_Close", CHANNEL_TYPE::EFFECT_0, EFFECT_VOLUME2);

	}
}

void CInventoryScript::SetHandleItem(int idx, unsigned char r, unsigned char g, unsigned char b)
{
	if (m_iSelectItem == idx)
		return;

	if (!g_bOpen)
	{
		// close 상태에서만 스케일 작업을 진행.
		// 원작에서는 font로 처리하고 있다.

		if (m_iSelectItem == -1)
		{
			// 처음 셋팅하는 경우에는 기존의 정보가 없음에 유의한다.
			m_pPanel->SetPanel(idx, 2, r, g, b);

			// 해당 자리의 아이템이 있는 경우에는 아이템의 크기를 받아둔다.
			if (m_pItemArr[idx])
			{
				CGameObject* pUIObj = m_pItemArr[idx]->GetUIObj();

				// 또한 아이템의 크기를 일정 비율로 키운다.
				CScaleScript* pScale = (CScaleScript*)pUIObj->FindScript((UINT)SCRIPT_TYPE::SCALESCRIPT);

				if (pScale == nullptr)
					assert(false);

				switch (m_pItemArr[idx]->GetType())
				{
				case ITEM_TYPE::WALL:
				case ITEM_TYPE::BENCH:
				case ITEM_TYPE::DOOR:
				case ITEM_TYPE::BLOCK:
				case ITEM_TYPE::ARMOR:
				case ITEM_TYPE::LEGGINGS:
				case ITEM_TYPE::HELMET:

					pScale->ScaleChange(1.2f, 0.2f); // return : 5 / 6
					break;

				default:
					pScale->ScaleChange(1.5f, 0.2f); // return : 2 / 3

					break;

				}


			}
		}
		else
		{
			// 처음이 아닌 경우에는 기존의 처리를 없애야 한다.

			// 해당 자리에 대하여 색을 원래대로 돌린다.

			if(m_iSelectItem < 10)
				m_pPanel->SetPanel(m_iSelectItem, 1);

			if (m_pItemArr[m_iSelectItem])
			{
				// 해당 자리에 아이템이 존재한 경우에는 원래 크기로 돌린다.
				CGameObject* pUIObj = m_pItemArr[m_iSelectItem]->GetUIObj();

				CScaleScript* pScale = (CScaleScript*)pUIObj->FindScript((UINT)SCRIPT_TYPE::SCALESCRIPT);

				if (pScale == nullptr)
					assert(false);

				switch (m_pItemArr[m_iSelectItem]->GetType())
				{
				case ITEM_TYPE::WALL:
				case ITEM_TYPE::BENCH:
				case ITEM_TYPE::DOOR:
				case ITEM_TYPE::BLOCK:
				case ITEM_TYPE::ARMOR:
				case ITEM_TYPE::LEGGINGS:
				case ITEM_TYPE::HELMET:

					pScale->ScaleChange(5.f / 6.f, 0.2f); // return : 5 / 6  1.2
					break;

				default:
					pScale->ScaleChange(2.f / 3.f, 0.2f); // return : 2 / 3   1.5

					break;

				}
			}
			// 아이템이 없던 경우에는 아이템의 크기를 돌리는 작업이 필요없음.

			// 새로운 셋팅.
			m_pPanel->SetPanel(idx, 2, r, g, b);

			if (m_pItemArr[idx])
			{
				CGameObject* pUIObj = m_pItemArr[idx]->GetUIObj();

				CScaleScript* pScale = (CScaleScript*)pUIObj->FindScript((UINT)SCRIPT_TYPE::SCALESCRIPT);

				if (pScale == nullptr)
					assert(false);

				switch (m_pItemArr[idx]->GetType())
				{
				case ITEM_TYPE::WALL:
				case ITEM_TYPE::BENCH:
				case ITEM_TYPE::DOOR:
				case ITEM_TYPE::BLOCK:
				case ITEM_TYPE::ARMOR:
				case ITEM_TYPE::LEGGINGS:
				case ITEM_TYPE::HELMET:

					pScale->ScaleChange(1.2f, 0.2f); // return : 5 / 6  1.2
					break;

				default:
					pScale->ScaleChange(1.5f, 0.2f); // return : 2 / 3   1.5

					break;

				}
			}
		}
	}
	
	m_iSelectItem = idx;

	// 어떤 아이템을 먹었는지에 따라서, Mouse에게 전달 -> 마우스의 함수에서 처리를 하는 것으로 하자.

	if(m_pItemArr[m_iSelectItem] != nullptr)
		m_pMouse->SelectItem(m_pItemArr[m_iSelectItem]->GetItem());
	else
		m_pMouse->SelectItem(ITEM::NONE);

}

void CInventoryScript::Render()
{
	// Font를 사용하여, 아이템의 개수를 출력한다.

	// 인벤토리Vec =  InventoryLT - 카메라의 LT
	static Vec2 tVec = Vec2(GetTransform()->GetLocalPos()) -
		Vec2(-CCore::GetInst()->GetRes().fWidth / 2.f, CCore::GetInst()->GetRes().fHeight / 2.f);

	static Vec2 tPanelSize = Vec2(GetTransform()->GetLocalScale()) / Vec2(10.f, 5.f);
	static wchar_t strLength[50] = {};

	// Open 상태와 상관없이 항상 Slot의 번호를 출력해야 한다.


	if (!IsOpen())
	{
		// 상단 10개.

		for (int x = 0; x < 10; ++x)
		{
			Vec2 tLT = tVec;
			tLT.x += x * tPanelSize.x;
			tLT.y *= -1.f;
			UINT iColor = FONT_RGBA(255, 255, 255, 255);
			float fSize = 17.f;

			if (m_iSelectItem == x)
			{
				fSize = 22.f;
				iColor = FONT_RGBA(0, 0, 0, 255);
			}

			wsprintf(strLength, L"%d", (x + 1) % 10);
			CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, tLT.x + 7, tLT.y + 3, fSize, iColor);
		}

		
			for (int x = 0; x < 10; ++x)
			{
				if (m_pItemArr[x] && ((UINT)m_pItemArr[x]->GetItem() < (UINT)ITEM::NUM_END))
				{
					// 아이템이 블럭인 경우에만 개수를 출력.
					wsprintf(strLength, L"%d", GetMtrlCount(m_pItemArr[x]->GetItem()));
					Vec2 tLT = tVec;
					tLT.x += x * tPanelSize.x;
					tLT.y *= -1.f;

					// 이 자리가 자신의 LT이다. (Wnd 좌표로 바꾼 것)
					CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, tLT.x + 9.f, tLT.y + 23.f, 20.f, FONT_RGBA(255, 255, 255, 255));
				}
				
			}
		
	}
	else
	{
		for (int x = 0; x < 10; ++x)
		{
			Vec2 tLT = tVec;
			tLT.x += x * tPanelSize.x;
			tLT.y *= -1.f;
			UINT iColor = FONT_RGBA(255, 255, 255, 255);
			float fSize = 17.f;

			// 선택한 아이템이 0 ~ 9번 인덱스에 있는 경우에는 색상을 다르게.
			if (m_iSelectItem == x)
			{
				fSize = 22.f;
				iColor = FONT_RGBA(25, 25, 25, 255);
			}

			wsprintf(strLength, L"%d", (x + 1) % 10);
			CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, tLT.x + 7, tLT.y + 3, fSize, iColor);
		}


		// 모두.
		for (int y = 0; y < 5; ++y)
		{
			for (int x = 0; x < 10; ++x)
			{
				int iIdx = y * 10 + x;
				if (m_pItemArr[iIdx] && ((UINT)m_pItemArr[iIdx]->GetItem() < (UINT)ITEM::NUM_END))
				{
					// 아이템이 블럭인 경우에만 개수를 출력.
					wsprintf(strLength, L"%d", GetMtrlCount(m_pItemArr[iIdx]->GetItem()));
					Vec2 tLT = tVec;
					tLT.x += x * tPanelSize.x;
					tLT.y -= y * tPanelSize.y;
					tLT.y *= -1.f;

					// 이 자리가 자신의 LT이다. (Wnd 좌표로 바꾼 것)
					CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, tLT.x + 9.f, tLT.y + 23.f, 20.f, FONT_RGBA(255, 255, 255, 255));
				}
			}
		}


	

	}

	if (m_pPanel)
	{
		m_iCollIdx = m_pPanel->GetCollIdx();

#ifdef _DEBUG
		wsprintf(strLength, L"CollIdx : %d", m_pPanel->GetCollIdx());
		CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, 450.f, 20.f, 20.f, FONT_RGBA(255, 255, 255, 255));
#endif
	}


	Vec2 tOutputPos;

	if(m_pMouse)
		tOutputPos = m_pMouse->GetOutputPos();


	// MouseObj가 무언가 가지고 있는 경우를 이곳에서 처리해준다.
	if (m_pMouse && m_pMouse->GetEquipItem() != nullptr)
	{
		// 무언가 잡고 있는 것이 있다.
		CUIItemScript* pItem = (CUIItemScript*)m_pMouse->GetEquipItem()->FindScript((UINT)SCRIPT_TYPE::UIITEMSCRIPT);

		if ((UINT)pItem->GetItem() < (UINT)ITEM::NUM_END)
		{
			// 개수로 새는 것이 가능한 경우 : BLOCK, GEL, WALL ITEM 등 이라면 마우스 장착시에 개수를 출력해준다.
			// Open 상태에서만 잡는 것이 가능하다. (닫으면 인벤토리로 들어간다.)

			lstrcpy(strLength, to_wstring(g_arrMtrlCount[(UINT)pItem->GetItem()]).c_str());
			 CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, 
				 m_pMouse->GetPointX() + 5, m_pMouse->GetPointY() + 18,
				 20.f, FONT_RGBA(255, 255, 255, 255));
		}
	}



	// MouseUIObj와 충돌이 일어난 경우 Item의  정보를 출력한다.

	// Panel이 출력이 되고 있는데, 아이템이 없을 수도 있다.


	else if (m_iCollIdx != -1)
	{
		CMouseScript::g_bUIColl = true;

		if (m_pItemArr[m_iCollIdx] != nullptr)
		{

			// 아이템의 이름을 출력한다.
			lstrcpyW(strLength, m_pItemArr[m_iCollIdx]->GetObj()->GetName().c_str());
			CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, tOutputPos.x, tOutputPos.y, 20.f, FONT_RGBA(255, 255, 255, 255));


			switch (m_pItemArr[m_iCollIdx]->GetType())
			{

			case ITEM_TYPE::WEAPON:
			{
				// Damage를 출력한다.
				int iMin = 0;
				int iMax = 0;

				switch (m_pItemArr[m_iCollIdx]->GetItem())
				{
				case ITEM::CACTUS_SWORD:
					iMin = CACTUS_SWORD_DMG_MIN;
					iMax = CACTUS_SWORD_DMG_MAX;
					break;

				case ITEM::COPPER_SWORD:
					iMin = COPPER_SWORD_DMG_MIN;
					iMax = COPPER_SWORD_DMG_MAX;
					break;
				}

				wsprintf(strLength, L"Damage : %d - %d", iMin, iMax);
				CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, tOutputPos.x, tOutputPos.y + 20.f, 20.f, FONT_RGBA(255, 255, 255, 255));

			}

			break;

			case ITEM_TYPE::ARMOR:
			case ITEM_TYPE::HELMET:
			case ITEM_TYPE::LEGGINGS:
			{
				// Defense를 출력한다.
				int iDefense = m_pItemArr[m_iCollIdx]->GetArmor()->GetArmor();
				wsprintf(strLength, L"Defense : %d", iDefense);
				CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, tOutputPos.x, tOutputPos.y + 20.f, 20.f, FONT_RGBA(255, 255, 255, 255));
			}
			break;






			case ITEM_TYPE::PICKAXE:
			case ITEM_TYPE::AXE:
			case ITEM_TYPE::HAMMER:
			{
				float fDigTime = 0.f;

				switch (m_pItemArr[m_iCollIdx]->GetItem())
				{
				case ITEM::CACTUS_PICKAXE:
					fDigTime = 0.2f;
					break;
				case ITEM::COPPER_AXE:
					fDigTime = 0.6f;

					break;
				case ITEM::COPPER_PICKAXE:
					fDigTime = 0.6f;

					break;
				case ITEM::WOOD_HAMMER:
					fDigTime = 0.3f;

					break;
				}


				swprintf_s(strLength, L"Time : %.1f", fDigTime);
				CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, tOutputPos.x, tOutputPos.y + 20.f, 20.f, FONT_RGBA(255, 255, 255, 255));
			}
				break;

			case ITEM_TYPE::TORCH:
				// Lighting

				lstrcpyW(strLength, L"Light");
				CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, tOutputPos.x, tOutputPos.y + 20.f, 20.f, FONT_RGBA(255, 255, 255, 255));
				break;

			case ITEM_TYPE::OTHER: // gel
				lstrcpyW(strLength, L"Material");
				CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, tOutputPos.x, tOutputPos.y + 20.f, 20.f, FONT_RGBA(255, 255, 255, 255));
				break;

			case ITEM_TYPE::BLOCK:
				lstrcpyW(strLength, L"Material");
				CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, tOutputPos.x, tOutputPos.y + 20.f, 20.f, FONT_RGBA(255, 255, 255, 255));

				lstrcpyW(strLength, L"Can be placed");
				CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, tOutputPos.x, tOutputPos.y + 40.f, 20.f, FONT_RGBA(255, 255, 255, 255));
				break;

			case ITEM_TYPE::WALL:
			case ITEM_TYPE::DOOR:

				lstrcpyW(strLength, L"Can be placed");
				CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, tOutputPos.x, tOutputPos.y + 20.f, 20.f, FONT_RGBA(255, 255, 255, 255));
				break;

			case ITEM_TYPE::BENCH:

				lstrcpyW(strLength, L"Can be placed");
				CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, tOutputPos.x, tOutputPos.y + 20.f, 20.f, FONT_RGBA(255, 255, 255, 255));

				lstrcpyW(strLength, L"Used for basic crafting");
				CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, tOutputPos.x, tOutputPos.y + 40.f, 20.f, FONT_RGBA(255, 255, 255, 255));
				break;

			}

		}
	}
	else if (!CMouseScript::g_bUIColl)
	{
		if (m_pMouse->GetCollider2D()->GetCollisionCount()) // Obj 충돌 && !UI충돌 
		{
			// 일단은 마우스와 충돌하고 있는 것은 타일 제외하고, Objects

			lstrcpyW(strLength, m_pMouse->GetCollisionStr().c_str());
			CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, tOutputPos.x, tOutputPos.y, 20.f, FONT_RGBA(255, 255, 255, 255));
		}
	}



	// Inventory 창 위에 글씨

	if (IsOpen())
	{
		// 열려있는 상태라면 Inventory
		lstrcpyW(strLength, L"Inventory");
		CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, 25.f, 0.f, 24.f, FONT_RGBA(255, 255, 255, 255));
	}
	else
	{
		// Close 상태라면 Handle이 있는 경우로 나뉜다.
		if (m_pPlayer && m_pPlayer->GetHandleItem())
		{
			Vec3 vScale = GetTransform()->GetLocalScale();

			// Close && HandleItem -> Inventory의 중심 상단에다가 Item의 이름을 출력한다.
			lstrcpyW(strLength, m_pPlayer->GetHandleItem()->GetObj()->GetName().c_str());
			CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, vScale.x / 2.f - 50.f, 0.f, 24.f, FONT_RGBA(255, 255, 255, 255));

		}
		else
		{
			// none이면 Inventory
			lstrcpyW(strLength, L"Inventory");
			CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, 25.f, 0.f, 24.f, FONT_RGBA(255, 255, 255, 255));
		}
	}

	
}

CItemScript* CInventoryScript::SetItem(size_t idx, CItemScript * pItem)
{
	CItemScript* pPrevItem = m_pItemArr[idx];
	m_pItemArr[idx] = pItem;


	if (pItem != nullptr)
	{
		// 아이템의 자리를 정렬해줄 필요가 있다.
		// idx = y * 10 + x;
		int iY = idx / 10;
		int iX = idx % 10;

		static Vec2 tPanelSize = m_pPanel->GetPanelSize();
		static Vec2 tPos = Vec2(GetTransform()->GetLocalPos());

		Vec2 tStartPos = tPos + Vec2(tPanelSize.x * iX, tPanelSize.y * -iY);
		CGameObject* pUIObj = pItem->GetUIObj();

		
		if(pUIObj != nullptr)
			pUIObj->GetTransform()->SetLocalPos(Vec3(tStartPos.x + tPanelSize.x / 2.f, tStartPos.y - tPanelSize.y / 2.f, 10.f));
	}
	

	return pPrevItem;

	// 해당 인벤토리에 아이템이 비어있는 경우에는 아이템을 넣어준다.
	// 아이템이 있는 경우에는 아이템을 셋팅하고 이전에 있던 것을 반환한다.
}

bool CInventoryScript::AddItem(CItemScript * pItem, bool bReverse)
{
	// 추가하려는 아이템의 Type을 얻어온다.
	ITEM_TYPE	eType = pItem->GetType();
	ITEM		eItem = pItem->GetItem();

	if (eType == ITEM_TYPE::BLOCK) // Material
	{
		// block인 경우에는 기존에 아이템이 추가가 되어있는 경우와 그렇지 않은 경우로 나눈다.

		int idx = AddedItem(eItem);

		if (idx != -1)
		{
			// 추가가 되어있다면, 아이템을 넣지 않고 수량을 증가시킨다. 수량 증가는 UI의 몫이다.
			CUIItemScript* pScript = m_pItemArr[idx]->GetUIItem();

			if (pScript == nullptr)
				assert(false);

			AddMtrlCount(eItem);

			/*
			Block에 대해서 기존의 Object를 활용할 수 있는 상황이다. (현재 연결한 Item - UIItem을 정리한다)
			*/

			DeleteObject(pItem->GetObj()); 
			DeleteObject(pItem->GetUIObj());

			pItem->GetObj()->SetEnable(false);
			pItem->GetUIObj()->SetEnable(false);

			assert(false);
			// 포인터를 통해서 동적할당을 한 이후에 필요가 없어서 다시 지운 경우이다.
			// 프레임 드랍이 예상되는 부분이기 때문에, 사용하지 못하도록 한다.
		}
		else
		{
			// 추가가 되어있지 않으면 그냥 넣어준다.
			idx = GetEmptyIdx(bReverse);

			if (idx == -1) // 빈 인덱스가 없는 경우.
				return false;

			SetItem(idx, pItem);
		}
		
		return true;
	}
	
	// 블럭이 아닌 경우라면, 칸을 하나씩 사용하는 아이템이다.
	// 인벤토리에 해당 아이템이 있는지의 여부와는 상관없이 인벤토리에 넣어준다.

	int idx = GetEmptyIdx(bReverse);

	if (idx == -1)
		return false;

	SetItem(idx, pItem);
	SetOpen(IsOpen());


	return true;
}

bool CInventoryScript::AddItem(ITEM eItem, int iCount, bool bReverse)
{
	// 인덱스를 통해서 아이템의 추가된 위치를 받아낸다. 
	// 여기서 아이템이 장비에 해당하면 항상 -1이 나오며, 아닌 경우라면 (block) 인덱스 또는 -1이 나온다.

	int iIdx = AddedItem(eItem);

	if (iIdx != -1)
	{
		// 인덱스가 나온 경우.
		// 블럭이면서 해당 인덱스가 나온 경우. (이미 들어있음)

		AddMtrlCount(eItem, iCount);
	}
	else
	{
		// 개수를 카운팅해야하는 오브젝트.
		if((UINT)eItem < (UINT)ITEM::NUM_END)
			AddMtrlCount(eItem, iCount);

		// -1이 나왔다.
		// 장비거나, 블럭인데 처음 추가 되는 경우.

		wstring strKey = ITEM_NAME[(UINT)eItem];
		CGameObject* pItemObj = CPrototypeMgr::GetInst()->CreateCloneObj(strKey, L"Item");
		CGameObject* pUIObj = CPrototypeMgr::GetInst()->CreateCloneObj(strKey + L"UI", L"UIItem");

		if (pUIObj == nullptr || pItemObj == nullptr)
			assert(false);

		CItemScript* pItem = (CItemScript*)pItemObj->FindScript((UINT)SCRIPT_TYPE::ITEMSCRIPT);
		CUIItemScript* pUIItem = (CUIItemScript*)pUIObj->FindScript((UINT)SCRIPT_TYPE::UIITEMSCRIPT);

		if (pUIItem == nullptr || pItem == nullptr)
			assert(false);

		pUIItem->SetItem(pItem);
		pItem->SetUIObj(pUIObj);

		size_t iEmptyIdx = GetEmptyIdx(bReverse);

		if (iEmptyIdx == -1)
			return false;

		SetItem(iEmptyIdx, pItem);
	}

	SetOpen(IsOpen());

	return true;
}

void CInventoryScript::ClearItem(CItemScript * pItem)
{
	// 삭제하려고 하는 아이템이 마우스에 들린 아이템인 경우에는 Inventory에 존재하지 않는다.
	if (m_pMouse->GetEquipItem() == pItem->GetUIObj())
	{
		// 마우스가 들고 있는 아이템을 nullptr로 처리하고, 해당 아이템을 날리는 처리를 한다.
		DeleteObject(pItem->GetObj());
		DeleteObject(pItem->GetUIObj());

		pItem->GetObj()->SetDisable(true);
		pItem->GetUIObj()->SetDisable(true);

		m_pMouse->SetEquipItem(nullptr);

		// 마우스에서 들고 있다가 사라진 것이다 (사용한 것) -> HandleItem이였다.
		m_pPlayer->SetHandleItem(nullptr);

		return;
	}


	// 이 아이템을 없애주세요.
	bool bDelete = false;

	for (int i = 0; i < 50; ++i)
	{
		if (m_pItemArr[i] != nullptr && m_pItemArr[i] == pItem)
		{
			// 아이템을 없애는 처리를 한다.
			ClearItem(i);

			bDelete = true;
			break;
		}
	}

	if (bDelete == false)
		assert(false);
}

void CInventoryScript::ClearItem(int iIdx)
{
	if (m_pItemArr[iIdx] == nullptr)
		assert(false);

	DeleteObject(m_pItemArr[iIdx]->GetObj());
	DeleteObject(m_pItemArr[iIdx]->GetUIObj());

	m_pItemArr[iIdx]->GetObj()->SetDisable(true);
	m_pItemArr[iIdx]->GetUIObj()->SetDisable(true);

	if (m_pItemArr[iIdx] == m_pPlayer->GetHandleItem())
		m_pPlayer->SetHandleItem(nullptr);

	

	m_pItemArr[iIdx] = nullptr;
}
