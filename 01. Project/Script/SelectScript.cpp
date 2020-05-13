#include "stdafx.h"
#include "SelectScript.h"
#include <Engine/TileMap2D.h>

CSelectScript::CSelectScript() :
	CScript((UINT)SCRIPT_TYPE::SELECTSCRIPT),
	m_eSelect(SELECT_TYPE::ONE)
{
}


CSelectScript::~CSelectScript()
{
}

void CSelectScript::ChangeSelectType(SELECT_TYPE eType)
{
	// 충돌체는 1 1이고, Transform의 크기를 키우면 된다.

	m_eSelect = eType;

	switch (eType)
	{
	case SELECT_TYPE::NONE:
			
		GetTransform()->SetLocalScale(0.f, 0.f, 0.f);

		break;

	case SELECT_TYPE::ONE:
		GetTransform()->SetLocalScale(16.f, 16.f, 30.f);

		break;

	case SELECT_TYPE::TWO_W:

		GetTransform()->SetLocalScale(16.f * 2.f, 16.f, 30.f);
		break;

	case SELECT_TYPE::TWO_H:
		GetTransform()->SetLocalScale(16.f, 16.f * 2.f, 30.f);

		break;

	case SELECT_TYPE::FOUR:
		GetTransform()->SetLocalScale(16.f * 2.f, 16.f * 2.f, 30.f);

		break;

	case SELECT_TYPE::TWO_W_THREE_H:
		GetTransform()->SetLocalScale(16.f * 2.f, 16.f * 3.f, 30.f);

		break;
	}
}

void CSelectScript::OnCollisionTileEnter(CTileMap2D* pTileMap, int iX, int iY)
{
	
	int iIdx = iY * 100 + iX;


	switch (m_eSelect)
	{
	case SELECT_TYPE::NONE:
		break;
	case SELECT_TYPE::ONE:

		if (IsIdx(iIdx) && pTileMap->GetTileItem(iIdx) != ITEM::NONE)
		{
			GetCollider2D()->OnCollisionEnter(nullptr);
			return;
		}

		break;
	case SELECT_TYPE::TWO_W:

		if (IsIdx(iIdx) && pTileMap->GetTileItem(iIdx) != ITEM::NONE)
		{
			GetCollider2D()->OnCollisionEnter(nullptr);
			return;
		}

		iIdx = iY * 100 + (iX + 1);

		if (IsIdx(iIdx) && pTileMap->GetTileItem(iIdx) != ITEM::NONE)
		{
			GetCollider2D()->OnCollisionEnter(nullptr);
			return;
		}
		break;


	case SELECT_TYPE::TWO_H:

		if (IsIdx(iIdx) && pTileMap->GetTileItem(iIdx) != ITEM::NONE)
		{
			GetCollider2D()->OnCollisionEnter(nullptr);
			return;
		}

		iIdx = (iY + 1) * 100 + iX;

		if (IsIdx(iIdx) && pTileMap->GetTileItem(iIdx) != ITEM::NONE)
		{
			GetCollider2D()->OnCollisionEnter(nullptr);
			return;
		}

		break;
	case SELECT_TYPE::FOUR:

		if (IsIdx(iIdx) && pTileMap->GetTileItem(iIdx) != ITEM::NONE)
		{
			GetCollider2D()->OnCollisionEnter(nullptr);
			return;
		}

		iIdx = (iY) * 100 + (iX + 1);

		if (IsIdx(iIdx) && pTileMap->GetTileItem(iIdx) != ITEM::NONE)
		{
			GetCollider2D()->OnCollisionEnter(nullptr);
			return;
		}

		iIdx = (iY + 1) * 100 + (iX);

		if (IsIdx(iIdx) && pTileMap->GetTileItem(iIdx) != ITEM::NONE)
		{
			GetCollider2D()->OnCollisionEnter(nullptr);
			return;
		}

		iIdx = (iY + 1) * 100 + (iX + 1);

		if (IsIdx(iIdx) && pTileMap->GetTileItem(iIdx) != ITEM::NONE)
		{
			GetCollider2D()->OnCollisionEnter(nullptr);
			return;
		}

		break;

	case SELECT_TYPE::TWO_W_THREE_H:

		for (int y = 0; y < 3; ++y)
		{
			for (int x = 0; x < 2; ++x)
			{
				iIdx = (iY + y) * 100 + (iX + x);

				if (IsIdx(iIdx) && pTileMap->GetTileItem(iIdx) != ITEM::NONE)
				{
					GetCollider2D()->OnCollisionEnter(nullptr);
					return;
				}
			}
		}
		

		break;
	}
}

