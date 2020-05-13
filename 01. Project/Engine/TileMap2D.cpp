#include "stdafx.h"
#include "TileMap2D.h"
#include "ConstBuffer.h"
#include "Device.h"
#include "Transform.h"
#include "MeshRender.h"	


CTileMap2D::CTileMap2D() :
	CComponent(COMPONENT_TYPE::TILEMAP2D),
	m_tTileBufferArr{},
	m_tTileInfoArr{}
{
	m_tTileSize = Vec2(16.f, 16.f);
}


CTileMap2D::~CTileMap2D()
{
}

bool CTileMap2D::SaveToScene(FILE * pFile)
{
	UINT iType = (UINT)GetComponentType();
	fwrite(&iType, 4, 1, pFile);

	fwrite(m_tTileBufferArr, sizeof(TileBuffer) * 4000, 1, pFile);

	return true;
}

bool CTileMap2D::LoadFromScene(FILE * pFile)
{
	fread(m_tTileBufferArr, sizeof(TileBuffer) * 4000, 1, pFile);

	for (size_t i = 0; i < 4000; ++i)
	{
		m_tTileBufferArr[i].fGapRatio = 0;
	}

	return true;
}

void CTileMap2D::SetIdx(int iIdx, int iTileIdxX, int iTileIdxY)
{
	if (iIdx < 0 || iIdx >= 4000)
		assert(false);

	m_tTileBufferArr[iIdx].iX = iTileIdxX;
	m_tTileBufferArr[iIdx].iY = iTileIdxY;
}

Vec2 CTileMap2D::GetTileLT(int iX, int iY)
{
	int idx = iY * 100 + iX;

	if (idx < 0 || idx >= 4000)
		assert(false);

	Vec3 tWorldPos = GetTransform()->GetLocalPos();

	tWorldPos.x += iX * m_tTileSize.x;
	tWorldPos.y -= iY * m_tTileSize.y;

	return Vec2(tWorldPos.x, tWorldPos.y);
	
}

int CTileMap2D::GetTileIdxX(float x)
{
	float xW = GetTransform()->GetPrevPos().x;
	x -= xW;

	x /= m_tTileSize.x;

	int iX = (int)x;
	
	if (iX < 0 || iX >= 100)
		return -1;

	return iX;
}

int CTileMap2D::GetTileIdxY(float y)
{
	float yW = GetTransform()->GetPrevPos().y;
	y -= yW;

	y /= -m_tTileSize.y;

	int iY = (int)y;

	if (iY < 0 || iY >= 40)
		return -1;

	return iY;
}


