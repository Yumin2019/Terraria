// MtrlDataDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "MtrlDataDlg.h"
#include "afxdialogex.h"
#include "resource.h"


#include <Engine/Shader.h>
#include <Engine/PathMgr.h>

// CMtrlDataDlg 대화 상자

IMPLEMENT_DYNAMIC(CMtrlDataDlg, CDialogEx)

CMtrlDataDlg::CMtrlDataDlg() :
	m_pMtrl(nullptr)
{
}

CMtrlDataDlg::~CMtrlDataDlg()
{
}

bool CMtrlDataDlg::Init(Ptr<CMaterial> pMtrl, const wstring& strName)
{
	m_pMtrl = pMtrl;
	SetWindowTextW((strName + L"'s Material").c_str());

	// 이곳에서는 Mtrl로부터, 필요한 데이터를 토대로 EditCtrl에 출력하고, 특정 Ctrl을 가리는 처리를 한다.

	// 모든 Ctrl과 Text에 대해서 일단 보이지 않도록 한다.

	// 메테리얼의 데이터를 레퍼런스로 받아온다.
	const MtrlParam& MtrlData = m_pMtrl->GetMtrlData();

	for (size_t i = 0; i < 4; ++i)
	{
		m_Int[i].ShowWindow(false);
		m_Float[i].ShowWindow(false);
		m_Vec2X[i].ShowWindow(false);
		m_Vec2Y[i].ShowWindow(false);
		m_Vec4X[i].ShowWindow(false);
		m_Vec4Y[i].ShowWindow(false);
		m_Vec4Z[i].ShowWindow(false);
		m_Vec4W[i].ShowWindow(false);
		m_Tex[i].ShowWindow(false);
		m_Int[i].ShowWindow(false);

		// Text
		m_IntText[i].ShowWindow(false);
		m_FloatText[i].ShowWindow(false);
		m_TexText[i].ShowWindow(false);

		for (size_t j = 0; j < 3; ++j)
			m_Vec2Text[i][j].ShowWindow(false);

		for (size_t j = 0; j < 5; ++j)
			m_Vec4Text[i][j].ShowWindow(false);
	}
	//CEdit m_Info; 는 X

	const vector<ShaderParam>& vecParam = m_pMtrl->GetShader()->GetShaderParamVec();
	CString strSet;

	for (size_t i = 0; i < vecParam.size(); ++i)
	{

		switch (vecParam[i].eType)
		{
		case SHADER_PARAM::INT_0:
		case SHADER_PARAM::INT_1:
		case SHADER_PARAM::INT_2:
		case SHADER_PARAM::INT_3:
		{
			// 해당 자리 Int에 대한 처리를 하기 위해 Text & Ctrl을 보여준다.
			m_Int[(UINT)vecParam[i].eType].ShowWindow(true);
			m_IntText[(UINT)vecParam[i].eType].ShowWindow(true);

			// Data Set
			int iData = MtrlData.m_IntArray[(UINT)vecParam[i].eType];
			m_Int[(UINT)vecParam[i].eType].SetWindowTextW(std::to_wstring(iData).c_str());
		}
			
			break;

		case SHADER_PARAM::FLOAT_0:
		case SHADER_PARAM::FLOAT_1:
		case SHADER_PARAM::FLOAT_2:
		case SHADER_PARAM::FLOAT_3:
		{
			m_Float[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::FLOAT_0].ShowWindow(true);
			m_FloatText[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::FLOAT_0].ShowWindow(true);

			float fData = MtrlData.m_FloatArray[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::FLOAT_0];
			m_Float[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::FLOAT_0].SetWindowTextW(std::to_wstring(fData).c_str());
		}
			
			break;

			break;
		case SHADER_PARAM::VEC2_0:
		case SHADER_PARAM::VEC2_1:
		case SHADER_PARAM::VEC2_2:
		case SHADER_PARAM::VEC2_3:
		{
			// Ctrl 활성화.
			m_Vec2X[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC2_0].ShowWindow(true);
			m_Vec2Y[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC2_0].ShowWindow(true);


			// Text
			for (size_t j = 0; j < 3; ++j)
				m_Vec2Text[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC2_0][j].ShowWindow(true);


			// Data Set
			float fDataX = MtrlData.m_Vec2Array[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC2_0].x;
			float fDataY = MtrlData.m_Vec2Array[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC2_0].y;

			m_Vec2X[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC2_0].SetWindowTextW(std::to_wstring(fDataX).c_str());
			m_Vec2Y[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC2_0].SetWindowTextW(std::to_wstring(fDataY).c_str());
		}
			
			break;
		case SHADER_PARAM::VEC4_0:
		case SHADER_PARAM::VEC4_1:
		case SHADER_PARAM::VEC4_2:
		case SHADER_PARAM::VEC4_3:
		{
			m_Vec4X[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0].ShowWindow(true);
			m_Vec4Y[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0].ShowWindow(true);
			m_Vec4Z[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0].ShowWindow(true);
			m_Vec4W[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0].ShowWindow(true);

			// Text
			for (size_t j = 0; j < 5; ++j)
				m_Vec4Text[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0][j].ShowWindow(true);

			float fDataX = MtrlData.m_Vec4Array[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0].x;
			float fDataY = MtrlData.m_Vec4Array[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0].y;
			float fDataZ = MtrlData.m_Vec4Array[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0].z;
			float fDataW = MtrlData.m_Vec4Array[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0].w;

			m_Vec4X[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0].SetWindowTextW(std::to_wstring(fDataX).c_str());
			m_Vec4Y[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0].SetWindowTextW(std::to_wstring(fDataY).c_str());
			m_Vec4Z[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0].SetWindowTextW(std::to_wstring(fDataZ).c_str());
			m_Vec4W[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0].SetWindowTextW(std::to_wstring(fDataW).c_str());
		}

			

			break;
	/*	case SHADER_PARAM::MATRIX_0:
			break;
		case SHADER_PARAM::MATRIX_1:
			break;
		case SHADER_PARAM::MATRIX_2:
			break;
		case SHADER_PARAM::MATRIX_3:
			break;*/
		case SHADER_PARAM::TEX_0:
		case SHADER_PARAM::TEX_1:
		case SHADER_PARAM::TEX_2:
		case SHADER_PARAM::TEX_3:
		{
			m_Tex[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::TEX_0].ShowWindow(true);
			m_TexText[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::TEX_0].ShowWindow(true);

			// Mtrl의 Texture에 셋팅한 이름을 가져온다.
			Ptr<CTexture> pTexture = m_pMtrl->GetTexture((UINT)vecParam[i].eType - (UINT)SHADER_PARAM::TEX_0);

			if (pTexture == nullptr)
			{
				m_Tex[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::TEX_0].SetWindowTextW(L"nullptr");
			}
			else
			{
				m_Tex[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::TEX_0].SetWindowTextW(CPathMgr::GetFileName(pTexture->GetName().c_str()));
			}
		}
			
			break;
		}

		strSet += SHADER_PARAM_NAME[(UINT)vecParam[i].eType];
		strSet += (L" : " + vecParam[i].strParam + L"\r\n").c_str();
	}

	m_Info.SetWindowTextW(strSet);

	return true;
}

