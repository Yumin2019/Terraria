#pragma once


/*
TreeCtrlDlg 함수는 TreeCtrl을 사용하는 HierachyDlg, ResListDlg에서 상속받아 사용하며,
OnCancel OnOK PostNCDestroy 같은 함수를 재정의하여 동일한 처리를 하기 위해서 만든
부모 클래스이다. predecessor
*/
class CTreeCtrlDlg :
	public CDialogEx
{
public:
	CTreeCtrlDlg();
	~CTreeCtrlDlg();

protected:
	CTreeCtrl					m_EditTree;
	map<DWORD_PTR, HTREEITEM>	m_mapHandle;

	HTREEITEM					m_hDragItem; // 드래그 중인 아이템의 핸들.
	HTREEITEM					m_hDestItem; // 드래그 목적지 아이템의 핸들.

public:
	virtual bool Init() = 0;
	virtual void Update() = 0;
	virtual void Renew() = 0;

protected:
	HTREEITEM AddItem(CString strItem, HTREEITEM hParent, DWORD_PTR dwData = 0);
	HTREEITEM FindItemByData(DWORD_PTR dwData)
	{
		map<DWORD_PTR, HTREEITEM>::iterator iter = m_mapHandle.find(dwData);

		if (iter == m_mapHandle.end())
			return nullptr;

		return iter->second;

		/*
		EventMgr를 통해서 게임오브젝트를 알아오고 이것에 대한 처리를 해야 하는 경우가
		많이 생긴다. 따라서, DWORD_PTR(즉, 주소)를 넣어주면 그것에 대한 HTREEITEM을 반환할 수
		있도록 map을 통해서 데이타를 구현해둔다.
		*/
	}

	void DeleteItemByData(DWORD_PTR dwData)
	{
		// Key(DWORD_PTR)을 넣어주면, 해당 키를 가진 pair를 없애주는 함수.
		map<DWORD_PTR, HTREEITEM>::iterator iter = m_mapHandle.find(dwData);

		if (iter == m_mapHandle.end())
		{
			// 해당 데이터를 가진 pair를 삭제하려고 했으나, pair를 찾지 못함.
			// Error에 가까운 상황이다.
		}
		else
		{
			// 없애준다.
			m_mapHandle.erase(iter);
		}
	}

	void DeleteItem(HTREEITEM hItem); 
	// handle을 넣어주면, 해당 handle의 자식까지 고려하여
	// 없애주는 처리를 하고 있는 함수이다.

	void MoveItem(HTREEITEM hSrcItem, HTREEITEM hDestItem); // 아이템을 계층관계를 고려하여 옮기는 함수.
	void GetChildItem(HTREEITEM hParent, vector<HTREEITEM>& vecItem); // 넣어준 핸들의 자식에 해당하는 핸들을 반환하는 함수.
	bool IsAncestor(HTREEITEM hParent, HTREEITEM hChild); // 넣어준 부모 핸들과 자식 핸들간의 계층관계 여부 반환함수.

	virtual void DragDrop(HTREEITEM hSrcItem, HTREEITEM hDestItem) {}
	virtual void DragFlag() {}
public:


	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBeginDrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
};

