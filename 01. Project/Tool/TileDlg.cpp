// TileDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "TileDlg.h"
#include "afxdialogex.h"
#include "PenMgr.h"

#include <Engine/PathMgr.h>
// CTileDlg 대화 상자


const wchar_t* TILE_NAME[(UINT)TILE::END]
{
	L"NONE",
	L"DIRT",
	L"STONE",
	L"WOOD",
	L"WOOD_WALL",
	L"CACTUS",
};

TileInfo	CTileDlg::g_tTileInfo[(UINT)TILE::END] = {};

IMPLEMENT_DYNAMIC(CTileDlg, CDialogEx)

CTileDlg::CTileDlg(CWnd* pParent /*=nullptr*/) :
	m_iIdx(0),
	m_iRow(0),
	m_iCol(0),
	m_ImageArr{},
	m_bUpdate(false),
	m_bSave(false)
{
	m_iTileNum = IMAGE_NUM_X * IMAGE_NUM_Y;
	m_pTileOption = new TILE[m_iTileNum];

	for (UINT i = 0; i < m_iTileNum; ++i)
	{
		m_pTileOption[i] = TILE::NONE;
	}

	wstring strPath = CPathMgr::GetResPath();
	strPath += L"Scene/TileOption.ot";

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strPath.c_str(), L"rb");

	if (pFile)
	{
		// 해당 정보를 순회하면서 읽는다.
		fread(m_pTileOption, 4, m_iTileNum, pFile);

		fclose(pFile);
	}
	

}

CTileDlg::~CTileDlg()
{

	if (m_bSave)
	{
		// 한번이라도 Option 모드를 셋팅한 경우 변경점이 발생했다고 생각하고 저장한다.
		// 키지 않았으면 넘어간다. 

		wstring strPath = CPathMgr::GetResPath();
		strPath += L"Scene/TileOption.ot";

		FILE* pFile = nullptr;
		_wfopen_s(&pFile, strPath.c_str(), L"wb");

		// 해당 정보를 순회하면서 저장한다.
		fwrite(m_pTileOption, 4, m_iTileNum, pFile);

		fclose(pFile);

	}

	SAFE_DELETE_ARRAY(m_pTileOption);
}

void CTileDlg::SetTileIdx(int iRow, int iCol)
{
	// 기존에 선택한 인덱스와 같은 경우에 처리하지 않도록 한다.
	if (m_iRow == iRow && m_iCol == iCol)
		return;

	m_iRow = iRow;
	m_iCol = iCol;

	CString strSet;
	strSet.Format(L"%d", m_iRow);
	m_EditY.SetWindowTextW(strSet);

	strSet.Format(L"%d", m_iCol);
	m_EditX.SetWindowTextW(strSet);

	// TILE MODE인 경우에는 선택한 인덱스를 띄워주는 처리를 하면 되고, OPTION MODE라면, 값도 셋팅해줘야 할 것이다.

	if (GetMode() == TILE_MODE::OPTION)
	{
		// 선택한 옵션을 얻어와서 해당 자리에 셋팅한다.
		int idx = m_iRow * IMAGE_NUM_X + m_iCol;
		m_pTileOption[idx] = (TILE)m_Option.GetCurSel();
	}


	DrawTexture(m_EditScrollW.GetScrollPos(), m_EditScrollH.GetScrollPos());
	DrawLine();
	DrawTile();

}