int CMtrlDataDlg::IsCollisionTex(const CPoint & point)
{
	// ComboBox의 사각형을 얻어와서, 이 점과 충돌인지 인식하는 함수이다.
	// 충돌이 난 경우에는 충돌이 난 인덱스를 반환하고, 아니라면 -1을 반환하자.
	// 들어온 point는 Screen좌표이다.

	CRect tRC;
	
	for (size_t i = 0; i < 4; ++i)
	{
		::GetWindowRect(m_Tex[i].m_hWnd, &tRC);

		if (tRC.left > point.x || tRC.right < point.x || tRC.top > point.y || tRC.bottom < point.y)
			continue;

		// 충돌
		return i;
	}

	return -1;
}

#include <Engine/ResMgr.h>

void CMtrlDataDlg::CollisionTex(const CString & strTexName, DWORD_PTR dwData, int idx)
{
	if (idx < 0 || idx >= 4)
		assert(false);


	int idxParam = (UINT)SHADER_PARAM::TEX_0 + idx;

	if (strTexName == L"" || dwData == 0)
	{
		// nullptr 값을 넣어준다.

		m_Tex[idx].SetWindowTextW(L"nullptr");
		m_pMtrl->SetData((SHADER_PARAM)idxParam, nullptr);
	}
	else
	{
		// 해당 텍스처를 셋팅한다.
		m_Tex[idx].SetWindowTextW(strTexName);
		Ptr<CTexture> pTex = (CTexture*)dwData;
		m_pMtrl->SetData((SHADER_PARAM)idxParam, &pTex);

	}
}


void CMtrlDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);


	// Edit Ctrl
	int idInt = IDC_EDIT1;
	int idFloat = IDC_EDIT5;
	int idVec2X = IDC_EDIT9;
	int idVec2Y = IDC_EDIT10;
	int idVec4X = IDC_EDIT17;
	int idVec4Y = IDC_EDIT18;
	int idVec4Z = IDC_EDIT19;
	int idVec4W = IDC_EDIT20;
	int idCombo = IDC_EDIT34;

	for (size_t i = 0; i < 4; ++i)
	{
		// int float
		DDX_Control(pDX, idInt + i, m_Int[i]);
		DDX_Control(pDX, idFloat + i, m_Float[i]);

		// vec2 XY
		DDX_Control(pDX, idVec2X + i * 2, m_Vec2X[i]);
		DDX_Control(pDX, idVec2Y + i * 2, m_Vec2Y[i]);

		// vec4 XYZW
		DDX_Control(pDX, idVec4X + i * 4, m_Vec4X[i]);
		DDX_Control(pDX, idVec4Y + i * 4, m_Vec4Y[i]);
		DDX_Control(pDX, idVec4Z + i * 4, m_Vec4Z[i]);
		DDX_Control(pDX, idVec4W + i * 4, m_Vec4W[i]);

		// tex
		DDX_Control(pDX, idCombo + i, m_Tex[i]);

	}

	DDX_Control(pDX, IDC_EDIT33, m_Info);

	// Text Ctrl
	idInt = IDC_TEXT1;
	idFloat = IDC_TEXT5;
	idVec2X = IDC_TEXT9; // x만 이용하자.
	idVec4X = IDC_TEXT21;
	idCombo = IDC_TEXT41;

	for (size_t i = 0; i < 4; ++i)
	{
		// int float
		DDX_Control(pDX, idInt + i, m_IntText[i]);
		DDX_Control(pDX, idFloat + i, m_FloatText[i]);

		// Vec2
		for (size_t j = 0; j < 3; ++j)
		{
			DDX_Control(pDX, idVec2X + i * 3 + j, m_Vec2Text[i][j]);
		}

		// Vec4
		for (size_t j = 0; j < 5; ++j)
		{
			DDX_Control(pDX, idVec4X + i * 5 + j, m_Vec4Text[i][j]);
		}

		// Tex Text
		DDX_Control(pDX, idCombo + i, m_TexText[i]);

		m_Tex[i].SetWindowTextW(L"nullptr");
	}

	
	// 리소스를 드래그 해서 생성하고, 적용하는 식으로 가야 한다.
	// 중복이면 제외처리. 이런식으로 할 것이 아니면... EditCtrl로 가야 한다.
	// 근데 차라리 그게 훨 나은 듯.ㄴ
	// m_Tex[0].SetCurSel(0);
	

}


BEGIN_MESSAGE_MAP(CMtrlDataDlg, CDialogEx)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CMtrlDataDlg 메시지 처리기

