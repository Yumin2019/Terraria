#include "stdafx.h"
#include "TreeCtrlDlg.h"


CTreeCtrlDlg::CTreeCtrlDlg()
{
}


CTreeCtrlDlg::~CTreeCtrlDlg()
{
}


HTREEITEM CTreeCtrlDlg::AddItem(CString strItem, HTREEITEM hParent, DWORD_PTR dwData)
{
	TVINSERTSTRUCT st = {};

	st.hParent = hParent; // �θ� �������� ����.
	st.hInsertAfter = TVI_SORT; // ������ ���
	st.item.pszText = strItem.GetBuffer();
	st.item.mask = TVIF_TEXT; // �ؽ�Ʈ�� �̿�.

	HTREEITEM hItem = m_EditTree.InsertItem(&st); // ������ ���� ������ �̿��Ͽ� �������� ���.
	m_EditTree.SetItemData(hItem, dwData); // �����ۿ� ����Ÿ�� ���

	// key + data : Res�� �ּҸ� �־����, Get�Ͽ� ���� ���� �ּ�.

	if (dwData != 0)
	{
		m_mapHandle.insert(make_pair(dwData, hItem));
	}
	// 0�� ���� �־�θ� ó������ �ʴ´�.


	return hItem;
}

void CTreeCtrlDlg::DeleteItem(HTREEITEM hItem)
{
	/*
	�θ� �����Ͽ� Handle�� ���踦 ���ֹ�����,
	map���� �ش� ������Ʈ�� �ּ� ������ ���ֹ�����. (�ߺ�Ű �Ұ�)
	*/

	list<HTREEITEM> listChild;
	listChild.push_back(hItem);

	HTREEITEM hTarget = nullptr;
	while (!listChild.empty())
	{
		// BFS ��İ� ����.
		hTarget = listChild.front();
		listChild.pop_front();

		// �ϳ� �ְ�, �װ��� ����, pair���� �����ְ�
		DeleteItemByData(m_EditTree.GetItemData(hTarget));

		// Ÿ�ٿ� ���� �ڽ��� �ִٸ�.
		if (m_EditTree.ItemHasChildren(hTarget))
		{
			HTREEITEM hChildItem = m_EditTree.GetChildItem(hTarget); 
			// �ش� Ÿ���� �ڽ��� �����´�.

			while (hChildItem != nullptr)
			{
				listChild.push_back(hChildItem);
				hChildItem = m_EditTree.GetNextSiblingItem(hChildItem);
				// ��� �ڽ��� list�� �߰�.
			}
		}
	}

	// �ϳ� ���, ������ map���� ���ְ�, �װ��� �ڽ��� ��� �־ ���� �ڽĵ鿡 ������ �����ϰ� �� �ڽĵ��� �߰��ϰ�..
	// bfs�� ��Ĵ�� ó���� �ȴ�.
	// �ᱹ ������ ������ ��ȸ�ϸ鼭 list�� �߰��� �Ǹ�, map�� �ִ� ������ ��� �������.
	// �θ� ���ּ� �ڽı��� ���ִ� ó���� �Ѵ�.
	// +  map�� �ִ� ���� ���� ����

	

	m_EditTree.DeleteItem(hItem);
}

void CTreeCtrlDlg::MoveItem(HTREEITEM hSrcItem, HTREEITEM hDestItem)
{
	// �ڱ� �ڽ����� ��ӵǴ� ���, �巡�� �������� ���� ���. 
	if (hSrcItem == hDestItem || hSrcItem == nullptr)
		return;

	wchar_t szName[255] = {};

	/*
	TV_ITEM ����ü�� ��°� �Է� ���ÿ� ����� �Ǵ� �Լ��̴�.
	*/

	TV_ITEM tItem = {};
	tItem.hItem = hSrcItem;
	tItem.pszText = szName;
	tItem.cchTextMax = 255;
	tItem.mask = TVIF_TEXT | TVIF_HANDLE;
	DWORD_PTR dwData = m_EditTree.GetItemData(hSrcItem);

	m_EditTree.GetItem(&tItem);
	// �ش� �ڵ�κ��� �̸��� �޾ƿ´�.

	// �������� ���ο� �������� ����ִ´�.
	HTREEITEM hCopyItem = AddItem(szName, hDestItem);
	m_EditTree.SetItemData(hCopyItem, dwData);

	// ���� �������� �ڽ��� �ִ� ���
	vector<HTREEITEM> vecChild;
	GetChildItem(hSrcItem, vecChild);

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		MoveItem(vecChild[i], hCopyItem);
	}

	m_EditTree.DeleteItem(hSrcItem);

	// �������� ������... �ֻ��� ������Ʈ�� �Ǵ� ��찡 �ƴ϶�� �����ش�.
	if (hDestItem != nullptr)
		m_EditTree.Expand(hDestItem, TVE_EXPAND);
}