bool CTileDlg::Init()
{
	CDialogEx::OnInitDialog();

	wstring strResPath = CPathMgr::GetResPath();
	strResPath += L"Texture/Tile/";

	for (size_t i = 0; i < TILE_MAX - 1; ++i)
	{
		wstring strTile = L"Tiles_" + std::to_wstring(i) + L".png";
		m_EditCombo.AddString(strTile.c_str());

		//m_ImageArr[i].Load((strResPath + strTile).c_str());
		m_TileSize[i] = Vec2(18.f, 18.f);
	}

	// 3 

	//Tree 22 * 22 index 5
	m_TileSize[5] = Vec2(22.f, 22.f);

	//Torch 22 * 22 index 4
	m_TileSize[4] = Vec2(22.f, 22.f);

	// Chair 18 * 20 index 7
	m_TileSize[7] = Vec2(18.f, 20.f);

	// Tiles
	m_EditCombo.AddString(L"Tiles.png");
	m_ImageArr[TILE_MAX - 1].Load((strResPath + L"Tiles.png").c_str());
	m_TileSize[TILE_MAX - 1] = Vec2(18.f, 18.f);

	m_iIdx = TILE_MAX - 1;

	// Mode
	m_Mode.AddString(L"TILE");
	m_Mode.AddString(L"OPTION");
	m_Mode.SetCurSel(0);

	// Option
	for (UINT i = 0; i < (UINT)TILE::END; ++i)
	{
		m_Option.AddString(TILE_NAME[i]);
	}

	m_Option.SetCurSel(0);
	m_Option.ShowWindow(false);


	DrawTexture(0, 0);
	DrawLine();
	DrawTile();


	// Digable, block, DROP ITEM
	g_tTileInfo[(UINT)TILE::NONE] = TileInfo(false, false, ITEM::NONE);
	g_tTileInfo[(UINT)TILE::DIRT] = TileInfo(true, true, ITEM::DIRT);
	g_tTileInfo[(UINT)TILE::STONE] = TileInfo(true, true, ITEM::STONE);
	g_tTileInfo[(UINT)TILE::WOOD] = TileInfo(true, true, ITEM::WOOD);
	g_tTileInfo[(UINT)TILE::WOOD_WALL] = TileInfo(true, false, ITEM::WOOD_WALL);
	g_tTileInfo[(UINT)TILE::CACTUS] = TileInfo(true, false, ITEM::CACTUS);


	for (UINT i = 0; i < (UINT)TILE::END; ++i)
		g_tTileInfo[i].m_bNatural = true;

	return true;
}

void CTileDlg::DrawTexture(int iSrcX, int iSrcY)
{
	// EditCtrl의 DC를 얻어온다.

	CDC* pDC = m_EditPic.GetWindowDC();

	CRect rc;
	m_EditPic.GetWindowRect(rc);
	Rectangle(*pDC, 0, 0, rc.Width(), rc.Height()); // 지우는 처리.

	// 이미지의 크기에서 출력하는 영역을 빼서 여분의 공간의 크기를 구한다.
	// 근데 이 값을 src의 위치에 영향을 받아야 한다.
	int iWidthEmpty = m_ImageArr[m_iIdx].GetWidth() - rc.Width() - iSrcX;
	int iHeightEmpty = m_ImageArr[m_iIdx].GetHeight() - rc.Height() - iSrcY;

	if (iWidthEmpty < 0)
	{
		// Width가 출력 영역보다 작은 경우.
		// 해당 사이즈까지만 출력.
		iWidthEmpty = m_ImageArr[m_iIdx].GetWidth();
	}
	else
	{
		// 여분이 존재하는 경우라면, 
		// 해당 위치를 잡아서, ctrl의 사이즈 만큼 출력한다.
		iWidthEmpty = rc.Width();
	}

	if (iHeightEmpty < 0)
	{
		// Height가 출력 영역보다 작은 경우
		iHeightEmpty = m_ImageArr[m_iIdx].GetHeight();
	}
	else
		iHeightEmpty = rc.Height();

	/*tInfo.nMax = m_ImageArr[m_iIdx].GetWidth() - rc.Width();*/
	m_ImageArr[m_iIdx].Draw(*pDC, 0, 0, iWidthEmpty, iHeightEmpty, iSrcX, iSrcY, iWidthEmpty, iHeightEmpty);

	m_EditPic.ReleaseDC(pDC);

}

