// TransformDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "TransformDlg.h"
#include "afxdialogex.h"
#include "MainFrame.h"

#include <Engine/Transform.h>

// CTransformDlg 대화 상자

IMPLEMENT_DYNAMIC(CTransformDlg, CDialogEx)

CTransformDlg::CTransformDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(COMPONENT_TYPE::TRANSFORM)
{
	m_tPivotArr[0] = Vec2(0.f, 1.f);
	m_tPivotArr[1] = Vec2(1.f, 1.f);
	m_tPivotArr[2] = Vec2(1.f, 0.f);
	m_tPivotArr[3] = Vec2(0.f, 0.f);
	m_tPivotArr[4] = Vec2(0.5f, 0.5f);
}

CTransformDlg::~CTransformDlg()
{
}

void CTransformDlg::UpdateCtrl()
{
	SetCtrl(true);
	// Target으로 부터 얻어온 Transform정보를 컨트롤에 띄워준다.
	CTransform* pTrans = GetTarget()->GetTransform();
	Vec3 tTransform[3] = { pTrans->GetLocalPos(), pTrans->GetLocalScale(), pTrans->GetLocalRot() };

	for (size_t i = 0; i < 3; ++i)
		tTransform[2][i] = XMConvertToDegrees(tTransform[2][i]);

	CString str;

	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			str.Format(L"%f", tTransform[i][j]);
			m_Edit[i * 3 + j].SetWindowTextW(str);
		}
	}

	// Pivot 나중에 추가될 여지를 둔다.

	for (size_t i = 0; i < 5; ++i)
	{
		const Vec2& tPivot = pTrans->GetPivot();
		
		if (tPivot == m_tPivotArr[i])
		{
			m_Pivot.SetCurSel(i);
			break;
		}
	}
	
	EnableUpdate();
	SetCtrl(false);

}


void CTransformDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	int id = IDC_EDIT1;
	for (size_t i = 0; i < 9; ++i)
	{
		DDX_Control(pDX, id + i, m_Edit[i]);
	}
	DDX_Control(pDX, IDC_COMBO1, m_Pivot);

	m_Pivot.AddString(L"Left Top");
	m_Pivot.AddString(L"Right Top");
	m_Pivot.AddString(L"Right Bottom");
	m_Pivot.AddString(L"Left Bottom");
	m_Pivot.AddString(L"Middle");

}


BEGIN_MESSAGE_MAP(CTransformDlg, CDialogEx)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT1, IDC_EDIT9, &CTransformDlg::EditPresent)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT1, IDC_EDIT9, &CComponentDlg::OnKillFocus)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT1, IDC_EDIT9, &CComponentDlg::OnSetFocus)
	
	ON_CBN_SELCHANGE(IDC_COMBO1, &CTransformDlg::OnChangedPivot)
	ON_BN_CLICKED(IDC_BUTTON1, &CTransformDlg::OnClicked2DScaleBtn)
END_MESSAGE_MAP()


// CTransformDlg 메시지 처리기




void CTransformDlg::EditPresent(UINT id)
{
	if (IsSetCtrl())
		return;

	CString str;

	Vec3 tData[3] = {};

	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			m_Edit[i * 3 + j].GetWindowTextW(str);
			tData[i][j] = _wtof(str);
			// [] operator를 연산자로 처리하여, *(this + i)의 형태로 되어있다.
			// 따라서, i = 0이면 x를 i = 1이면 y를 의미한다.
		}
	}

	// 이렇게 각 EditCtrl을 통해서 얻은 문자열을 float형으로 변환하고,
	// Vec3[3]에 저장한다. (0 Pos 1 Scale..)
	// 이것을 Target의 값으로 설정한다.

	for (size_t i = 0; i < 3; ++i)
		tData[2][i] = XMConvertToRadians(tData[2][i]);

	GetTarget()->GetTransform()->SetLocalPos(tData[0]);
	GetTarget()->GetTransform()->SetLocalScale(tData[1]);
	GetTarget()->GetTransform()->SetLocalRot(tData[2]);
}



void CTransformDlg::OnChangedPivot()
{
	int idx = m_Pivot.GetCurSel();
	GetTarget()->GetTransform()->SetPivot(m_tPivotArr[idx].x, m_tPivotArr[idx].y);
}

#include <Engine/MeshRender.h>

void CTransformDlg::OnClicked2DScaleBtn()
{
	// 편의용. 누르면, 2D 이미지에 맞춰서 scale 잡아준다.

	CMeshRender* pMeshRender = GetTarget()->GetMeshRender();
	if (pMeshRender)
	{
		for (size_t i = 0; i < 4; ++i)
		{
			Ptr<CTexture> pTex = pMeshRender->GetSharedMaterial()->GetTexture(i);
			if (pTex != nullptr)
			{
				GetTarget()->GetTransform()->SetLocalScale(Vec3(pTex->GetWidth(), pTex->GetHeight(), 1.f));
				break;
			}
		}
			
	}
}
