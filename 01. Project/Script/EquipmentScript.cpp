#include "stdafx.h"
#include "EquipmentScript.h"
#include "PlayerScript.h"
#include "PanelScript.h"
#include "MouseScript.h"
#include "ItemScript.h"
#include "UIItemScript.h"
#include "ArmorScript.h"
#include "InventoryScript.h"


CEquipmentScript::CEquipmentScript() :
	CScript((UINT)SCRIPT_TYPE::EQUIPMENTSCRIPT),
	m_pPanel(nullptr),
	m_pPlayer(nullptr),
	m_pMouse(nullptr),
	m_pItemArr{},
	m_iCollIdx(-1)
{
}


CEquipmentScript::~CEquipmentScript()
{
}

void CEquipmentScript::EquipmentClick()
{
	if (KEYDOWN(KEY_TYPE::KEY_LBTN))
	{
		if (m_iCollIdx != -1)
		{
			m_pPlayer->SetHandleItem(m_pItemArr[m_iCollIdx]);
			m_pMouse->SelectItem(ITEM::NONE);

			// 마우스가 장착한 아이템이 없다.
			if (m_pMouse->GetEquipItem() == nullptr)
			{
				if (m_pItemArr[m_iCollIdx])
				{
					// 클릭한 곳이 아이템이 존재한다면, 이 아이템을 Mouse가 들도록 하고,
					// 해당 인벤토리의 자리를 없는 것으로 처리한다.

					m_pMouse->SetEquipItem(m_pItemArr[m_iCollIdx]->GetUIObj());

					// 마우스가 가져가기 전에, 해제.
					if (m_pItemArr[m_iCollIdx]->GetArmor() != nullptr)
					{
						m_pItemArr[m_iCollIdx]->GetArmor()->Enable(false);
					}

					SetItem(m_iCollIdx, nullptr);
				}
				//	else 처리하지 않는다. 

			}
			else
			{

				// 마우스에 장착해둔 UIItemScript를 이용하여, Item에 해당하는 Script를 얻어온다.
				CUIItemScript* pUIItem = (CUIItemScript*)m_pMouse->GetEquipItem()->FindScript((UINT)SCRIPT_TYPE::UIITEMSCRIPT);
				CItemScript* pCurItem = pUIItem->GetItemScript();

				// 마우스는 장착한 아이템이 있다.
				
				bool bProcess = false;

				if (m_iCollIdx == 10 && pUIItem->GetType() == ITEM_TYPE::HELMET)
				{
					// Helmet
					bProcess = true;
				}
				else if (m_iCollIdx == 20 && pUIItem->GetType() == ITEM_TYPE::ARMOR)
				{
					// Armor
					bProcess = true;
				}
				else if (m_iCollIdx == 30 && pUIItem->GetType() == ITEM_TYPE::LEGGINGS)
				{
					// Leggings
					bProcess = true;
				}

				if (bProcess)
				{
					// 자리와 매칭이 되는 경우에만 처리를 해준다.
					if (m_pItemArr[m_iCollIdx])
					{
						// 아이템이 있는 곳을 클릭한 경우에는... 
						// 마우스가 가지고 있던 아이템을 해당 인벤토리의 자리에 넣고,
						// 그곳에 있던 아이템을 Mouse에 셋팅한다. (교환)



						if (pCurItem == nullptr)
							assert(false);

						CItemScript* pPreItem = SetItem(m_iCollIdx, pCurItem);
						m_pMouse->SetEquipItem(pPreItem->GetUIObj());

						if (pPreItem->GetArmor() != nullptr)
						{
							pPreItem->GetArmor()->Enable(false);
						}

					}
					else
					{
						// 클릭한 곳에 아이템이 없다.
						// 해당 인벤토리의 자리에 Mouse가 들고 있는 아이템을 넣고, 마우스는 아무것도 들고 있지 않도록 한다.
						SetItem(m_iCollIdx, pCurItem);
						m_pMouse->SetEquipItem(nullptr);

					}
				}
			}
		}
	}
}

void CEquipmentScript::OpenEquipment()
{
	if (CInventoryScript::g_bOpen)
	{

		for (int i = 1; i <= 3; ++i)
		{
			m_pPanel->SetPanel(i * 10, 1);

			if (m_pItemArr[i * 10] != nullptr)
				m_pItemArr[i * 10]->GetUIObj()->SetEnable(true);

		}

	}
	else
	{
		for (int i = 1; i <= 3; ++i)
		{
			m_pPanel->SetPanel(i * 10, 0);

			if (m_pItemArr[i * 10] != nullptr)
				m_pItemArr[i * 10]->GetUIObj()->SetEnable(false);

		}

	}

}

