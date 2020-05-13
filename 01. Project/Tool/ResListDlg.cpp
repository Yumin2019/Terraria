// ResListDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "ResListDlg.h"
#include "afxdialogex.h"

#include <Engine/PathMgr.h>
#include <Engine/ResMgr.h>

// res
#include <Engine/Texture.h>
#include <Engine/Sound.h>
#include <Engine/Mesh.h>
#include <Engine/Material.h>
#include <Engine/Shader.h>

// CResListDlg 대화 상자

IMPLEMENT_DYNAMIC(CResListDlg, CDialogEx)

CResListDlg::CResListDlg(CWnd* pParent /*=nullptr*/) :
	m_eDragType(RES_TYPE::END)
{

}

CResListDlg::~CResListDlg()
{
}

bool CResListDlg::Init()
{
	CRect rt;
	GetClientRect(rt);

	m_EditTree.SetWindowPos(nullptr, 0, 0, rt.Width(), rt.Height(), 0);

	// Content File Resource Loading
	LoadResource();

	// 트리컨트롤 내용 갱신.
	Renew();

	return true;
}

void CResListDlg::Update()
{
}

void CResListDlg::Renew()
{
	m_EditTree.DeleteAllItems();

	HTREEITEM hParent = nullptr;

	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		// MESH라면.. 최상위 핸들 추가.
		hParent = AddItem(RES_TYPE_NAME[i], nullptr);

		const map<wstring, CResource*>& mapRes = CResMgr::GetInst()->GetResMap((RES_TYPE)i);

		map<wstring, CResource*>::const_iterator iter;
		map<wstring, CResource*>::const_iterator iterEnd = mapRes.end();

		for (iter = mapRes.begin(); iter != iterEnd; ++iter)
		{
			// ResMgr로부터  해당 Resource map을 받아서, 순회를 하면서 리소스 추가.
			// 부모는 위에서 정한 핸들로..
			AddItem(CPathMgr::GetFileName(iter->first.c_str()), hParent, (DWORD_PTR)iter->second);
		}
	}

	// 모든 리소스에 대해서 리소스
	// 리소스에 해당하는 것들...
}

void CResListDlg::DragFlag()
{
	HTREEITEM hParent = m_EditTree.GetParentItem(m_hDragItem);

	if (hParent == nullptr) // 최상위 부모 (resName)
		return;

		// 부모의 이름을 토대로 판단을 한다.
		CString strName = m_EditTree.GetItemText(hParent);

		for (size_t i = 0; i < (UINT)RES_TYPE::END; ++i)
		{
			if (strName == RES_TYPE_NAME[i])
			{
				m_eDragType = (RES_TYPE)i;
				break;
			}
		}


}

void CResListDlg::LoadResource()
{
	CString strContent = CPathMgr::GetResPath();

	LoadResource(strContent);
}

void CResListDlg::LoadResource(const CString & strFolderPath)
{
	CString strPath = strFolderPath + L"/*.*";
	WIN32_FIND_DATA tData = {};
	HANDLE hFindHandle = FindFirstFile(strPath, &tData);
	CString strFileName;

	while (true)
	{
		strFileName = tData.cFileName;

		if (tData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strFileName != L"." && strFileName != L"..")
			{
				if(strFolderPath[strFolderPath.GetLength() - 1] != L'/')
					LoadResource(strFolderPath + L"/" + tData.cFileName);

				else
					LoadResource(strFolderPath + tData.cFileName);
			}

			// 윈도우에서는 숨김파일로 되어있는 .과 ..이라는 이름의 파일이 자동으로 생기게 된다.
			// 이 파일은 무시하고 실제 파일인 경우에는 다시 함수를 호출하여 그곳에서 파일을 찾도록 한다.
		}
		else
		{
			// 폴더가 아닌 리소스 파일.
			Load(strFolderPath +  L"/" + strFileName);
		}

		// 다음 파일을 찾아온다. 찾아오지 못한 경우에는 break;
		if (!FindNextFile(hFindHandle, &tData))
			break;
	}

	FindClose(hFindHandle);
}