void CTileDlg::DrawLine()
{
	CDC* pDC = m_EditPic.GetWindowDC();

	if (GetMode() == TILE_MODE::TILE)
	{
		HPEN hPen = CPenMgr::GetInst()->GetMagentaPen();
		HPEN hPrevPen = (HPEN)SelectObject(*pDC, hPen);

		// Tile의 간격을 생각한다. 
		CRect rc = {
			GetTileGapX(m_iIdx) * m_iCol,
			GetTileGapY(m_iIdx) * m_iRow,
			GetTileGapX(m_iIdx) * m_iCol + GetTileSizeX(m_iIdx),
			GetTileGapY(m_iIdx) * m_iRow + GetTileSizeY(m_iIdx)
		};

		// scroll 적용
		rc.left -= m_EditScrollW.GetScrollPos();
		rc.right -= m_EditScrollW.GetScrollPos();
		rc.top -= m_EditScrollH.GetScrollPos();
		rc.bottom -= m_EditScrollH.GetScrollPos();

		// 선택한 타일 그리기.
		MoveToEx(*pDC, rc.left, rc.top, nullptr);
		LineTo(*pDC, rc.right, rc.top);
		LineTo(*pDC, rc.right, rc.bottom);
		LineTo(*pDC, rc.left, rc.bottom);
		LineTo(*pDC, rc.left, rc.top);

		// 원래 Pen으로.
		SelectObject(*pDC, hPrevPen);

	}
	else
	{
		// OPTION MODE : 모든 TILE의 OPTION에 따라서 RECT를 그려준다.
		// 보이는 부분만 그리는 것이 맞다.
		// Scroll에 해당하는 부분의 인덱스만 구해준다. 어디부터 어디까지 보이고 있는가 ? 2200개 정도인데, 이것을 그린다고 해서 버벅대지는 않는다.
		// 그건 더블버퍼링의 문제이기 때문이다. 느린 것이 아니다.

		// 아무튼, 그냥 다 해주는 걸로 할까...

		for(int y = 0; y < IMAGE_NUM_Y; ++y)
		{
			for (int x = 0; x < IMAGE_NUM_X; ++x)
			{
				int idx = y * IMAGE_NUM_X + x;

				if (m_pTileOption[idx] == TILE::NONE)
					continue;

				HBRUSH hBrush = CPenMgr::GetInst()->GetTileBrush((int)m_pTileOption[idx]);
				HBRUSH hPrevBrush = (HBRUSH)SelectObject(*pDC, hBrush);

				// Tile의 간격을 생각한다. 
				CRect rc = {
					GetTileGapX(m_iIdx) * x + 3,
					GetTileGapY(m_iIdx) * y + 3,
					GetTileGapX(m_iIdx) * x + GetTileSizeX(m_iIdx) - 3,
					GetTileGapY(m_iIdx) * y + GetTileSizeY(m_iIdx) - 3
				};

				// scroll 적용
				rc.left -= m_EditScrollW.GetScrollPos();
				rc.right -= m_EditScrollW.GetScrollPos();
				rc.top -= m_EditScrollH.GetScrollPos();
				rc.bottom -= m_EditScrollH.GetScrollPos();

				// 선택한 타일 그리기.
				Rectangle(*pDC, rc.left, rc.top, rc.right, rc.bottom);

				// BRUSH
				SelectObject(*pDC, hPrevBrush);
			}
		}

	}

	// 리소스 반환 
	m_EditPic.ReleaseDC(pDC);
}

void CTileDlg::DrawTile()
{
	// Tile pic ctrl의 사이즈를 받아옴.
	CRect rc;
	m_EditTile.GetClientRect(&rc);

	CDC* pDC = m_EditTile.GetWindowDC();
	Rectangle(*pDC, 0, 0, rc.right, rc.bottom);


	if (GetMode() == TILE_MODE::TILE)
	{
		m_ImageArr[m_iIdx].Draw(*pDC, 0, 0, rc.Width(), rc.Height(),
			m_iCol * GetTileGapX(m_iIdx),
			m_iRow * GetTileGapY(m_iIdx),
			GetTileSizeX(m_iIdx),
			GetTileSizeY(m_iIdx));
	}
	else
	{
		// OPTION MODE

		// 내가 고른 옵션에 해당하는 pen을 설정하여 작은 사각형을 그려주자.
		HBRUSH hBrush = CPenMgr::GetInst()->GetTileBrush((int)m_Option.GetCurSel());
		HBRUSH hPrevBrush = (HBRUSH)SelectObject(*pDC, hBrush);

		Rectangle(*pDC, rc.left, rc.top, rc.right, rc.bottom);

		// 원래 BRUSH으로.
		SelectObject(*pDC, hPrevBrush);
		
	}
	
	m_EditTile.ReleaseDC(pDC);
}

void CTileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_1, m_EditPic);
	DDX_Control(pDX, IDC_COMBO1, m_EditCombo);
	DDX_Control(pDX, IDC_SCROLLBAR3, m_EditScrollH);
	DDX_Control(pDX, IDC_SCROLLBAR2, m_EditScrollW);


	DDX_Control(pDX, IDC_EDIT1, m_EditX);
	DDX_Control(pDX, IDC_EDIT2, m_EditY);
	DDX_Control(pDX, IDC_STATIC2, m_EditTile);

	m_EditX.SetWindowTextW(L"0");
	m_EditY.SetWindowTextW(L"0");

	DDX_Control(pDX, IDC_EDIT3, m_SizeX);
	DDX_Control(pDX, IDC_EDIT4, m_SizeY);
	DDX_Control(pDX, IDC_COMBO2, m_Mode);

	DDX_Control(pDX, IDC_COMBO4, m_Option);
}


BEGIN_MESSAGE_MAP(CTileDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CTileDlg::OnBnClickedLeft)
	ON_BN_CLICKED(IDC_BUTTON2, &CTileDlg::OnBnClickedRight)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CTileDlg::OnChangedCombo)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEHWHEEL()
	ON_CBN_SELCHANGE(IDC_COMBO2, &CTileDlg::OnChangedTileMode)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CTileDlg::OnChangedTileOption)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CTileDlg 메시지 처리기




void CTileDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
					   // 윈도우 기본 코드 건들였다가, (한 줄 주석걸음)
					   // OnPaint 함수 오지게 호출당함.. ㅎㄷㄷ


	if (!m_bUpdate)
	{
		// 처음 업데이트 !

		// EditCtrl의 DC를 얻어온다.
		CDC* pDC = m_EditPic.GetWindowDC();
		CRect rc;
		m_EditPic.GetWindowRect(rc);
		Rectangle(*pDC, 0, 0, rc.Width(), rc.Height());
		m_ImageArr[m_iIdx].Draw(*pDC, 0, 0);

		// 현재 그리고 있는 것을 combobox가 출력해주는 것으로 하자.
		m_EditCombo.SetCurSel(m_iIdx);

		// Picture Ctrl의 사이즈를 받는다.

		// Height에 대한 정보를 설정한다.
		SCROLLINFO tInfo = {};
		tInfo.nPos = 0;
		tInfo.nMin = 0;
		tInfo.nMax = m_ImageArr[m_iIdx].GetHeight() - rc.Height();
		if (tInfo.nMax < 0)
			tInfo.nMax = 0;
		tInfo.cbSize = sizeof(tInfo);
		tInfo.nPage = 1; // 스크롤바 크기의 n %
		tInfo.nTrackPos = 0;
		tInfo.fMask = SIF_ALL;
		m_EditScrollH.SetScrollInfo(&tInfo);


		// Width
		tInfo = {};
		tInfo.nPos = 0;
		tInfo.nMin = 0;
		tInfo.nMax = m_ImageArr[m_iIdx].GetWidth() - rc.Width();
		if (tInfo.nMax < 0)
			tInfo.nMax = 0;
		tInfo.cbSize = sizeof(tInfo);
		tInfo.nPage = 1;
		tInfo.nTrackPos = 0;
		tInfo.fMask = SIF_ALL;
		m_EditScrollW.SetScrollInfo(&tInfo);

		// 인덱스 초기화.
		SetTileIdx(0, 0);
		m_EditPic.ReleaseDC(pDC);

		m_bUpdate = true;

		// 이미지의 크기를 출력해준다.
		m_SizeX.SetWindowTextW(std::to_wstring(m_ImageArr[m_iIdx].GetWidth()).c_str());
		m_SizeY.SetWindowTextW(std::to_wstring(m_ImageArr[m_iIdx].GetHeight()).c_str());
	}
	else
	{
		// 기존 상태 유지.
		DrawTexture(m_EditScrollW.GetScrollPos(), m_EditScrollH.GetScrollPos());
	}

	DrawLine();
	DrawTile();

}