void CTreeCtrlDlg::GetChildItem(HTREEITEM hParent, vector<HTREEITEM>& vecItem)
{
	HTREEITEM hChild = m_EditTree.GetChildItem(hParent);

	while (hChild)
	{
		vecItem.push_back(hChild);
		hChild = m_EditTree.GetNextSiblingItem(hChild);
	}
}

bool CTreeCtrlDlg::IsAncestor(HTREEITEM hParent, HTREEITEM hChild)
{
	HTREEITEM hItem = m_EditTree.GetParentItem(hChild);

	while (hItem != nullptr)
	{
		if (hItem == hParent)
			return true;

		hItem = m_EditTree.GetParentItem(hItem);
	}

	return false;
}


BEGIN_MESSAGE_MAP(CTreeCtrlDlg, CDialogEx)
END_MESSAGE_MAP()


void CTreeCtrlDlg::OnMouseMove(UINT nFlags, CPoint point)
{

	// ������ ���� �˾Ƴ��� ���� Point�� ���̾��Ű�� �������� �� �����ǥ�� ��ȯ�Ѵ�.

	ClientToScreen(&point);
	::ScreenToClient(m_EditTree.m_hWnd, &point);
	// ::ScreenToClient �����Լ�. �ڵ��� �߰������� �޴´�.
	// ScreenToClient ����Լ�.

	// TreeCtrl�� HitTest�Լ��� Client������ �������� �Ͽ�,
	// �־��� Point�� ���ؼ� ���� �浹�ߴ��� �Ǵ����ش�.
	m_hDestItem = m_EditTree.HitTest(point);

	// ������ != �������� �װ��� üũ.
	if (m_hDragItem != m_hDestItem)
		m_EditTree.SelectDropTarget(m_hDestItem);

	else
	{
		m_EditTree.SelectDropTarget(nullptr);
		m_hDestItem = nullptr;
	}
	// ���� ��쿡�� üũ���� ����.

	CDialogEx::OnMouseMove(nFlags, point);
}


void CTreeCtrlDlg::OnBeginDrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	m_hDragItem = pNMTreeView->itemNew.hItem; // �巡���� �������� �ڵ��� ����.

	SetCapture(); 
	// �������� �޼����� ����æ��.
	// ���콺�� ���õ� �޼����� ��쿡�� �ٷ� �Ʒ��� �ִ� �����찡 �޼����� �������� ���·�
	// �Ǿ� �ִ�.
	// ����, MouseMove�� ���� ó���� �ϱ� ���ؼ� �ش� �����찡 �׻� �޼����� ���� �ʿ䰡 �ֵ�.
	
	// ���� ���. ���̾��Ű Dlg���� �巡�׸� �����Ͽ� GameView������ �Ű��� ��쿡
	// �������� ������ �������, �׿� ���� ó���� ���ӵǾ�� �� ���̴�.
	// ��, �������� ������ ��������� ���콺�� ���� ó���� ���������� �ϱ� ���ϴ� ��쿡
	// ����ϴ� �Լ��� SetCapture�̰�, �̰��� �����ϸ�, �ش� �������� ������ ����� ��쿡��
	// MouseMove�� ���� �޼����� ���� �� �ְԵȴ�.

	DragFlag(); // DragFlag�� ���� ���� �����ϴ� �Լ���, ResList���� ����Ѵ�.

	*pResult = 0;
}


void CTreeCtrlDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	
	// Drag ������ ������, ���� ��ư�� �ø���Ȳ�̴�.
	// �������� ������ ��� ���¿��� ��� mouseMove �޼����� ó������ ���̰�,
	// �� ���� ���Ŀ��� �� �̻� ���콺�� ���� ó���� ���� �ʾƾ� �Ѵ�.
	ReleaseCapture();

	// ���� 1 : ���� ������Ʈ�� ������ ���� ���� �������� �ϴ� ���. DestItem�� nullptr.
	// �� ��쿡�� ó������ �ʴ´�.
	if (m_hDestItem == nullptr)
		return;


	// �巡�� �������� �������� �ִ´�.
	// DragItem�� Dest�� ���� �����̶��. ó������ �ʴ´�.
	if (!IsAncestor(m_hDragItem, m_hDestItem))
	{
		DragDrop(m_hDragItem, m_hDestItem); // ���ӳ� ������Ʈ�� ���ؼ� �������� ����.
		MoveItem(m_hDragItem, m_hDestItem); // Ʈ�������� ����.
	}

	// ��� Ÿ�� ����.
	m_EditTree.SelectDropTarget(nullptr);

	// �巡��, ������ �ʱ�ȭ.
	m_hDragItem = nullptr;
	m_hDestItem = nullptr;

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CTreeCtrlDlg::OnOK()
{
}


void CTreeCtrlDlg::OnCancel()
{
}


void CTreeCtrlDlg::PostNcDestroy()
{
	delete this;
	CDialogEx::PostNcDestroy();
}
