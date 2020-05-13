#include "stdafx.h"
#include "BackGroundScript.h"

#include <Engine/ConstBuffer.h>
#include <Engine/Camera.h>
#include <Engine/Device.h>

#include "PlayerScript.h"

CBackGroundScript::CBackGroundScript() :
	m_fCameraGapY(0.f),
	CScript((UINT)SCRIPT_TYPE::BACKGROUNDSCRIPT)
{
}


CBackGroundScript::~CBackGroundScript()
{
}

bool CBackGroundScript::SaveToScene(FILE * pFile)
{
	fwrite(&m_tSpeed, sizeof(Vec2), 1, pFile);
	fwrite(&m_tResSrc, sizeof(Vec2), 1, pFile);
	fwrite(&m_tResDest, sizeof(Vec2), 1, pFile);
	fwrite(&m_tLength, sizeof(Vec2), 1, pFile);
	fwrite(&m_tSrc, sizeof(Vec2), 1, pFile);
	fwrite(&m_tDest, sizeof(Vec2), 1, pFile);
	fwrite(&m_tOutputLength, sizeof(Vec2), 1, pFile);
	fwrite(&m_tImageLength, sizeof(Vec2), 1, pFile);
	fwrite(&m_fCameraGapY, sizeof(float), 1, pFile);

	/*
	CGameObject*	m_pCamera;
	*/
	return true;
}

bool CBackGroundScript::LoadFromScene(FILE * pFile)
{
	fread(&m_tSpeed, sizeof(Vec2), 1, pFile);
	fread(&m_tResSrc, sizeof(Vec2), 1, pFile);
	fread(&m_tResDest, sizeof(Vec2), 1, pFile);
	fread(&m_tLength, sizeof(Vec2), 1, pFile);
	fread(&m_tSrc, sizeof(Vec2), 1, pFile);
	fread(&m_tDest, sizeof(Vec2), 1, pFile);
	fread(&m_tOutputLength, sizeof(Vec2), 1, pFile);
	fread(&m_tImageLength, sizeof(Vec2), 1, pFile);
	fread(&m_fCameraGapY, sizeof(float), 1, pFile);

	return true;
}

void CBackGroundScript::SetRelativePos(const Vec2 & tSrc, const Vec2 & tDest)
{
	// ����ϴ� ������ ������ �Ǿ� ���� ���� ��� assert
	if (m_tLength.x == 0.f || m_tLength.y == 0.f)
		assert(false);

	m_tSrc = m_tResSrc + tSrc;
	m_tDest = m_tResSrc + tDest;

	m_tOutputLength = m_tDest - m_tSrc;
}

void CBackGroundScript::SetRelativePos(float srcX, float srcY, float destX, float destY)
{
	if (m_tLength.x == 0.f || m_tLength.y == 0.f)
		assert(false);

	Vec2 tSrc = Vec2(srcX, srcY);
	Vec2 tDest = Vec2(destX, destY);

	m_tSrc = m_tResSrc + tSrc;
	m_tDest = m_tResSrc + tDest;

	m_tOutputLength = m_tDest - m_tSrc;
}

void CBackGroundScript::SetInfo(const Vec2 & tSpeed, const Vec2 & tResSrc, const Vec2 & tResDest,
	const Vec2 & tSrcPos, const Vec2 & tOutputLength, 
	const Vec2 & tImageLength, CGameObject * pCamera)
{
	m_tSpeed = tSpeed;
	m_tResSrc = tResSrc;
	m_tResDest = tResDest;
	m_tLength = tResDest - tResSrc;
	m_tSrc = tSrcPos;
	m_tDest = tSrcPos + tOutputLength;
	m_tOutputLength = tOutputLength;
	m_tImageLength = tImageLength;
}

void CBackGroundScript::Awake()
{
	vector<CGameObject*> vecFind;
	CSceneMgr::GetInst()->FindObjByTag(L"Player", vecFind);

	if (vecFind.empty())
		assert(false);

	m_pPlayer = (CPlayerScript*)vecFind[0]->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT);

	if (m_pPlayer == nullptr)
		assert(false);


	// ForestTree0,1,2
	// ForestMT0,1

	m_tSpeed.y = 0.f;

	if (GetObj()->GetName().substr(0, GetObj()->GetName().size() - 1) == L"ForestTree")
	{
		m_tSrc.y = 10.f;
		m_tSpeed.y = 10.f;
	}
}

