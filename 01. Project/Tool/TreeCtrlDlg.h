#pragma once


/*
TreeCtrlDlg �Լ��� TreeCtrl�� ����ϴ� HierachyDlg, ResListDlg���� ��ӹ޾� ����ϸ�,
OnCancel OnOK PostNCDestroy ���� �Լ��� �������Ͽ� ������ ó���� �ϱ� ���ؼ� ����
�θ� Ŭ�����̴�. predecessor
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

	HTREEITEM					m_hDragItem; // �巡�� ���� �������� �ڵ�.
	HTREEITEM					m_hDestItem; // �巡�� ������ �������� �ڵ�.

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
		EventMgr�� ���ؼ� ���ӿ�����Ʈ�� �˾ƿ��� �̰Ϳ� ���� ó���� �ؾ� �ϴ� ��찡
		���� �����. ����, DWORD_PTR(��, �ּ�)�� �־��ָ� �װͿ� ���� HTREEITEM�� ��ȯ�� ��
		�ֵ��� map�� ���ؼ� ����Ÿ�� �����صд�.
		*/
	}

	void DeleteItemByData(DWORD_PTR dwData)
	{
		// Key(DWORD_PTR)�� �־��ָ�, �ش� Ű�� ���� pair�� �����ִ� �Լ�.
		map<DWORD_PTR, HTREEITEM>::iterator iter = m_mapHandle.find(dwData);

		if (iter == m_mapHandle.end())
		{
			// �ش� �����͸� ���� pair�� �����Ϸ��� ������, pair�� ã�� ����.
			// Error�� ����� ��Ȳ�̴�.
		}
		else
		{
			// �����ش�.
			m_mapHandle.erase(iter);
		}
	}

	void DeleteItem(HTREEITEM hItem); 
	// handle�� �־��ָ�, �ش� handle�� �ڽı��� ����Ͽ�
	// �����ִ� ó���� �ϰ� �ִ� �Լ��̴�.

	void MoveItem(HTREEITEM hSrcItem, HTREEITEM hDestItem); // �������� �������踦 ����Ͽ� �ű�� �Լ�.
	void GetChildItem(HTREEITEM hParent, vector<HTREEITEM>& vecItem); // �־��� �ڵ��� �ڽĿ� �ش��ϴ� �ڵ��� ��ȯ�ϴ� �Լ�.
	bool IsAncestor(HTREEITEM hParent, HTREEITEM hChild); // �־��� �θ� �ڵ�� �ڽ� �ڵ鰣�� �������� ���� ��ȯ�Լ�.

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