void CTileDlg::OnBnClickedLeft()
{
	//--m_iIdx;

	//if (m_iIdx < 0)
	//	m_iIdx = TILE_MAX - 1;

	//m_bUpdate = false;
	//OnPaint();
}


void CTileDlg::OnBnClickedRight()
{
	//++m_iIdx;

	//if (m_iIdx >= TILE_MAX)
	//	m_iIdx = 0;

	//m_bUpdate = false;
	//OnPaint();

}


void CTileDlg::OnChangedCombo()
{
	if (m_EditCombo.GetCurSel() == m_iIdx)
		return;

	m_EditCombo.SetCurSel(TILE_MAX - 1); // 고정.

	// On Paint -> SetCursel -> 


	/*m_iIdx = m_EditCombo.GetCurSel();
	m_bUpdate = false;
	OnPaint();*/
}




void CTileDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// 스크롤바가 여러 개가 있다면, 모두 이 메세지를 호출하기 때문에 스크롤바의 주소를 따져주는 것인가 ?
	// 그런가봄.


	if (pScrollBar == &m_EditScrollH)
	{
		// 스크롤바 위치를 가져온다.

		SCROLLINFO tInfo = {};

		if (m_EditScrollH.GetScrollInfo(&tInfo))
		{
			switch (nSBCode)
			{
			case SB_BOTTOM: // max 설정 
			case SB_TOP: // min 설정
			case SB_ENDSCROLL: // 스크롤 종료
				// 끝까지 도달을 한 경우에 nPos를 max에 맞춰주는 처리를 한다면 스크롤을 통해서도 맞춰줘야 한다.

				// 버튼을 통해서 이동 -> max까지 정확한 이동 처리가 가능.
				// 이후 EndScroll (문제 없음)


				return;
			

			case SB_LINEDOWN:
				tInfo.nPos += 10.f;
				// 화살표 아래

				if (tInfo.nPos > tInfo.nMax)
					tInfo.nPos = tInfo.nMax;
				break;

			case SB_LINEUP:
				tInfo.nPos -= 10.f;
				// 화살표 위

				if (tInfo.nPos < 0)
					tInfo.nPos = 0;
				break;

			case SB_PAGEDOWN:
				// 버튼과 기둥의 사이의 아래를 누름.
				tInfo.nPos += 10.f;

				if (tInfo.nPos > tInfo.nMax)
					tInfo.nPos = tInfo.nMax;

				break;

			case SB_PAGEUP:
				// 버튼과 기둥의 사이에 위를 누름
				tInfo.nPos -= 10.f;

				if (tInfo.nPos < 0)
					tInfo.nPos = 0;
				break;

			case SB_THUMBPOSITION:
				// 드래그를 하여 마우스 버튼을 놓은 것. HIWORD가 위치를 나타낸다.
				tInfo.nPos = nPos;

				// scroll을 통한 이동 -> 최대한의 이동은 상단위치를 기준으로 함.
				// 따라서, nPos + Percent(10)이 max를 넘어가는 경우. max를 잡아주는 것이 맞을 것 같다.

				if (nPos + tInfo.nPage > tInfo.nMax)
				{
					tInfo.nPos = tInfo.nMax;
				}
				break;

			case SB_THUMBTRACK:
				// 드래그 상자를 끌고 있음 HIWORD : 위치
				tInfo.nPos = nPos;

				if (nPos + tInfo.nPage > tInfo.nMax)
				{
					tInfo.nPos = tInfo.nMax;
				}

				break;


				
			}

			m_EditScrollH.SetScrollInfo(&tInfo);
			// Info에 맞춰서 Picture Ctrl을 통한 출력위치를 다르게 처리해주자.
			// tInfo.nPos는 범위가 지정되어 있다.
			// H의 경우에는 해당 위치부터 출력하면 되고, W는 그대로 두면 될 것이다.
			// W와 H가 각각 가지고 있는 정보가 다른 만큼, 각 ctrl로 부터 현재위치를 가져오는 처리를 해야 한다.
			int iSrcX = m_EditScrollW.GetScrollPos();
			int iSrcY = tInfo.nPos;

			DrawTexture(iSrcX, iSrcY);
			DrawLine();
		}

			

	}
}


void CTileDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	if (pScrollBar == &m_EditScrollW)
	{
		SCROLLINFO tInfo = {};

		if (m_EditScrollW.GetScrollInfo(&tInfo))
		{
			switch (nSBCode)
			{

				/*
#define SB_LINELEFT         0
#define SB_LINERIGHT        1
#define SB_PAGELEFT         2
#define SB_THUMBPOSITION    4
#define SB_THUMBTRACK       5
#define SB_LEFT             6
#define SB_RIGHT            7
#define SB_ENDSCROLL        8
				*/
			case SB_RIGHT: // max 설정 
			case SB_LEFT: // min 설정
			case SB_ENDSCROLL: // 스크롤 종료

				return;


			case SB_LINERIGHT:
				tInfo.nPos += 10.f;

				if (tInfo.nPos > tInfo.nMax)
					tInfo.nPos = tInfo.nMax;
				break;

			case SB_LINELEFT:
				tInfo.nPos -= 10.f;

				if (tInfo.nPos < 0)
					tInfo.nPos = 0;
				break;

			case SB_PAGERIGHT:
				tInfo.nPos += 10.f;

				if (tInfo.nPos > tInfo.nMax)
					tInfo.nPos = tInfo.nMax;

				break;

			case SB_PAGELEFT:
				tInfo.nPos -= 10.f;

				if (tInfo.nPos < 0)
					tInfo.nPos = 0;
				break;

			case SB_THUMBPOSITION:
				tInfo.nPos = nPos;

				if (nPos + tInfo.nPage > tInfo.nMax)
				{
					tInfo.nPos = tInfo.nMax;
				}
				break;

			case SB_THUMBTRACK:
				tInfo.nPos = nPos;

				if (nPos + tInfo.nPage > tInfo.nMax)
				{
					tInfo.nPos = tInfo.nMax;
				}

				break;



			}

			m_EditScrollW.SetScrollInfo(&tInfo);
			int iSrcX = tInfo.nPos;
			int iSrcY = m_EditScrollH.GetScrollPos();

			DrawTexture(iSrcX, iSrcY);
			DrawLine();
		}



	}
}


void CTileDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// point는 windowPos 즉, TileDlg를 기반으로 인식을 한다. (애초에 Client영역이 아니면 처리도 안 된다)
	//CDialogEx::OnLButtonDown(nFlags, point);

	CPoint tPoint = point;
	ClientToScreen(&tPoint);
	
	CRect tRC;
	m_EditPic.GetWindowRect(tRC);

	tPoint.x -= tRC.left;
	tPoint.y -= tRC.top;

	// Scroll Ctrl을 통해서, Point의 위치를 조절한다.
	tPoint.y += m_EditScrollH.GetScrollPos();
	tPoint.x += m_EditScrollW.GetScrollPos();

	int iRow = tPoint.y / GetTileGapY(m_iIdx);
	int iCol = tPoint.x / GetTileGapX(m_iIdx);

	// 현재 텍스처의 이미지의 범위를 알아낸다.
	int iColMax = m_ImageArr[m_iIdx].GetWidth() / GetTileGapX(m_iIdx);
	int iRowMax = m_ImageArr[m_iIdx].GetHeight() / GetTileGapY(m_iIdx);

	if (iRow < 0 || iCol < 0 || iCol >= iColMax || iRow >= iRowMax)
	{
		// 비정상적인 범위.
		return;
	}
	else
	{
		SetTileIdx(iRow, iCol);

	}

}



#include "ButtonDlg.h"

