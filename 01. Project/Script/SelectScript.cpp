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
	// �浹ü�� 1 1�̰�, Transform�� ũ�⸦ Ű��� �ȴ�.

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
	
	// �ϴ�, LT�� �������� �ϱ� ������ ����ε� �ε����� ������ ���� ���ɼ��� �ִ�.
	// �̷� ��쿡�� �ε����� ������ �ϸ� �� �Ǵϱ�, ������ �ش��ϴ��� Ȯ���ϰ� ���Ŀ� ITEM�� NONE�� �ƴ��� Ȯ������.

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


