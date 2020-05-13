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

	st.hParent = hParent; // 부모 아이템을 지정.
	st.hInsertAfter = TVI_SORT; // 정렬을 사용
	st.item.pszText = strItem.GetBuffer();
	st.item.mask = TVIF_TEXT; // 텍스트를 이용.

	HTREEITEM hItem = m_EditTree.InsertItem(&st); // 위에서 적은 정보를 이용하여 아이템을 등록.
	m_EditTree.SetItemData(hItem, dwData); // 아이템에 데이타를 등록

	// key + data : Res의 주소를 넣어놓고, Get하여 얻어온 것이 주소.

	if (dwData != 0)
	{
		m_mapHandle.insert(make_pair(dwData, hItem));
	}
	// 0의 값을 넣어두면 처리하지 않는다.


	return hItem;
}

void CTreeCtrlDlg::DeleteItem(HTREEITEM hItem)
{
	/*
	부모를 삭제하여 Handle의 관계를 없애버리고,
	map에서 해당 오브젝트의 주소 정보를 없애버린다. (중복키 불가)
	*/

	list<HTREEITEM> listChild;
	listChild.push_back(hItem);

	HTREEITEM hTarget = nullptr;
	while (!listChild.empty())
	{
		// BFS 방식과 동일.
		hTarget = listChild.front();
		listChild.pop_front();

		// 하나 넣고, 그것을 빼서, pair에서 없애주고
		DeleteItemByData(m_EditTree.GetItemData(hTarget));

		// 타겟에 딸린 자식이 있다면.
		if (m_EditTree.ItemHasChildren(hTarget))
		{
			HTREEITEM hChildItem = m_EditTree.GetChildItem(hTarget); 
			// 해당 타겟의 자식을 가져온다.

			while (hChildItem != nullptr)
			{
				listChild.push_back(hChildItem);
				hChildItem = m_EditTree.GetNextSiblingItem(hChildItem);
				// 모든 자식을 list에 추가.
			}
		}
	}

	// 하나 잡고, 정보를 map에서 빼주고, 그것의 자식을 모두 넣어서 다음 자식들에 정보를 제거하고 그 자식들을 추가하고..
	// bfs의 방식대로 처리가 된다.
	// 결국 최하위 노드까지 순회하면서 list에 추가가 되며, map에 있는 정보도 모두 사라진다.
	// 부모를 없애서 자식까지 없애는 처리를 한다.
	// +  map에 있는 정보 또한 삭제

	

	m_EditTree.DeleteItem(hItem);
}

void CTreeCtrlDlg::MoveItem(HTREEITEM hSrcItem, HTREEITEM hDestItem)
{
	// 자기 자신으로 드롭되는 경우, 드래그 아이템이 없는 경우. 
	if (hSrcItem == hDestItem || hSrcItem == nullptr)
		return;

	wchar_t szName[255] = {};

	/*
	TV_ITEM 구조체는 출력과 입력 동시에 사용이 되는 함수이다.
	*/

	TV_ITEM tItem = {};
	tItem.hItem = hSrcItem;
	tItem.pszText = szName;
	tItem.cchTextMax = 255;
	tItem.mask = TVIF_TEXT | TVIF_HANDLE;
	DWORD_PTR dwData = m_EditTree.GetItemData(hSrcItem);

	m_EditTree.GetItem(&tItem);
	// 해당 핸들로부터 이름을 받아온다.

	// 목적지에 새로운 아이템을 집어넣는다.
	HTREEITEM hCopyItem = AddItem(szName, hDestItem);
	m_EditTree.SetItemData(hCopyItem, dwData);

	// 원본 아이템이 자식이 있는 경우
	vector<HTREEITEM> vecChild;
	GetChildItem(hSrcItem, vecChild);

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		MoveItem(vecChild[i], hCopyItem);
	}

	m_EditTree.DeleteItem(hSrcItem);

	// 목적지가 있으면... 최상위 오브젝트가 되는 경우가 아니라면 펼쳐준다.
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

	// 선택한 셀을 알아내기 위해 Point를 하이어라키를 기준으로 한 상대좌표로 변환한다.

	ClientToScreen(&point);
	::ScreenToClient(m_EditTree.m_hWnd, &point);
	// ::ScreenToClient 전역함수. 핸들을 추가적으로 받는다.
	// ScreenToClient 멤버함수.

	// TreeCtrl의 HitTest함수는 Client영역을 기준으로 하여,
	// 넣어준 Point에 대해서 셀과 충돌했는지 판단해준다.
	m_hDestItem = m_EditTree.HitTest(point);

	// 목적지 != 도착지면 그곳을 체크.
	if (m_hDragItem != m_hDestItem)
		m_EditTree.SelectDropTarget(m_hDestItem);

	else
	{
		m_EditTree.SelectDropTarget(nullptr);
		m_hDestItem = nullptr;
	}
	// 같은 경우에는 체크하지 않음.

	CDialogEx::OnMouseMove(nFlags, point);
}


void CTreeCtrlDlg::OnBeginDrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	m_hDragItem = pNMTreeView->itemNew.hItem; // 드래그한 아이템의 핸들을 설정.

	SetCapture(); 
	// 윈도우의 메세지를 가로챈다.
	// 마우스와 관련된 메세지의 경우에는 바로 아래에 있는 윈도우가 메세지를 가져가는 형태로
	// 되어 있다.
	// 따라서, MouseMove에 대한 처리를 하기 위해서 해당 윈도우가 항상 메세지를 받을 필요가 있따.
	
	// 예를 들면. 하이어라키 Dlg에서 드래그를 시작하여 GameView쪽으로 옮겨진 경우에
	// 윈도우의 범위를 벗어났으나, 그에 대한 처리는 지속되어야 할 것이다.
	// 즉, 윈도우의 영역을 벗어났음에도 마우스에 대한 처리를 지속적으로 하길 원하는 경우에
	// 사용하는 함수가 SetCapture이고, 이것을 선언하면, 해당 윈도우의 범위를 벗어나는 경우에도
	// MouseMove에 대한 메세지를 받을 수 있게된다.

	DragFlag(); // DragFlag에 대한 값을 저장하는 함수로, ResList에서 사용한다.

	*pResult = 0;
}


void CTreeCtrlDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	
	// Drag 동작을 끝내고, 왼쪽 버튼을 올린상황이다.
	// 윈도우의 영역을 벗어난 상태에서 계속 mouseMove 메세지를 처리했을 것이고,
	// 뗀 상태 이후에는 더 이상 마우스에 대한 처리를 하지 않아야 한다.
	ReleaseCapture();

	// 예외 1 : 같은 오브젝트를 가지고 같은 곳에 넣으려고 하는 경우. DestItem을 nullptr.
	// 이 경우에는 처리하지 않는다.
	if (m_hDestItem == nullptr)
		return;


	// 드래그 아이템을 목적지에 넣는다.
	// DragItem이 Dest의 상위 계층이라면. 처리하지 않는다.
	if (!IsAncestor(m_hDragItem, m_hDestItem))
	{
		DragDrop(m_hDragItem, m_hDestItem); // 게임내 오브젝트에 대해서 계층관계 적용.
		MoveItem(m_hDragItem, m_hDestItem); // 트리에서의 적용.
	}

	// 드롭 타겟 해제.
	m_EditTree.SelectDropTarget(nullptr);

	// 드래그, 목적지 초기화.
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
