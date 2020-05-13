#include "stdafx.h"
#include "PoolMgr.h"
#include "PrototypeMgr.h"

#include <Engine/GameObject.h>
#include "ItemObjScript.h"
#include "ParticleScript.h"
#include "MonsterHPScript.h"
#include "MonsterScript.h"

CPoolMgr::CPoolMgr()
{
}


CPoolMgr::~CPoolMgr()
{
}

void CPoolMgr::OrderParticle(int iCount, vector<CParticleScript*>& vecParticle)
{
	if (m_ParticleList.size() < iCount)
	{
		// 현재 일 할 수 있는 개수보다 원하는 개수가 더 많은 경우.
		// 미리 생성하는 개수를 잘못잡았다고 판단하고 터트린다.
		MessageBox(nullptr, L"Particle Count is too low", nullptr, MB_OK);
		assert(false);
	}


	vecParticle.reserve((size_t)iCount);

	auto iterList = m_ParticleList.begin();

	for (; iCount > 0; --iCount)
	{
		vecParticle.push_back((*iterList));
		(*iterList)->GetObj()->SetEnable(true);
		++iterList;
	}

	// list에서의 삭제 과정.
	m_ParticleList.erase(m_ParticleList.begin(), iterList);
}

void CPoolMgr::OrderObj(LIST_ITEM eItem, int iCount, vector<CItemObjScript*>& vecObj)
{
	if (m_ObjList[(UINT)eItem].size() < iCount)
	{
		MessageBox(nullptr, L"Obj Count is too low", nullptr, MB_OK);
		assert(false);
	}
	else if (eItem == LIST_ITEM::END)
		assert(false);

	vecObj.reserve((size_t)iCount);

	auto iterList = m_ObjList[(UINT)eItem].begin();

	for (; iCount > 0; --iCount)
	{
		vecObj.push_back((*iterList));
		(*iterList)->GetObj()->SetEnable(true);
		++iterList;
	}

	m_ObjList[(UINT)eItem].erase(m_ObjList[(UINT)eItem].begin(), iterList);
}

CItemObjScript * CPoolMgr::OrderObj(LIST_ITEM eItem)
{
	if (m_ObjList[(UINT)eItem].empty())
	{
		MessageBox(nullptr, L"Obj Count is too low", nullptr, MB_OK);
		assert(false);
	}
	else if (eItem == LIST_ITEM::END)
		assert(false);

	auto iterList = m_ObjList[(UINT)eItem].end();
	--iterList;
	CItemObjScript* pItem = (*iterList);
	pItem->GetObj()->SetEnable(true);

	m_ObjList[(UINT)eItem].erase(iterList);

	return pItem;
}

void CPoolMgr::ReturnParticle(CParticleScript * pParticle)
{
	if (pParticle == nullptr)
		assert(false);

	pParticle->GetObj()->SetEnable(false);
	m_ParticleList.push_front(pParticle);
}

void CPoolMgr::ReturnParticle(const vector<CParticleScript*>& vecParticle)
{
	for (size_t i = 0; i < vecParticle.size(); ++i)
	{
		if (vecParticle[i] == nullptr)
			assert(false);

		vecParticle[i]->GetObj()->SetEnable(false);
		m_ParticleList.push_front(vecParticle[i]);
	}

}

void CPoolMgr::ReturnObj(LIST_ITEM eItem, CItemObjScript * pObj)
{
	if (pObj == nullptr || eItem == LIST_ITEM::END)
		assert(false);

	pObj->GetObj()->SetEnable(false);

	if (m_ObjList[(UINT)eItem].size() < m_ObjCount[(UINT)eItem])
	{
		m_ObjList[(UINT)eItem].push_front(pObj);
	}
	else
	{
		CScript::DeleteObject(pObj->GetObj());
	}
	
}

void CPoolMgr::ReturnObj(LIST_ITEM eItem, const vector<CItemObjScript*>& vecObj)
{
	if (eItem == LIST_ITEM::END)
		assert(false);

	for (size_t i = 0; i < vecObj.size(); ++i)
	{
		if (vecObj[i] == nullptr)
			assert(false);

		vecObj[i]->GetObj()->SetEnable(false);

		if (m_ObjList[(UINT)eItem].size() < m_ObjCount[(UINT)eItem])
		{
			m_ObjList[(UINT)eItem].push_front(vecObj[i]);
		}
		else
		{
			CScript::DeleteObject(vecObj[i]->GetObj());
		}
	}
}