void CSelectScript::OnCollisionTileExit(CTileMap2D* pTileMap, int iX, int iY)
{
	int iIdx = iY * 100 + iX;
	
	// 일단, LT를 기준으로 하기 때문에 제대로된 인덱스가 나오지 않을 가능성이 있다.
	// 이런 경우에는 인덱스에 접근을 하면 안 되니까, 범위에 해당하는지 확인하고 이후에 ITEM이 NONE이 아닌지 확인하자.

	switch (m_eSelect)
	{
	case SELECT_TYPE::NONE:
		break;
	case SELECT_TYPE::ONE:

		if (IsIdx(iIdx) && pTileMap->GetTileItem(iIdx) != ITEM::NONE)
		{
			GetCollider2D()->OnCollisionExit(nullptr);
			return;
		}

		break;
	case SELECT_TYPE::TWO_W:

		if (IsIdx(iIdx) && pTileMap->GetTileItem(iIdx) != ITEM::NONE)
		{
			GetCollider2D()->OnCollisionExit(nullptr);
			return;
		}

		iIdx = iY * 100 + (iX + 1);

		if (IsIdx(iIdx) && pTileMap->GetTileItem(iIdx) != ITEM::NONE)
		{
			GetCollider2D()->OnCollisionExit(nullptr);
			return;
		}
		break;


	case SELECT_TYPE::TWO_H:

		if (IsIdx(iIdx) && pTileMap->GetTileItem(iIdx) != ITEM::NONE)
		{
			GetCollider2D()->OnCollisionExit(nullptr);
			return;
		}

		iIdx = (iY + 1) * 100 + iX;

		if (IsIdx(iIdx) && pTileMap->GetTileItem(iIdx) != ITEM::NONE)
		{
			GetCollider2D()->OnCollisionExit(nullptr);
			return;
		}

		break;
	case SELECT_TYPE::FOUR:

		if (IsIdx(iIdx) && pTileMap->GetTileItem(iIdx) != ITEM::NONE)
		{
			GetCollider2D()->OnCollisionExit(nullptr);
			return;
		}

		iIdx = (iY) * 100 + (iX + 1);

		if (IsIdx(iIdx) && pTileMap->GetTileItem(iIdx) != ITEM::NONE)
		{
			GetCollider2D()->OnCollisionExit(nullptr);
			return;
		}

		iIdx = (iY + 1) * 100 + (iX);

		if (IsIdx(iIdx) && pTileMap->GetTileItem(iIdx) != ITEM::NONE)
		{
			GetCollider2D()->OnCollisionExit(nullptr);
			return;
		}

		iIdx = (iY + 1) * 100 + (iX + 1);

		if (IsIdx(iIdx) && pTileMap->GetTileItem(iIdx) != ITEM::NONE)
		{
			GetCollider2D()->OnCollisionExit(nullptr);
			return;
		}

		break;

	case SELECT_TYPE::TWO_W_THREE_H:

		for (int y = 0; y < 3; ++y)
		{
			for (int x = 0; x < 2; ++x)
			{
				iIdx = (iY + y) * 100 + (iX + x);

				if (IsIdx(iIdx) && pTileMap->GetTileItem(iIdx) != ITEM::NONE)
				{
					GetCollider2D()->OnCollisionExit(nullptr);
					return;
				}
			}
		}


		break;
	}



}