void CTileMap2D::SetTileDisplay(int iX, int iY)
{
	if (IsTileIdx(iX, iY) == false)
		return;

	if (GetTileInfo(iX, iY).m_bBlock == false)
		return;

	// ���� ��ġ���. ó�� ������ ������ ���� Ÿ���� ��ġ�� �Ǵ��ؼ� ��ȯ�Ѵ�.
	// �ڽ��� ��ġ�� �������� �Ͽ� �¿���� ���ڰ� ������� 3���� Ÿ���� Ȯ���ؼ� ���� ������ ���
	// �̰��� �������� �Ÿ���� ����.

	int iGap = 4;

	for (int iWallNum = 1; iWallNum <= 4; ++iWallNum)
	{
		// �簢���� ������ 4���� ��ġ�� ��´�.
		// up : (iY - iWallNum) * 100 + iX
		// donw : (iY + iWallNum) * 100 + iX
		int iLeftTop = (iY - iWallNum) * 100 + iX - iWallNum;
		int iRightTop = (iY - iWallNum) * 100 + iX + iWallNum;
		int iLeftBottom = (iY + iWallNum) * 100 + iX - iWallNum;
		int iRightBottom = (iY + iWallNum) * 100 + iX + iWallNum;

		// 4���� �������� ���ؼ� ó���Ѵ�.

		if (IsTileIdx(iLeftTop) && !GetTileInfo(iLeftTop).m_bBlock)
		{
			iGap = iWallNum - 1;
			break;
		}

		if (IsTileIdx(iRightTop) && !GetTileInfo(iRightTop).m_bBlock)
		{
			iGap = iWallNum - 1;
			break;
		}

		if (IsTileIdx(iLeftBottom) && !GetTileInfo(iLeftBottom).m_bBlock)
		{
			iGap = iWallNum - 1;
			break;
		}

		if (IsTileIdx(iRightBottom) && !GetTileInfo(iRightBottom).m_bBlock)
		{
			iGap = iWallNum - 1;
			break;
		}

		// 4���� ������ ������ ���� �ش��ϴ� �κп� ���ؼ� ó���Ѵ�.

		// ����
		bool bEnd = false;

		for (int iStart = iLeftTop + 1; iStart < iRightTop; ++iStart)
		{
			if (IsTileIdx(iStart) && !GetTileInfo(iStart).m_bBlock)
			{
				iGap = iWallNum - 1;
				bEnd = true;
				break;
			}
		}
		
		if (bEnd)
			break;

		// bottom 
		for (int iStart = iLeftBottom + 1; iStart < iRightBottom; ++iStart)
		{
			if (IsTileIdx(iStart) && !GetTileInfo(iStart).m_bBlock)
			{
				iGap = iWallNum - 1;
				bEnd = true;
				break;
			}
		}

		if (bEnd)
			break;

		// Left
		for (int iStart = iLeftTop + 100; iStart < iLeftBottom; iStart += 100)
		{
			if (IsTileIdx(iStart) && !GetTileInfo(iStart).m_bBlock)
			{
				iGap = iWallNum - 1;
				bEnd = true;
				break;
			}
		}

		if (bEnd)
			break;

		// Right
		for (int iStart = iRightTop + 100; iStart < iRightBottom; iStart += 100)
		{
			if (IsTileIdx(iStart) && !GetTileInfo(iStart).m_bBlock)
			{
				iGap = iWallNum - 1;
				bEnd = true;
				break;
			}
		}

		if (bEnd)
			break;


	}



	// �¿���� �밢�� ��ġ���� �����Ͽ� Gap�� ���ϰ� �־��ش�. 
	//GetTileBuffer(iX, iY).fGapRatio = iGap * 0.25f;


	switch (iGap)
	{
	case 0:
		GetTileBuffer(iX, iY).fGapRatio = 0.f;

		break;

	case 1:
		GetTileBuffer(iX, iY).fGapRatio = 0.15f;

		break;

	case 2:
		GetTileBuffer(iX, iY).fGapRatio = 0.3f;

		break;

	case 3:
		GetTileBuffer(iX, iY).fGapRatio = 0.4f;

		break;

	case 4:
		GetTileBuffer(iX, iY).fGapRatio = 0.5f;

		break;
	}

}

void CTileMap2D::SetTileDisplay(int iIdx)
{
	// y * 100 + x
	SetTileDisplay(iIdx % 100, iIdx / 100);
}

void CTileMap2D::Awake()
{
	
}

void CTileMap2D::UpdateData()
{
	static CConstBuffer* pTileBuffer = CDevice::GetInst()->FindConstBuffer(L"TileBuffer");

	pTileBuffer->SetData(m_tTileBufferArr, sizeof(TileBuffer) * 4000);
	pTileBuffer->UpdateData((UINT)SHADER_TYPE::ST_VERTEX | (UINT)SHADER_TYPE::ST_PIXEL);

	// g_vec4_0 : float2 (LTPos), float2 Scale
	Vec3 vPos = GetTransform()->GetLocalPos();
	Vec3 vScale = GetTransform()->GetWorldScale();

	GetMeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC4_0, &Vec4(vPos.x, vPos.y, vScale.x, vScale.y));
}