CItemScript * CEquipmentScript::SetItem(size_t idx, CItemScript * pItem)
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


		if (pUIObj != nullptr)
			pUIObj->GetTransform()->SetLocalPos(Vec3(tStartPos.x + tPanelSize.x / 2.f, tStartPos.y - tPanelSize.y / 2.f, 10.f));

		// 있다는 소리는 ... ArmorScript의 활성화를 의미.

		pItem->GetArmor()->Enable(true);

		// 장착한 아이템이 생겼다. 
		switch (idx)
		{
		case HELMET_IDX:
			m_pPlayer->SetHelmet(m_pItemArr[idx]->GetArmor());
			break;

		case ARMOR_IDX:
			m_pPlayer->SetArmor(m_pItemArr[idx]->GetArmor());

			break;

		case LEGGINGS_IDX:
			m_pPlayer->SetLeggings(m_pItemArr[idx]->GetArmor());

			break;
		}
	}
	else
	{
		// 장작한 아이템이 없어졌다. nullptr.
		switch (idx)
		{
		case HELMET_IDX:
			m_pPlayer->SetHelmet(nullptr);
			break;

		case ARMOR_IDX:
			m_pPlayer->SetArmor(nullptr);

			break;

		case LEGGINGS_IDX:
			m_pPlayer->SetLeggings(nullptr);

			break;
		}
	}


	//  여기에 들어오는 아이템은 장비아이템이다.
	

	return pPrevItem;

	// 해당 인벤토리에 아이템이 비어있는 경우에는 아이템을 넣어준다.
	// 아이템이 있는 경우에는 아이템을 셋팅하고 이전에 있던 것을 반환한다.
}

void CEquipmentScript::Awake()
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
		m_pPanel->SetPanel(i, 0);
	}


	vector<CGameObject*> vecFind;
	CSceneMgr::GetInst()->FindObjByTag(L"Player", vecFind);

	// Player
	if (vecFind.empty())
		assert(false);

	m_pPlayer = (CPlayerScript*)vecFind[0]->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT);

	if (m_pPlayer == nullptr)
		assert(false);

	// Mouse
	vecFind.clear();
	CSceneMgr::GetInst()->FindObjByTag(L"Mouse", vecFind);

	if (vecFind.empty())
		assert(false);

	m_pMouse = (CMouseScript*)vecFind[0]->FindScript((UINT)SCRIPT_TYPE::MOUSESCRIPT);

	if (m_pMouse == nullptr)
		assert(false);

	// MouseUIObj를 Panel의 Target으로
	vecFind.clear();
	CSceneMgr::GetInst()->FindObjByTag(L"MouseUIObj", vecFind);

	if (vecFind.empty())
		assert(false);

	m_pPanel->SetTarget(vecFind[0]);

}

void CEquipmentScript::Update()
{
	// Equipment Click 처리를 한다.
	EquipmentClick();
}

void CEquipmentScript::Render()
{
	static wchar_t str[30] = {};

	if (m_pPanel != nullptr)
	{
		m_iCollIdx = m_pPanel->GetCollIdx();
		Vec2 tOutputPos = m_pMouse->GetOutputPos();

#ifdef _DEBUG
		wsprintf(str, L"CollIdx : %d", m_pPanel->GetCollIdx());
		CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, str, 560.f, 20.f, 20.f, FONT_RGBA(255, 255, 255, 255));

#endif
		if (m_iCollIdx != -1)
		{
			CMouseScript::g_bUIColl = true;

			if (m_pItemArr[m_iCollIdx] != nullptr)
			{
				lstrcpyW(str, m_pItemArr[m_iCollIdx]->GetObj()->GetName().c_str());
				CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, str, tOutputPos.x, tOutputPos.y, 20.f, FONT_RGBA(255, 255, 255, 255));

				// 장비의 Defense 수치를 출력한다.
				int iDefense = m_pItemArr[m_iCollIdx]->GetArmor()->GetArmor();
				wsprintf(str, L"Defense : %d", iDefense);
				CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, str, tOutputPos.x, tOutputPos.y + 20.f, 20.f, FONT_RGBA(255, 255, 255, 255));


			}
		}

	}

}