void CResListDlg::Load(const CString & strFullPath)
{
	CString strExt = CPathMgr::GetExt(strFullPath);
	CString strRelativePath = CPathMgr::GetRelativePath(strFullPath);

	if (strExt == L".png" || strExt == L".tga" || strExt == L".dds" || strExt == L".jpeg" || strExt == L".jpg" || strExt == L".bmp")
	{
		// Texture
		CResMgr::GetInst()->Load<CTexture>(strRelativePath.GetBuffer(), strRelativePath.GetBuffer());

		// 불러온 리소스가 절대로 키값이 겹치지 않도록 상대 좌표를 이용하여 Key를 등록한다.
		// 상대좌표를 넘겨주면 그것을 절대좌표로 만들어서 리소스를 읽어온다.
	}
	else if (strExt == L".mtrl")
	{
		// Material
		CResMgr::GetInst()->Load<CMaterial>(strRelativePath.GetBuffer(), strRelativePath.GetBuffer());

	}
	else if (strExt == L".mp3" || strExt == L".wav" || strExt == L".ogg")
	{
		// Sound
	}
	else if (strExt == L".fbx" || strExt == L".FBX" || strExt == L".mdat")
	{
		// Mesh
		CResMgr::GetInst()->Load<CMesh>(strRelativePath.GetBuffer(), strRelativePath.GetBuffer());

	}
}

void CResListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_EditTree);
}


BEGIN_MESSAGE_MAP(CResListDlg, CDialogEx)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE1, &CTreeCtrlDlg::OnBeginDrag)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CResListDlg 메시지 처리기

#include "MainFrame.h"
#include "InfoView.h"
#include "MeshRenderDlg.h"
#include "Animator2DDlg.h"

#include "MtrlDataDlg.h"
#include "AddAnimDlg.h"

void CResListDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	// 뗀 상태에서 Point를 Screen을 기준으로 바꾼다.
	// 강제호출. MtrlDataDlg를 가져온다. InfoView->MeshRenderDlg->GetMtrlDlg;
	CMeshRenderDlg* pMeshRenderDlg = (CMeshRenderDlg*)((CInfoView*)((CMainFrame*)AfxGetMainWnd())->GetInfoView())->GetComDlg(COMPONENT_TYPE::MESHRENDER);
	CAnimator2DDlg* pAnimator = (CAnimator2DDlg*)((CInfoView*)((CMainFrame*)AfxGetMainWnd())->GetInfoView())->GetComDlg(COMPONENT_TYPE::ANIMATOR2D);

	CMtrlDataDlg* pMtrlData = pMeshRenderDlg->GetMtrlDataDlg();
	CAddAnimDlg* pAddAnim = pAnimator->GetAddAnimDlg();


	ClientToScreen(&point);


	// 창이 켜져 있지 않으면 처리하지 않는다.
	if (pMeshRenderDlg->IsShow())
	{
		if (m_eDragType == RES_TYPE::TEXTURE)
		{

			int idx = pMtrlData->IsCollisionTex(point);

			if (idx != -1)
			{

				if (m_hDragItem)
				{
					// instance tex Res*
					pMtrlData->CollisionTex(m_EditTree.GetItemText(m_hDragItem), m_EditTree.GetItemData(m_hDragItem), idx);
				}
				else
				{
					//nullptr 셋팅
					pMtrlData->CollisionTex(L"", 0, idx);

				}
			}
		}

		// 다른 Type은 나중에 처리하자.
	}
	
	// 애니메이터
	if (pAnimator->IsShow())
	{
		if (m_eDragType == RES_TYPE::TEXTURE)
		{

			int idx = pAddAnim->IsCollisionTex(point);

			if (idx != -1)
			{
				if (m_hDragItem)
				{
					pAddAnim->CollisionTex(m_EditTree.GetItemText(m_hDragItem), m_EditTree.GetItemData(m_hDragItem), idx);
				}
				else
				{
					//nullptr 셋팅
					pAddAnim->CollisionTex(L"", 0, idx);
				}
			}
		}
	}
	
	
	m_eDragType = RES_TYPE::END;
	m_hDragItem = nullptr;
	m_hDestItem = nullptr;

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CResListDlg::PostNcDestroy()
{
	// 현재 Mtr의 상태를 저장한다.
	const map<wstring, CResource*>& mapMtrl = CResMgr::GetInst()->GetResMap(RES_TYPE::MATERIAL);

	wstring strPath = CPathMgr::GetResPath();

	for (const auto& pair : mapMtrl)
	{
		pair.second->Save(strPath);
		// 참고로 Play Mode 상태임
	}

	delete this;
}