void CBackGroundScript::Update()
{
	
}

void CBackGroundScript::LateUpdate()
{

	float fTime = DT;
	// Res������ ������ǥ�� �ű��.
	// Res�� ������ ��ǥ�踦 �̿��Ͽ� �־��� �� ó��. �����ϴ� ����� ��η� ó���� �ؾ� �Ѵ�.

	if (!KEYPRESS(KEY_TYPE::KEY_LBTN) && !m_pPlayer->GetTransform()->IsMoveX() && 
		!m_pPlayer->IsSwayItem())
	{
		if (KEYPRESS(KEY_TYPE::KEY_A))
		{
			m_tSrc.x -= m_tSpeed.x * fTime;
			m_tDest.x = m_tSrc.x + m_tOutputLength.x;

			if (m_tSrc.x < m_tResSrc.x)
			{
				m_tSrc.x = m_tResSrc.x;
				m_tDest.x = m_tSrc.x + m_tOutputLength.x;
			}
		}

		else if (KEYPRESS(KEY_TYPE::KEY_D))
		{

			m_tSrc.x += m_tSpeed.x * fTime;
			m_tDest.x = m_tSrc.x + m_tOutputLength.x;

			if (m_tDest.x > m_tResDest.x)
			{
				m_tDest.x = m_tResDest.x;
				m_tSrc.x = m_tDest.x - m_tOutputLength.x;
			}
		}
	}


	// �÷��̾��� ������ ���� ó���� �Ѵ�.
	if (m_pPlayer->IsJump())
	{
		if (m_pPlayer->GetTransform()->IsMoveY() == false)
		{
			if (m_pPlayer->GetTransform()->GetMove().y >= 0.f)
			{
				m_tSrc.y -= m_tSpeed.y * fTime;
				m_tDest.y = m_tSrc.y + m_tOutputLength.y;

				if (m_tSrc.y < m_tResSrc.y)
				{
					m_tSrc.y = m_tResSrc.y;
					m_tDest.y = m_tSrc.y + m_tOutputLength.y;
				}
			}
			else
			{
				m_tSrc.y += m_tSpeed.y * fTime;
				m_tDest.y = m_tSrc.y + m_tOutputLength.y;

				if (m_tDest.y > m_tResDest.y)
				{
					m_tDest.y = m_tResDest.y;
					m_tSrc.y = m_tDest.y - m_tOutputLength.y;
				}
			}
		}
		
	}

	/*switch (eVertical)
	{
	case MOVE_DIR_VERTICAL::MD_UP:

		

		break;

	case MOVE_DIR_VERTICAL::MD_DOWN:

		m_tSrc.y += m_tSpeed.y * fTime;
		m_tDest.y = m_tSrc.y + m_tOutputLength.y;

		if (m_tDest.y > m_tResDest.y)
		{
			m_tDest.y = m_tResDest.y;
			m_tSrc.y = m_tDest.y - m_tOutputLength.y;
		}

		break;
	}*/
}

void CBackGroundScript::UpdateData()
{
	static CConstBuffer* pAnimBuffer = CDevice::GetInst()->FindConstBuffer(L"BackGround");

	// UV ��ȯ ������ ��ģ��.

	/*
	Vec2 tLeftTop;
	Vec2 tLength; // 16
	int IsAni2D;
	int arrPadding[3]; // 16
	*/

	static Vec2 tLength = m_tOutputLength / m_tImageLength;
	Vec2 tLeftTop = m_tSrc / m_tImageLength;

	static BackGround data = {};
	data.tLength = tLength;
	data.tLeftTop = tLeftTop;

	pAnimBuffer->SetData(&data);
	pAnimBuffer->UpdateData((UINT)SHADER_TYPE::ST_VERTEX | (UINT)SHADER_TYPE::ST_PIXEL);
}