void CTileDlg::OnCancel()
{
	CButtonDlg::g_bShowTile = false;
	CPopUpDlg::OnCancel();
}


void CTileDlg::OnChangedTileMode()
{
	if ((int)TILE_MODE::TILE == m_Mode.GetCurSel())
	{
		// OPTION -> TILE
		m_Option.ShowWindow(false);
	}
	else
	{
		// TILE -> OPTION
		m_Option.ShowWindow(true);
		m_bSave = true;

	}
}


void CTileDlg::OnChangedTileOption()
{
	DrawTile();
}


void CTileDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// 누른 위치를 picture ctrl을 기준으로 얻어와야 한다.

	if (GetMode() == TILE_MODE::OPTION)
	{
		CPoint tPoint = point;
		ClientToScreen(&tPoint);

		CRect tRC;
		m_EditPic.GetWindowRect(tRC);

		tPoint.x -= tRC.left;
		tPoint.y -= tRC.top;

		// Scroll Ctrl을 통해서, Point의 위치를 조절한다.
		tPoint.y += m_EditScrollH.GetScrollPos();
		tPoint.x += m_EditScrollW.GetScrollPos();

		int iRow = tPoint.y / GetTileGapY(m_iIdx);
		int iCol = tPoint.x / GetTileGapX(m_iIdx);

		// 현재 텍스처의 이미지의 범위를 알아낸다.
		int iColMax = m_ImageArr[m_iIdx].GetWidth() / GetTileGapX(m_iIdx);
		int iRowMax = m_ImageArr[m_iIdx].GetHeight() / GetTileGapY(m_iIdx);

		if (iRow < 0 || iCol < 0 || iCol >= iColMax || iRow >= iRowMax)
		{
			// 비정상적인 범위.
			return;
		}
		else
		{
			m_iStartX = iCol;
			m_iStartY = iRow;
		}

	}

	CPopUpDlg::OnRButtonDown(nFlags, point);
}


void CTileDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (GetMode() == TILE_MODE::OPTION)
	{
		CPoint tPoint = point;
		ClientToScreen(&tPoint);

		CRect tRC;
		m_EditPic.GetWindowRect(tRC);

		tPoint.x -= tRC.left;
		tPoint.y -= tRC.top;

		// Scroll Ctrl을 통해서, Point의 위치를 조절한다.
		tPoint.y += m_EditScrollH.GetScrollPos();
		tPoint.x += m_EditScrollW.GetScrollPos();

		int iRow = tPoint.y / GetTileGapY(m_iIdx);
		int iCol = tPoint.x / GetTileGapX(m_iIdx);

		// 현재 텍스처의 이미지의 범위를 알아낸다.
		int iColMax = m_ImageArr[m_iIdx].GetWidth() / GetTileGapX(m_iIdx);
		int iRowMax = m_ImageArr[m_iIdx].GetHeight() / GetTileGapY(m_iIdx);

		if (iRow < 0 || iCol < 0 || iCol >= iColMax || iRow >= iRowMax)
		{
			// 비정상적인 범위.
			return;
		}
		else
		{
			// 타일 일괄적용 ~

			DrawTexture(m_EditScrollW.GetScrollPos(), m_EditScrollH.GetScrollPos()); 
			// NONE의 경우에는 다시 그리지 않기때문에
			// 일단 Texture를 다시 그리고 각 tile에 대한 처리를 한다.
			// 예를 들면 색칠이 되어 있는 상태에서 none으로 드래그를 한 경우. DrawTexture로 인해서 색칠이 되고,
			// Option Draw에 의해서, (그려지지 않고 패스) 처리된다.


			TILE eTile = (TILE)m_Option.GetCurSel();

			for (int y = m_iStartY; y <= iRow; ++y)
			{
				for (int x = m_iStartX; x <= iCol; ++x)
				{
					int idx = y * IMAGE_NUM_X + x;
					
					m_pTileOption[idx] = eTile;
				}
			}

			DrawLine();

		}

	}
	CPopUpDlg::OnRButtonUp(nFlags, point);
}
