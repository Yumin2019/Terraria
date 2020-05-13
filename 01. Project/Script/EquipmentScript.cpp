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

			// ���콺�� ������ �������� ����.
			if (m_pMouse->GetEquipItem() == nullptr)
			{
				if (m_pItemArr[m_iCollIdx])
				{
					// Ŭ���� ���� �������� �����Ѵٸ�, �� �������� Mouse�� �鵵�� �ϰ�,
					// �ش� �κ��丮�� �ڸ��� ���� ������ ó���Ѵ�.

					m_pMouse->SetEquipItem(m_pItemArr[m_iCollIdx]->GetUIObj());

					// ���콺�� �������� ����, ����.
					if (m_pItemArr[m_iCollIdx]->GetArmor() != nullptr)
					{
						m_pItemArr[m_iCollIdx]->GetArmor()->Enable(false);
					}

					SetItem(m_iCollIdx, nullptr);
				}
				//	else ó������ �ʴ´�. 

			}
			else
			{

				// ���콺�� �����ص� UIItemScript�� �̿��Ͽ�, Item�� �ش��ϴ� Script�� ���´�.
				CUIItemScript* pUIItem = (CUIItemScript*)m_pMouse->GetEquipItem()->FindScript((UINT)SCRIPT_TYPE::UIITEMSCRIPT);
				CItemScript* pCurItem = pUIItem->GetItemScript();

				// ���콺�� ������ �������� �ִ�.
				
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
					// �ڸ��� ��Ī�� �Ǵ� ��쿡�� ó���� ���ش�.
					if (m_pItemArr[m_iCollIdx])
					{
						// �������� �ִ� ���� Ŭ���� ��쿡��... 
						// ���콺�� ������ �ִ� �������� �ش� �κ��丮�� �ڸ��� �ְ�,
						// �װ��� �ִ� �������� Mouse�� �����Ѵ�. (��ȯ)



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
						// Ŭ���� ���� �������� ����.
						// �ش� �κ��丮�� �ڸ��� Mouse�� ��� �ִ� �������� �ְ�, ���콺�� �ƹ��͵� ��� ���� �ʵ��� �Ѵ�.
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
		// �������� �ڸ��� �������� �ʿ䰡 �ִ�.
		// idx = y * 10 + x;
		int iY = idx / 10;
		int iX = idx % 10;

		static Vec2 tPanelSize = m_pPanel->GetPanelSize();
		static Vec2 tPos = Vec2(GetTransform()->GetLocalPos());

		Vec2 tStartPos = tPos + Vec2(tPanelSize.x * iX, tPanelSize.y * -iY);
		CGameObject* pUIObj = pItem->GetUIObj();


		if (pUIObj != nullptr)
			pUIObj->GetTransform()->SetLocalPos(Vec3(tStartPos.x + tPanelSize.x / 2.f, tStartPos.y - tPanelSize.y / 2.f, 10.f));

		// �ִٴ� �Ҹ��� ... ArmorScript�� Ȱ��ȭ�� �ǹ�.

		pItem->GetArmor()->Enable(true);

		// ������ �������� �����. 
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
		// ������ �������� ��������. nullptr.
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


	//  ���⿡ ������ �������� ���������̴�.
	

	return pPrevItem;

	// �ش� �κ��丮�� �������� ����ִ� ��쿡�� �������� �־��ش�.
	// �������� �ִ� ��쿡�� �������� �����ϰ� ������ �ִ� ���� ��ȯ�Ѵ�.
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

	// MouseUIObj�� Panel�� Target����
	vecFind.clear();
	CSceneMgr::GetInst()->FindObjByTag(L"MouseUIObj", vecFind);

	if (vecFind.empty())
		assert(false);

	m_pPanel->SetTarget(vecFind[0]);

}

void CEquipmentScript::Update()
{
	// Equipment Click ó���� �Ѵ�.
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

				// ����� Defense ��ġ�� ����Ѵ�.
				int iDefense = m_pItemArr[m_iCollIdx]->GetArmor()->GetArmor();
				wsprintf(str, L"Defense : %d", iDefense);
				CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, str, tOutputPos.x, tOutputPos.y + 20.f, 20.f, FONT_RGBA(255, 255, 255, 255));


			}
		}

	}

}
