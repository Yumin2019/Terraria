// ListDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "ListDlg.h"
#include "afxdialogex.h"


// CListDlg 대화 상자

IMPLEMENT_DYNAMIC(CListDlg, CDialogEx)


CListDlg::CListDlg(const CString & strCaption, const vector<CString>& vecList) :
	 CDialogEx(IDD_LIST, nullptr),
	m_strCaption(strCaption),
	m_vecList(vecList)
{
	// 부모 윈도우 nullptr 주고, Caption / Vec를 생성자로 받도록 한다.
}

CListDlg::~CListDlg()
{
}

void CListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_EditList);

}


BEGIN_MESSAGE_MAP(CListDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CListDlg::OnDoubleClick)
END_MESSAGE_MAP()


// CListDlg 메시지 처리기


BOOL CListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 캡션 내용 설정.
	SetWindowTextW(m_strCaption);

	// 목록 추가
	for (size_t i = 0; i < m_vecList.size(); ++i)
	{
		m_EditList.InsertItem(i, m_vecList[i]);
	}

	return TRUE; 
}


void CListDlg::OnDoubleClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	m_iSelect = pNMItemActivate->iItem;

	if (m_iSelect == -1) // 바깥 클릭.
	{
		CDialogEx::OnCancel();
		return;
	}


	// 제대로 선택한 경우에 선택한 텍스트를 뽑아내서 저장하고 종료.
	m_strSelect = m_EditList.GetItemText(m_iSelect, 0);
	CDialogEx::OnOK();
}


void CListDlg::OnOK()
{
	m_iSelect = m_EditList.GetNextItem(-1, LVIS_SELECTED);

	if (m_iSelect == -1)
	{
		// 제대로 선택하지 않고, Enter를 누른 경우에는 종료가 되도록 했다.
		CDialogEx::OnCancel();
		return;
	}

	
	m_strSelect = m_EditList.GetItemText(m_iSelect, 0);
	CDialogEx::OnOK();
}
