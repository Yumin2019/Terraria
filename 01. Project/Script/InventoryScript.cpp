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

	// ���ʿ� ���� �ý����� ���� �� �ִ� ��쿡 �������� ����ϴ� ����̱� ������ ��ᰡ ���� ��쿡 �������� ��µ��� ���� ���̴�.
	// ����, �������� Ŭ���� �� �ִٴ� �Ҹ��� �ش� �������� ���� ��ᰡ �ִٴ� �Ҹ��� �ȴ�.

	// ���࿡ UseCount = 5���̰�, Item = dirt�̴�.
	// �κ��丮�� Dirt�������� 1���� �ִ� ��� : �ϳ����� 5���� ����.
	//(���⼭ ������ ������ ��� assert ó���� �ϸ� �ǰ�, 0�̶�� ���ִ� ó���� ���ش�)

	InvenSlot tSlot;

	// �ش� �������� ������ �ڸ��� ItemScript* �� ������ �ֵ��� �Ѵ�.
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

	// ������ ������ �ϳ���� �����Ѵ�.

	int iCount = GetMtrlCount(eMtrl);
	iCount -= iUseCount;

	// �ʿ��� ��ŭ�� ���µ�, 0���� ũ��. ��. ��Ȳ����.
	if (iCount > 0)
	{
		// ���� ��ŭ�� Update���ش�.
		SetMtrlCount(eMtrl, iCount);
	}

	else if (iCount == 0)
	{
		// 0�� ���.
		// �� �������� ������ �ʿ��� ����� ������ �����ߴ�.
		// �ش� �����۸� ���ְ� ������.
		ClearItem(tSlot.iIdx);
		SetMtrlCount(eMtrl, iCount);
	}
	else
	{
		// ������ ��� : ���� 5���� �ִµ�, 6���� ������ �� ���̴�. ����ȭ�� ���� ���� �ʾҴٰ� �� �� �ִ�.
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
		// �ϴ�, Close ���¿� Open ���� �������, ���콺�� Ŭ���� �ϴ� ��쿡�� �������� ������ �ȴ�.
		// Close ���¿��� Mouse�� �������� �ް� �ٴϴ� ���� 2������ �����̰�, �ϴ� �������� �����Ѵ�.

		if (m_iCollIdx != -1)
		{
			
			CResMgr::GetInst()->Play(L"Grab", CHANNEL_TYPE::GRAB, EFFECT_VOLUME);

			// Open ���¿����� Item�� ���콺�� ����ٴϵ��� ó���ؾ� �Ѵ�.
			// Mouse�� Ư�� �������� ��� �ִ� ���¿��� Close ���°� �� ��쿡 �ٽ� Ǯ�������, �ڸ��� �ٲ�� �ȴ�.
			// �̰����� �� ��... ���콺�� ��� �ִ� �������� Add�ؼ� ó���� �ϴ� ������ ���δ�.
			
			/*
			1. Close ���¿��� ���콺�� �̿��ؼ� �������� Ŭ���� ��� : �ܼ��� �������� �����Ѵ�. (�̰��� ���̴�)
			2. Open ���¿��� ���콺�� �̿��ؼ� �������� Ŭ���Ѵ� : �������� �����Ѵ�.
			�ش� �������� �ڸ��� nullptr ó���� �Ѵ�. (Mouse�� �̰��� ��� �ִ´�)



			(���� ��¸� ���콺�� �������� �ϴٰ�, ó���ϴ� ����� �ƴ϶�� ������ ����... Mouse�� �������� ������
			���·� Close ���·� �����, �ش� �ڸ��� �������� ���� ���� �ƴ϶�, AddItem���� ó���� �ǰ� �־���,
			�̰��� �κ��丮�� �ڸ����� null ó���� �ϰ� ���콺�� �̰��� ��� �ִ´ٴ� ���̴�.
			*/
			if (g_bOpen)
			{
				// ���콺�� ������ �������� ����.
				if (m_pMouse->GetEquipItem() == nullptr)
				{
					// Ŭ���� ���� �������� �����Ѵٸ�, �� �������� Mouse�� �鵵�� �ϰ�,
					// �ش� �κ��丮�� �ڸ��� ���� ������ ó���Ѵ�.

					m_pPlayer->SetHandleItem(m_pItemArr[m_iCollIdx]);
					SetHandleItem(m_iCollIdx, 255, 242, 0);
					CInventoryScript::g_bSelect = true; // ������ �������� ����� �ʵ��� �ϰ� �ʹ�.
					m_fSecond = 0.f;

					// ������ �����ۿ� ���� Select Tile�� ����� �����Ѵ�.
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

					// ���콺�� �����ص� UIItemScript�� �̿��Ͽ�, Item�� �ش��ϴ� Script�� ���´�.
					CUIItemScript* pUIItem = (CUIItemScript*)m_pMouse->GetEquipItem()->FindScript((UINT)SCRIPT_TYPE::UIITEMSCRIPT);
					CItemScript* pCurItem = pUIItem->GetItemScript();

					// ���콺�� ������ �������� �ִ�.

					if (m_pItemArr[m_iCollIdx])
					{
						// �������� �ִ� ���� Ŭ���� ��쿡��... 
						// ���콺�� ������ �ִ� �������� �ش� �κ��丮�� �ڸ��� �ְ�,
						// �װ��� �ִ� �������� Mouse�� �����Ѵ�. (��ȯ)

						

						if (pCurItem == nullptr)
							assert(false);

						CItemScript* pPreItem = SetItem(m_iCollIdx, pCurItem);
						m_pMouse->SetEquipItem(pPreItem->GetUIObj());

						m_pPlayer->SetHandleItem(pPreItem);
						SetHandleItem(m_iCollIdx, 255, 242, 0);
						CInventoryScript::g_bSelect = true; // ������ �������� ����� �ʵ��� �ϰ� �ʹ�.
						m_fSecond = 0.f;

						// ������ �����ۿ� ���� Select Tile�� ����� �����Ѵ�.
						if (pPreItem != nullptr)
							m_pMouse->SelectItem(pPreItem->GetItem());
						else
							m_pMouse->SelectItem(ITEM::NONE);

					}
					else
					{
						// Ŭ���� ���� �������� ����.
						// �ش� �κ��丮�� �ڸ��� Mouse�� ��� �ִ� �������� �ְ�, ���콺�� �ƹ��͵� ��� ���� �ʵ��� �Ѵ�.
						SetItem(m_iCollIdx, pCurItem);
						m_pMouse->SetEquipItem(nullptr);


						m_pPlayer->SetHandleItem(m_pItemArr[m_iCollIdx]);
						SetHandleItem(m_iCollIdx, 255, 242, 0);
						CInventoryScript::g_bSelect = true; // ������ �������� ����� �ʵ��� �ϰ� �ʹ�.
						m_fSecond = 0.f;

						// ������ �����ۿ� ���� Select Tile�� ����� �����Ѵ�.
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
				CInventoryScript::g_bSelect = true; // ������ �������� ����� �ʵ��� �ϰ� �ʹ�.
				m_fSecond = 0.f;

				// ������ �����ۿ� ���� Select Tile�� ����� �����Ѵ�.
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
				// ��Ŭ���� �ڸ��� �������� �־���, �ش� �������� ���� �ش��ϴ� ��쿡...
				// �� ��� �����Ѵ�.
				// �̹� ������ ��� �ִ� ���... ���� ������ ��� �����ϰ�, �԰� �ִ� ��� �κ��丮�� ���´�.

				if (m_pItemArr[m_iCollIdx]->GetType() == ITEM_TYPE::HELMET)
				{
					if (m_pEquipment->GetHelmet() == nullptr)
					{
						// �������� ����ǿ� �Ѱ��ش�.
						// �κ��丮������ ������.
						m_pEquipment->SetItem(HELMET_IDX, m_pItemArr[m_iCollIdx]);
						SetItem(m_iCollIdx, nullptr);
					}
					else
					{
						// �̹� ������ ��� �ִ� ���.
						// ���� ���� ��� �����ϰ�, ������ ���� ���� �κ��丮�� ������. (�ε����� �����ϰ�...)
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
	// �κ��丮 Ŭ���� ���� ó���� ����Ѵ�.
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

	// �������� ��������� !
	CScene* pScene = CSceneMgr::GetInst()->GetScene();
	CLayer* pItemLayer = pScene->FindLayer(L"Item");

	if (pItemLayer == nullptr)
		assert(false);

	const vector<CGameObject*>& vecObj = pItemLayer->GetParentObjVec();

	for (size_t i = 0; i < vecObj.size(); ++i)
	{
		CScript* pScript = vecObj[i]->FindScript((UINT)SCRIPT_TYPE::ITEMSCRIPT);

		// ItemLayer�� �ִµ� ItemScript�� ������ ���� �ʾҴٸ� assert
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

	// ó�� ���۽� Item���� 0
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
	// ������ ������ ���� ������.
	if ((UINT)eItem >= (UINT)ITEM::NUM_END)
		return -1;

	// ���̴� :  �ش� �������� ã�Ƽ� �ε����� ��ȯ�Ѵ�.
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
	// UI�� ��ġ�� �����ϰ�, Open, Close�� ���� ó���� �Ѵ�.

	/*
		�츮�� ���� ���� :
		x�� y�� �ε���
		�� Panel�� ũ�� x, y

	*/

	// ArmorPanel ���.
	if (m_pArmorPanel)
		m_pArmorPanel->SetEnable(g_bOpen);

	static Vec2 tPanelSize = m_pPanel->GetPanelSize();
	static Vec2 tPos = Vec2(GetTransform()->GetLocalPos());

	Vec2 tStartPos;
	// ��ġ ���� . startPos = tPos + (tPanelSize.x * x, tPanelSize.y * y)

	if (g_bOpen)
	{
		

		for (int y = 0; y < 5; ++y)
		{
			for (int x = 0; x < 10; ++x)
			{
				int idx = y * 10 + x;
				m_pPanel->SetPanel(idx, 1); // ��� ���.

				if (m_pItemArr[idx])
				{
					// Awake���� �� ���� ����ó���� ���ؼ� ������ �� �Ǿ� �ִ� ��쿡�� ��Ʈ����. (�ݵ�� �ִ�)
					CGameObject* pUIObj = m_pItemArr[idx]->GetUIObj();
					pUIObj->GetMeshRender()->SetEnable(true);

					tStartPos = tPos + Vec2(tPanelSize.x * x, tPanelSize.y * -y);

					pUIObj->GetTransform()->SetLocalPos(Vec3(tStartPos.x + tPanelSize.x / 2.f, tStartPos.y - tPanelSize.y / 2.f, 10.f));
					// Pivot�� �߾��̹Ƿ� Panel�� �߾� ��ġ�� �־��ָ� �ǰڴ�.
				}

			}
		}

		// Close -> Open
		// ��� Panel �Ķ���
		// ������ �������� �ִ� ��쿡�� �ٽ� ũ�� ���̱�.

		if (m_iSelectItem != -1)
		{
			if (m_pItemArr[m_iSelectItem] && bChange)
			{
				// �ش� �ڸ��� �������� ������ ��쿡�� ���� ũ��� ������.
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
	


		// Close ���¿��� ���콺�� �鸰 �������� �ִ� ��쿡 �̰��� �ٽ� Inventory�� �ִ� �۾��� �ؾ� �Ѵ�.
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
					m_pPanel->SetPanel(idx, 1); // ��� 10�� �� ���.
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
					// �ϴ��� 40���� �ش��ϴ� �������� Y���� �����ؼ� ����� ���´�.

					pUIObj->GetTransform()->SetLocalPos(Vec3(tStartPos.x + tPanelSize.x / 2.f, tStartPos.y - tPanelSize.y / 2.f, 10.f));
				}

			}
		}

		// Open -> Close ���¶��... �ش� �ڸ� �����.
		// �������� �ִ� ��쿡�� �ٽ� ũ�� Ű���.

		if (m_iSelectItem != -1)
		{
			if (m_pItemArr[m_iSelectItem] && bChange)
			{
				// �ش� �ڸ��� �������� ������ ��쿡�� ���� ũ��� ������.
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
		// close ���¿����� ������ �۾��� ����.
		// ���ۿ����� font�� ó���ϰ� �ִ�.

		if (m_iSelectItem == -1)
		{
			// ó�� �����ϴ� ��쿡�� ������ ������ ������ �����Ѵ�.
			m_pPanel->SetPanel(idx, 2, r, g, b);

			// �ش� �ڸ��� �������� �ִ� ��쿡�� �������� ũ�⸦ �޾Ƶд�.
			if (m_pItemArr[idx])
			{
				CGameObject* pUIObj = m_pItemArr[idx]->GetUIObj();

				// ���� �������� ũ�⸦ ���� ������ Ű���.
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
			// ó���� �ƴ� ��쿡�� ������ ó���� ���־� �Ѵ�.

			// �ش� �ڸ��� ���Ͽ� ���� ������� ������.

			if(m_iSelectItem < 10)
				m_pPanel->SetPanel(m_iSelectItem, 1);

			if (m_pItemArr[m_iSelectItem])
			{
				// �ش� �ڸ��� �������� ������ ��쿡�� ���� ũ��� ������.
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
			// �������� ���� ��쿡�� �������� ũ�⸦ ������ �۾��� �ʿ����.

			// ���ο� ����.
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

	// � �������� �Ծ������� ����, Mouse���� ���� -> ���콺�� �Լ����� ó���� �ϴ� ������ ����.

	if(m_pItemArr[m_iSelectItem] != nullptr)
		m_pMouse->SelectItem(m_pItemArr[m_iSelectItem]->GetItem());
	else
		m_pMouse->SelectItem(ITEM::NONE);

}

void CInventoryScript::Render()
{
	// Font�� ����Ͽ�, �������� ������ ����Ѵ�.

	// �κ��丮Vec =  InventoryLT - ī�޶��� LT
	static Vec2 tVec = Vec2(GetTransform()->GetLocalPos()) -
		Vec2(-CCore::GetInst()->GetRes().fWidth / 2.f, CCore::GetInst()->GetRes().fHeight / 2.f);

	static Vec2 tPanelSize = Vec2(GetTransform()->GetLocalScale()) / Vec2(10.f, 5.f);
	static wchar_t strLength[50] = {};

	// Open ���¿� ������� �׻� Slot�� ��ȣ�� ����ؾ� �Ѵ�.


	if (!IsOpen())
	{
		// ��� 10��.

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
					// �������� ���� ��쿡�� ������ ���.
					wsprintf(strLength, L"%d", GetMtrlCount(m_pItemArr[x]->GetItem()));
					Vec2 tLT = tVec;
					tLT.x += x * tPanelSize.x;
					tLT.y *= -1.f;

					// �� �ڸ��� �ڽ��� LT�̴�. (Wnd ��ǥ�� �ٲ� ��)
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

			// ������ �������� 0 ~ 9�� �ε����� �ִ� ��쿡�� ������ �ٸ���.
			if (m_iSelectItem == x)
			{
				fSize = 22.f;
				iColor = FONT_RGBA(25, 25, 25, 255);
			}

			wsprintf(strLength, L"%d", (x + 1) % 10);
			CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, tLT.x + 7, tLT.y + 3, fSize, iColor);
		}


		// ���.
		for (int y = 0; y < 5; ++y)
		{
			for (int x = 0; x < 10; ++x)
			{
				int iIdx = y * 10 + x;
				if (m_pItemArr[iIdx] && ((UINT)m_pItemArr[iIdx]->GetItem() < (UINT)ITEM::NUM_END))
				{
					// �������� ���� ��쿡�� ������ ���.
					wsprintf(strLength, L"%d", GetMtrlCount(m_pItemArr[iIdx]->GetItem()));
					Vec2 tLT = tVec;
					tLT.x += x * tPanelSize.x;
					tLT.y -= y * tPanelSize.y;
					tLT.y *= -1.f;

					// �� �ڸ��� �ڽ��� LT�̴�. (Wnd ��ǥ�� �ٲ� ��)
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


	// MouseObj�� ���� ������ �ִ� ��츦 �̰����� ó�����ش�.
	if (m_pMouse && m_pMouse->GetEquipItem() != nullptr)
	{
		// ���� ��� �ִ� ���� �ִ�.
		CUIItemScript* pItem = (CUIItemScript*)m_pMouse->GetEquipItem()->FindScript((UINT)SCRIPT_TYPE::UIITEMSCRIPT);

		if ((UINT)pItem->GetItem() < (UINT)ITEM::NUM_END)
		{
			// ������ ���� ���� ������ ��� : BLOCK, GEL, WALL ITEM �� �̶�� ���콺 �����ÿ� ������ ������ش�.
			// Open ���¿����� ��� ���� �����ϴ�. (������ �κ��丮�� ����.)

			lstrcpy(strLength, to_wstring(g_arrMtrlCount[(UINT)pItem->GetItem()]).c_str());
			 CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, 
				 m_pMouse->GetPointX() + 5, m_pMouse->GetPointY() + 18,
				 20.f, FONT_RGBA(255, 255, 255, 255));
		}
	}



	// MouseUIObj�� �浹�� �Ͼ ��� Item��  ������ ����Ѵ�.

	// Panel�� ����� �ǰ� �ִµ�, �������� ���� ���� �ִ�.


	else if (m_iCollIdx != -1)
	{
		CMouseScript::g_bUIColl = true;

		if (m_pItemArr[m_iCollIdx] != nullptr)
		{

			// �������� �̸��� ����Ѵ�.
			lstrcpyW(strLength, m_pItemArr[m_iCollIdx]->GetObj()->GetName().c_str());
			CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, tOutputPos.x, tOutputPos.y, 20.f, FONT_RGBA(255, 255, 255, 255));


			switch (m_pItemArr[m_iCollIdx]->GetType())
			{

			case ITEM_TYPE::WEAPON:
			{
				// Damage�� ����Ѵ�.
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
				// Defense�� ����Ѵ�.
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
		if (m_pMouse->GetCollider2D()->GetCollisionCount()) // Obj �浹 && !UI�浹 
		{
			// �ϴ��� ���콺�� �浹�ϰ� �ִ� ���� Ÿ�� �����ϰ�, Objects

			lstrcpyW(strLength, m_pMouse->GetCollisionStr().c_str());
			CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, tOutputPos.x, tOutputPos.y, 20.f, FONT_RGBA(255, 255, 255, 255));
		}
	}



	// Inventory â ���� �۾�

	if (IsOpen())
	{
		// �����ִ� ���¶�� Inventory
		lstrcpyW(strLength, L"Inventory");
		CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, 25.f, 0.f, 24.f, FONT_RGBA(255, 255, 255, 255));
	}
	else
	{
		// Close ���¶�� Handle�� �ִ� ���� ������.
		if (m_pPlayer && m_pPlayer->GetHandleItem())
		{
			Vec3 vScale = GetTransform()->GetLocalScale();

			// Close && HandleItem -> Inventory�� �߽� ��ܿ��ٰ� Item�� �̸��� ����Ѵ�.
			lstrcpyW(strLength, m_pPlayer->GetHandleItem()->GetObj()->GetName().c_str());
			CFontMgr::GetInst()->DrawFont((UINT)FONT::ANDY, strLength, vScale.x / 2.f - 50.f, 0.f, 24.f, FONT_RGBA(255, 255, 255, 255));

		}
		else
		{
			// none�̸� Inventory
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
		// �������� �ڸ��� �������� �ʿ䰡 �ִ�.
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

	// �ش� �κ��丮�� �������� ����ִ� ��쿡�� �������� �־��ش�.
	// �������� �ִ� ��쿡�� �������� �����ϰ� ������ �ִ� ���� ��ȯ�Ѵ�.
}

bool CInventoryScript::AddItem(CItemScript * pItem, bool bReverse)
{
	// �߰��Ϸ��� �������� Type�� ���´�.
	ITEM_TYPE	eType = pItem->GetType();
	ITEM		eItem = pItem->GetItem();

	if (eType == ITEM_TYPE::BLOCK) // Material
	{
		// block�� ��쿡�� ������ �������� �߰��� �Ǿ��ִ� ���� �׷��� ���� ���� ������.

		int idx = AddedItem(eItem);

		if (idx != -1)
		{
			// �߰��� �Ǿ��ִٸ�, �������� ���� �ʰ� ������ ������Ų��. ���� ������ UI�� ���̴�.
			CUIItemScript* pScript = m_pItemArr[idx]->GetUIItem();

			if (pScript == nullptr)
				assert(false);

			AddMtrlCount(eItem);

			/*
			Block�� ���ؼ� ������ Object�� Ȱ���� �� �ִ� ��Ȳ�̴�. (���� ������ Item - UIItem�� �����Ѵ�)
			*/

			DeleteObject(pItem->GetObj()); 
			DeleteObject(pItem->GetUIObj());

			pItem->GetObj()->SetEnable(false);
			pItem->GetUIObj()->SetEnable(false);

			assert(false);
			// �����͸� ���ؼ� �����Ҵ��� �� ���Ŀ� �ʿ䰡 ��� �ٽ� ���� ����̴�.
			// ������ ����� ����Ǵ� �κ��̱� ������, ������� ���ϵ��� �Ѵ�.
		}
		else
		{
			// �߰��� �Ǿ����� ������ �׳� �־��ش�.
			idx = GetEmptyIdx(bReverse);

			if (idx == -1) // �� �ε����� ���� ���.
				return false;

			SetItem(idx, pItem);
		}
		
		return true;
	}
	
	// ���� �ƴ� �����, ĭ�� �ϳ��� ����ϴ� �������̴�.
	// �κ��丮�� �ش� �������� �ִ����� ���οʹ� ������� �κ��丮�� �־��ش�.

	int idx = GetEmptyIdx(bReverse);

	if (idx == -1)
		return false;

	SetItem(idx, pItem);
	SetOpen(IsOpen());


	return true;
}

bool CInventoryScript::AddItem(ITEM eItem, int iCount, bool bReverse)
{
	// �ε����� ���ؼ� �������� �߰��� ��ġ�� �޾Ƴ���. 
	// ���⼭ �������� ��� �ش��ϸ� �׻� -1�� ������, �ƴ� ����� (block) �ε��� �Ǵ� -1�� ���´�.

	int iIdx = AddedItem(eItem);

	if (iIdx != -1)
	{
		// �ε����� ���� ���.
		// ���̸鼭 �ش� �ε����� ���� ���. (�̹� �������)

		AddMtrlCount(eItem, iCount);
	}
	else
	{
		// ������ ī�����ؾ��ϴ� ������Ʈ.
		if((UINT)eItem < (UINT)ITEM::NUM_END)
			AddMtrlCount(eItem, iCount);

		// -1�� ���Դ�.
		// ���ų�, ���ε� ó�� �߰� �Ǵ� ���.

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
	// �����Ϸ��� �ϴ� �������� ���콺�� �鸰 �������� ��쿡�� Inventory�� �������� �ʴ´�.
	if (m_pMouse->GetEquipItem() == pItem->GetUIObj())
	{
		// ���콺�� ��� �ִ� �������� nullptr�� ó���ϰ�, �ش� �������� ������ ó���� �Ѵ�.
		DeleteObject(pItem->GetObj());
		DeleteObject(pItem->GetUIObj());

		pItem->GetObj()->SetDisable(true);
		pItem->GetUIObj()->SetDisable(true);

		m_pMouse->SetEquipItem(nullptr);

		// ���콺���� ��� �ִٰ� ����� ���̴� (����� ��) -> HandleItem�̿���.
		m_pPlayer->SetHandleItem(nullptr);

		return;
	}


	// �� �������� �����ּ���.
	bool bDelete = false;

	for (int i = 0; i < 50; ++i)
	{
		if (m_pItemArr[i] != nullptr && m_pItemArr[i] == pItem)
		{
			// �������� ���ִ� ó���� �Ѵ�.
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