void CPoolMgr::OrderMonsterUI(CMonsterScript * pMonster)
{
	if (pMonster == nullptr)
		assert(false);

	for (int i = 0; i < 5; ++i)
	{
		if (m_arrMonsterUI[i]->GetMonster() == nullptr)
		{
			m_arrMonsterUI[i]->GetObj()->SetEnable(true);
			m_arrMonsterUI[i]->SetMonster(pMonster);
			m_arrMonsterUI[i]->SetData(pMonster->GetHP(), pMonster->GetHPMax());
			pMonster->SetMonsterUI(m_arrMonsterUI[i]);
			
			break;
		}
	}
}

void CPoolMgr::ReturnMonsterUI(CMonsterScript * pMonster)
{
	if (pMonster == nullptr)
		assert(false);

	for (int i = 0; i < 5; ++i)
	{
		if (m_arrMonsterUI[i]->GetMonster() == pMonster)
		{
			m_arrMonsterUI[i]->GetObj()->SetEnable(false);
			m_arrMonsterUI[i]->SetMonster(nullptr);
			pMonster->SetMonsterUI(nullptr);
			break;
		}
	}
}

bool CPoolMgr::Init(int iParticleNum, int arrCount[(UINT)LIST_ITEM::END])
{
	// 미리 Layer에 배치된 상태지만, Enable = false인 상태이다.
	// 다시 반환이 되는 순간 Enabel = false로 만들고.
	// 나가는 순간 True로 만든다.

	memcpy(m_ObjCount, arrCount, sizeof(int) * (int)LIST_ITEM::END);
	Vec3 vPos = Vec3(0.f, 0.f, 90.f);

	m_ParticleList.clear();
	//ParticleObject를 생성한다.
	for (int i = 0; i < iParticleNum; ++i)
	{
		CGameObject* pParticleObj = CPrototypeMgr::GetInst()->CreateCloneObj(L"ParticleObj", L"Particle");
		pParticleObj->SetEnable(false);
		pParticleObj->GetTransform()->SetLocalPos(vPos);

		CParticleScript* pParticle = (CParticleScript*)pParticleObj->FindScript((UINT)SCRIPT_TYPE::PARTICLESCRIPT);

		if (pParticle == nullptr)
			assert(false);

		m_ParticleList.push_back(pParticle);
	}

	for (UINT iList = 0; iList < (UINT)LIST_ITEM::END; ++iList)
	{
		m_ObjList[iList].clear();
		wstring strObj;

		switch ((LIST_ITEM)iList)
		{
		case LIST_ITEM::CACTUS: strObj = L"CactusObj"; break;
		case LIST_ITEM::GEL: strObj = L"GelObj"; break;
		case LIST_ITEM::WOOD: strObj = L"WoodObj"; break;
		}

		for (int i = 0; i < m_ObjCount[iList]; ++i)
		{
			CGameObject* pObj = CPrototypeMgr::GetInst()->CreateCloneObj(strObj, L"ItemObj");
			pObj->SetEnable(false);
			pObj->GetTransform()->SetLocalPos(vPos);

			CItemObjScript* pItemObj = (CItemObjScript*)pObj->FindScript((UINT)SCRIPT_TYPE::ITEMOBJSCRIPT);
			m_ObjList[iList].push_back(pItemObj);
		}
	}


	for (int i = 0; i < 5; ++i)
	{
		// Monster UI
		CGameObject* pObj = CPrototypeMgr::GetInst()->CreateCloneObj(L"MonsterUIObj", L"MonsterUI");
		pObj->SetEnable(false);

		CMonsterHPScript* pScript = (CMonsterHPScript*)pObj->FindScript((UINT)SCRIPT_TYPE::MONSTERHPSCRIPT);

		m_arrMonsterUI[i] = pScript;
	}

	

	
	return true;
}

void CPoolMgr::SetEnable(bool bEnable)
{
	auto iter = m_ParticleList.begin();
	auto iterEnd = m_ParticleList.end();

	for (; iter != iterEnd; ++iter)
		(*iter)->GetObj()->SetEnable(bEnable);

	for (UINT iList = 0; iList < (UINT)LIST_ITEM::END; ++iList)
	{
		for (int i = 0; i < m_ObjCount[iList]; ++i)
		{
			auto iterList = m_ObjList[iList].begin();
			auto iterListEnd = m_ObjList[iList].end();

			for (; iterList != iterListEnd; ++iterList)
				(*iterList)->GetObj()->SetEnable(bEnable);
		}
	}
}

LIST_ITEM GetListItem(ITEM eItem)
{
	switch (eItem)
	{
	case ITEM::CACTUS:
		return LIST_ITEM::CACTUS;

	case ITEM::WOOD:
		return LIST_ITEM::WOOD;

	case ITEM::GEL:
		return LIST_ITEM::GEL;
	}

	return LIST_ITEM::END;
}
