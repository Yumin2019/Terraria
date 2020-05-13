#pragma once


#include "PopUpDlg.h"

#define TILE_MAX 15

// TILEINFO에서 사용한다.
#define IMAGE_NUM_X 32
#define IMAGE_NUM_Y 54



// CTileDlg 대화 상자

class CTileDlg : public CPopUpDlg
{
	DECLARE_DYNAMIC(CTileDlg)

public:
	CTileDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTileDlg();

private:
	CImage	m_ImageArr[TILE_MAX];
	Vec2	m_TileSize[TILE_MAX];
	int		m_iIdx; // Image Num

	// 타일 이미지에서 각 타일마다 적용할 옵션 값을 설정하기 위한 동적 배열이다.
	// 이 정보를 Save / Load 를 통해서 이용하도록 한다.
	TILE*	m_pTileOption; 

	// GetCursel을 통해서 알아오자.
	// 현재 타일 옵션을 의미한다. 
	// 현재 타일 모드이다.

	
	int		m_iTileNum; // Image에서 타일이 존재하는 개수이다. (TILE_MODE :: OPTION에서 사용)

	// 선택한 타일의 인덱스.
	int		m_iRow;
	int		m_iCol;

	bool	m_bUpdate;


	// OPTION MODE
	int			m_iStartX;
	int			m_iStartY;
	bool		m_bSave;

public:
	static TileInfo	g_tTileInfo[(UINT)TILE::END];


private:
	void SetTileIdx(int iRow, int iCol);
	TILE_MODE GetMode() const { return (TILE_MODE)m_Mode.GetCurSel(); }

public:
	int GetTileIdxX() const { return m_iCol; }
	int GetTileIdxY() const { return m_iRow; }

public:

	Vec2 GetImageSize(int iIdx) { return Vec2(m_ImageArr[iIdx].GetWidth(), m_ImageArr[iIdx].GetHeight()); }

	// 실제 사이즈
	int GetTileSizeX(int iIdx) const { return m_TileSize[iIdx].x - 2.f; }
	int GetTileSizeY(int iIdx) const { return m_TileSize[iIdx].y - 2.f; }

	// 간격 : LeftTop에서 사용.
	int GetTileGapX(int iIdx) const { return m_TileSize[iIdx].x; }
	int GetTileGapY(int iIdx) const { return m_TileSize[iIdx].y; }

	int GetTextureIdx() const { return m_iIdx; }

	Vec2 GetLeftTop()// 현재 선택된 타일의 왼쪽 상단 위치를 반환.
	{
		return Vec2(m_iCol * GetTileGapX(m_iIdx), m_iRow * GetTileGapY(m_iIdx));
	}

	TILE* GetTileOption() { return m_pTileOption; }

public:
	bool Init();
	void DrawTexture(int iSrcX, int iSrcY);
	void DrawLine();
	// dest 0 0  고정. width height picture ctrl의 사이즈. src에서의 위치. src에서의 size는 크거나 작을 가능성이 있으므로
	// 따로 설정하자.
	// 다시 생각을 해봤는데, src에서의 size는 함수의 내부적으로 맞춰서 처리를 하는 것이 맞을 것 같다.

	// 선택한 타일을 출력해준다.
	void DrawTile();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TILEDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	CStatic m_EditPic;
	afx_msg void OnBnClickedLeft();
	afx_msg void OnBnClickedRight();
	CComboBox m_EditCombo;
	afx_msg void OnChangedCombo();
	CScrollBar m_EditScrollH;
	CScrollBar m_EditScrollW;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CEdit m_EditX;
	CEdit m_EditY;
	CStatic m_EditTile;
	virtual void OnCancel();
	CEdit m_SizeX;
	CEdit m_SizeY;
	CComboBox m_Mode;
	CComboBox m_Option;
	afx_msg void OnChangedTileMode();
	afx_msg void OnChangedTileOption();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};
