// AddAnimDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "AddAnimDlg.h"
#include "afxdialogex.h"
#include "resource.h"

#include <Engine/PathMgr.h>
#include <Engine/Animation2D.h>


// CAddAnimDlg 대화 상자

IMPLEMENT_DYNAMIC(CAddAnimDlg, CDialogEx)

CAddAnimDlg::CAddAnimDlg(CWnd* pParent) :
	m_pAnimator(nullptr),
	m_pEditAnim(nullptr)
{

}

CAddAnimDlg::~CAddAnimDlg()
{
}

bool CAddAnimDlg::Init(CAnimator2D * pAnimator, const wstring& strName)
{
	if (pAnimator == nullptr)
		assert(false);

	SetWindowTextW((strName + L"'s Animation Create").c_str());

	m_Name.SetWindowTextW(L"");
	m_Tex.SetWindowTextW(L"nullptr");
	m_FrameCnt.SetWindowTextW(L"");
	m_PlayTime.SetWindowTextW(L"");
	m_Option.SetCurSel(0);

	m_Vec2[0][0].SetWindowTextW(L"");
	m_Vec2[0][1].SetWindowTextW(L"");
	m_Vec2[1][0].SetWindowTextW(L"");
	m_Vec2[1][1].SetWindowTextW(L"");

	m_pAnimator = pAnimator;
	m_pEditAnim = nullptr;

	m_HorizonBox.SetCheck(1);


	m_AddBtn.SetWindowTextW(L"ADD");

	return true;
}

bool CAddAnimDlg::InitEdit(CAnimator2D * pAnimator, CAnimation2D * pAnimation)
{
	if (pAnimator == nullptr)
		assert(false);

	else if (pAnimation == nullptr)
		assert(false);

	SetWindowTextW((pAnimation->GetName() + L" Edit").c_str());

	m_pAnimator = pAnimator;
	m_pEditAnim = pAnimation;

	// name
	m_Name.SetWindowTextW(m_pEditAnim->GetName().c_str());

	// texture
	vector<Ani2DFrame>& vecFrame = m_pEditAnim->GetFrameVec();

	if (vecFrame.empty())
	{
		assert(false);
		return false;
	}

	m_pTexture = vecFrame[0].pTexture;

	// 일괄적용을 했다는 가정하에 셋팅한다.
	m_Tex.SetWindowTextW(CPathMgr::GetFileName(vecFrame[0].pTexture->GetName().c_str()));

	// Frame Size
	m_FrameCnt.SetWindowTextW(std::to_wstring(vecFrame.size()).c_str());

	// Play Time (일괄적용)
	m_PlayTime.SetWindowTextW(std::to_wstring(vecFrame.size() * vecFrame[0].fDuration).c_str());

	// Option 애니메이션으로부터 uint값을 얻어와서 이것을 셋팅. (이름순서가 enum문 순서와 동일하게 되어 있음)
	m_Option.SetCurSel((UINT)m_pEditAnim->GetOption());
	
	// 값을 가져오면, 이것은 UV값이다. size / ImageSize 따라서, 이 값에 ImageSize를 곱해서 실제
	// 수치로 적용해준다.

	// LT
	Vec2 tImageSize = Vec2(vecFrame[0].pTexture->GetWidth(), vecFrame[0].pTexture->GetHeight());

	m_Vec2[0][0].SetWindowTextW(std::to_wstring(vecFrame[0].tLeftTop.x * tImageSize.x).c_str());
	m_Vec2[0][1].SetWindowTextW(std::to_wstring(vecFrame[0].tLeftTop.y * tImageSize.y).c_str());

	// Length
	m_Vec2[1][0].SetWindowTextW(std::to_wstring(vecFrame[0].tLength.x * tImageSize.x).c_str());
	m_Vec2[1][1].SetWindowTextW(std::to_wstring(vecFrame[0].tLength.y * tImageSize.y).c_str());

	// 설정된 애니메이션의 위치를 보고, 가로/세로를 구분해보자.
	// 일단 한 장인 경우에는 가로든 세로든 상관이 없다.
	
	if (vecFrame.size() == 1)
	{
		m_HorizonBox.SetCheck(1);
	}
	else
	{
		// 장면이 여러 장이라면, 위치를 토대로 가로인지 세로인지를 판단한다.
		if (vecFrame[0].tLeftTop.x == vecFrame[1].tLeftTop.x)
			m_HorizonBox.SetCheck(0);
		else
			m_HorizonBox.SetCheck(1);

	}

	m_AddBtn.SetWindowTextW(L"EDIT");

	return true;
}

int CAddAnimDlg::IsCollisionTex(const CPoint & point)
{
	// 넘겨준 Tex가 Texture와 충돌했는지 판단하고 인덱스 반환.
	CRect tRC;

	::GetWindowRect(m_Tex.m_hWnd, &tRC);

	if (tRC.left > point.x || tRC.right < point.x || tRC.top > point.y || tRC.bottom < point.y)
		return -1;

	return 0;
}

void CAddAnimDlg::CollisionTex(const CString & strTexName, DWORD_PTR dwData, int idx)
{
	if (idx != 0)
		assert(false);

	if (strTexName == L"" || dwData == 0)
	{
		// nullptr 값을 넣어준다.
		m_Tex.SetWindowTextW(L"nullptr");
	}
	else
	{
		// 해당 텍스처를 셋팅한다.
		m_Tex.SetWindowTextW(strTexName);
		m_pTexture = (CTexture*)dwData;
	}
}



void CAddAnimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Name);
	DDX_Control(pDX, IDC_EDIT2, m_Tex);
	DDX_Control(pDX, IDC_EDIT3, m_FrameCnt);
	DDX_Control(pDX, IDC_EDIT4, m_PlayTime);
	DDX_Control(pDX, IDC_COMBO2, m_Option);
	DDX_Control(pDX, IDC_CHECK1, m_HorizonBox);

	DDX_Control(pDX, IDC_EDIT5, m_Vec2[0][0]);
	DDX_Control(pDX, IDC_EDIT6, m_Vec2[0][1]);
	DDX_Control(pDX, IDC_EDIT7, m_Vec2[1][0]);
	DDX_Control(pDX, IDC_EDIT8, m_Vec2[1][1]);

	m_HorizonBox.SetCheck(1);
	m_Tex.SetWindowTextW(L"nullptr");

	for (size_t i = 0; i < (UINT)ANIMATION_OPTION::AO_END; ++i)
	{
		m_Option.AddString(ANIMATION_OPTION_NAME[i]);
	}

	m_Option.SetCurSel(0);
	DDX_Control(pDX, IDC_BUTTON2, m_AddBtn);
}


BEGIN_MESSAGE_MAP(CAddAnimDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CAddAnimDlg::OnChangedOption)
	ON_BN_CLICKED(IDC_BUTTON2, &CAddAnimDlg::OnBnClickedAnimationAdd)
END_MESSAGE_MAP()


// CAddAnimDlg 메시지 처리기


void CAddAnimDlg::OnChangedOption()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

#include <Engine/GameObject.h>
#include <Engine/Animator2D.h>
#include "ComponentDlg.h"

void CAddAnimDlg::OnBnClickedAnimationAdd()
{
	// 이름을 얻어온다.
	CString strName;
	CString strData;
	m_Name.GetWindowTextW(strName);

	// Debug
	if (strName == L"")
	{
		MessageBox(L"Anim Name", 0, MB_OK);
		return;
	}


	if (m_pTexture == nullptr)
	{
		MessageBox(L"Texture", 0, MB_OK);
		return;
	}

	m_FrameCnt.GetWindowTextW(strData);
	int iFrameCount = _wtoi(strData);

	if (iFrameCount <= 0)
	{
		MessageBox(L"Count", 0, MB_OK);
		return;
	}

	m_PlayTime.GetWindowTextW(strData);
	float fPlayTime = _wtof(strData);
	if (fPlayTime <= 0.f)
	{
		MessageBox(L"Time", 0, MB_OK);
		return;
	}

	// LeftTop을 Pass
	Vec2 tLength;

	// Length X Y
	m_Vec2[1][0].GetWindowTextW(strData);
	tLength.x = _wtof(strData);

	if (tLength.x <= 0.f)
	{
		MessageBox(L"LengthX", 0, MB_OK);
		return;
	}

	m_Vec2[1][1].GetWindowTextW(strData);
	tLength.y = _wtof(strData);

	if (tLength.y <= 0.f)
	{
		MessageBox(L"LengthY", 0, MB_OK);
		return;
	}

	// 애니메이터에 애니메이션을 추가해준다.
	if (!m_pAnimator)
	{
		assert(false);
		return;
	}

	Vec2 tLeftTop;

	m_Vec2[0][0].GetWindowTextW(strData);
	tLeftTop.x = _wtof(strData);

	m_Vec2[0][1].GetWindowTextW(strData);
	tLeftTop.y = _wtof(strData);

	// Option
	ANIMATION_OPTION eOption = (ANIMATION_OPTION)m_Option.GetCurSel();


	if (m_pEditAnim == nullptr)
	{
		// Add 작업인 경우에...
		bool bAdd = m_pAnimator->AddAnimation(strName.GetBuffer(), m_pTexture, tLeftTop, tLength, iFrameCount,
			fPlayTime, eOption, m_HorizonBox.GetCheck());

		if (bAdd)
		{
			// Ctrl 업데이트
			GetDlg()->UpdateCtrl();

			MessageBox(L"애니메이션 추가 완료", 0, MB_OK);
		}
		else
		{
			MessageBox(L"중복키 사용으로 실패", 0, MB_OK);
			// 이미 존재하는 키를 가지고 추가.
		}
	}
	else
	{
		// Edit
		// 참고 : 편집하는 애니메이션은 현재 애니메이션이지 않음. 삭제도 마찬가지.

		// 만약에 이름을 바꾸는 경우가 생긴다면, CtrlWnd에서 얻어오는 이름은 더 이상 키값(찾아오기 위한)이 될 수 없다.
		// 또한, map의 키 값과 Name이 다르다는 것이기에 꼬인다...

		// 결론 : 내부적으로는 없애고, 다시 추가를 한다.

		// 우리가 바꾼 것은 EditCtrl의 이름... Animation의 이름이 아니다.  Animation의 이름은 키값이다. 
		map<wstring, CAnimation2D*>& mapAnim = m_pAnimator->GetAnimations();
		if (m_pAnimator->FindAnimation(m_pEditAnim->GetName()))
		{
			// 이름을 가진 애니메이션이 존재한다. 삭제하고  다음 작업.
			mapAnim.erase(m_pEditAnim->GetName());
		}
		else
		{
			assert(false);
			return;
		}

		m_pAnimator->AddAnimation(strName.GetBuffer(), m_pTexture, tLeftTop, tLength, iFrameCount,
			fPlayTime, eOption, m_HorizonBox.GetCheck());

		GetDlg()->UpdateCtrl();

		MessageBox(L"애니메이션 편집 완료", 0, MB_OK);
	
		m_pEditAnim = nullptr;
	}

}

void CAddAnimDlg::OnOK() {}