void CMtrlDataDlg::OnOK() 
{
	// enter : OK 
	// 적용을 해준다.

	// Ctrl로부터 값을 얻어온다. -> 이것을 Param에 적용한다.

	MtrlParam& MtrlData = m_pMtrl->GetMtrlData();
	const vector<ShaderParam>& vecParam = m_pMtrl->GetShader()->GetShaderParamVec();
	CString strGet;

	// MtrlData를 레퍼런스롤 얻어오고, VecParam의 정보를 토대로 값을 넣어주는 데이터를 알아온다.
	// Enter키를 누르면 이렇게 수정한 값을 넘겨주는 역할을 한다.
	// 텍스처의 경우에는 따로 처리를 하고 있다.

	for (size_t i = 0; i < vecParam.size(); ++i)
	{

		switch (vecParam[i].eType)
		{
		case SHADER_PARAM::INT_0:
		case SHADER_PARAM::INT_1:
		case SHADER_PARAM::INT_2:
		case SHADER_PARAM::INT_3:
		{
			// Data Set
			
			m_Int[(UINT)vecParam[i].eType].GetWindowTextW(strGet);
			MtrlData.m_IntArray[(UINT)vecParam[i].eType] = _wtoi(strGet);
		}

		break;

		case SHADER_PARAM::FLOAT_0:
		case SHADER_PARAM::FLOAT_1:
		case SHADER_PARAM::FLOAT_2:
		case SHADER_PARAM::FLOAT_3:
		{
			m_Float[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::FLOAT_0].GetWindowTextW(strGet);
			MtrlData.m_FloatArray[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::FLOAT_0] = _wtof(strGet);
		}

		break;

		break;
		case SHADER_PARAM::VEC2_0:
		case SHADER_PARAM::VEC2_1:
		case SHADER_PARAM::VEC2_2:
		case SHADER_PARAM::VEC2_3:
		{
			m_Vec2X[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC2_0].GetWindowTextW(strGet);
			MtrlData.m_Vec2Array[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC2_0].x = _wtof(strGet);

			m_Vec2Y[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC2_0].GetWindowTextW(strGet);
			MtrlData.m_Vec2Array[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC2_0].y = _wtof(strGet);

		}

		break;
		case SHADER_PARAM::VEC4_0:
		case SHADER_PARAM::VEC4_1:
		case SHADER_PARAM::VEC4_2:
		case SHADER_PARAM::VEC4_3:
		{
			m_Vec4X[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0].GetWindowTextW(strGet);
			MtrlData.m_Vec4Array[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0].x = _wtof(strGet);

			m_Vec4Y[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0].GetWindowTextW(strGet);
			MtrlData.m_Vec4Array[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0].y = _wtof(strGet);

			m_Vec4Z[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0].GetWindowTextW(strGet);
			MtrlData.m_Vec4Array[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0].z = _wtof(strGet);

			m_Vec4W[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0].GetWindowTextW(strGet);
			MtrlData.m_Vec4Array[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0].w = _wtof(strGet);


		}



		break;
		/*	case SHADER_PARAM::MATRIX_0:
				break;
			case SHADER_PARAM::MATRIX_1:
				break;
			case SHADER_PARAM::MATRIX_2:
				break;
			case SHADER_PARAM::MATRIX_3:
				break;*/
		//case SHADER_PARAM::TEX_0:
		//case SHADER_PARAM::TEX_1:
		//case SHADER_PARAM::TEX_2:
		//case SHADER_PARAM::TEX_3:
		//{
		//	m_Tex[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::TEX_0].ShowWindow(true);
		//	m_TexText[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::TEX_0].ShowWindow(true);

		//	// Mtrl의 Texture에 셋팅한 이름을 가져온다.
		//	Ptr<CTexture> pTexture = m_pMtrl->GetTexture((UINT)vecParam[i].eType - (UINT)SHADER_PARAM::TEX_0);

		//	if (pTexture == nullptr)
		//	{
		//		m_Tex[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::TEX_0].SetWindowTextW(L"nullptr");
		//	}
		//	else
		//	{
		//		m_Tex[(UINT)vecParam[i].eType - (UINT)SHADER_PARAM::TEX_0].SetWindowTextW(pTexture->GetName().c_str());
		//	}
		//}

		//break;
		}

	}

	m_pMtrl->Save(CPathMgr::GetResPath());

}


void CMtrlDataDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);
}
