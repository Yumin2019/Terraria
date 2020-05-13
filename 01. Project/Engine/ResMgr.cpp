#include "stdafx.h"
#include "ResMgr.h"

#include "Material.h"
#include "Mesh.h"
#include "Shader.h"

#include "RenderMgr.h"

DEFINITION_SINGLE(CResMgr);

CResMgr::CResMgr() :
	m_pSoundSys(nullptr),
	m_arrChannel{}
{
}


CResMgr::~CResMgr()
{
	// CloneRes에서 참조하던 Shader를 Release하기 위해서는 Delete를 먼저해서는 안 된다.
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
		Safe_Delete_VecList(m_vecCloneRes[i]);

	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
		Safe_Delete_Map(m_mapRes[i]);

	// Mtrl에 의한 참조가 풀리면서 Texture, Shader의 참조도 0이 되면서 자동으로 소멸.

	// CReousrce* -> CSound* -> Sound* 
	// 사운드의 경우에는 map의 노드들을 삭제하면서 CSound클래스의 소멸자가 호출되고, 여기서 Sound* 멤버를 Release한다.
	
	// Sound System Close
	if (m_pSoundSys)
	{
		m_pSoundSys->close(); 
		m_pSoundSys->release(); // 카운팅 방식을 통해서 더 이상 사용하지 않겠다고 말하는 것.
	}

}

void CResMgr::CreateSound()
{
	/* 
	필요한 Sound를 Load한다. 단순히 경로를 설정하고
	SetOption함수에서 여기서 Load가 이루어진다. 
	Load SetOption이 같이 이루어져야 한다.
	*/

	// 로딩 예시 : Ptr<CSound> pSound = Load<CSound>(L"Title", L"Sound/Title.wav"); pSound->SetOption(true);
	// 사용예시 : Play(L"Title", CHANNEL_TYPE::BGM);

	Ptr<CSound> pSound = nullptr;
	
	pSound = Load<CSound>(L"Dig_0", L"Sound/Dig_0.wav"); pSound->SetOption(false);
	pSound = Load<CSound>(L"Dig_1", L"Sound/Dig_1.wav"); pSound->SetOption(false);
	pSound = Load<CSound>(L"Dig_2", L"Sound/Dig_2.wav"); pSound->SetOption(false);

	pSound = Load<CSound>(L"Door_Closed", L"Sound/Door_Closed.wav"); pSound->SetOption(false);
	pSound = Load<CSound>(L"Door_Opened", L"Sound/Door_Opened.wav"); pSound->SetOption(false);

	pSound = Load<CSound>(L"Grab", L"Sound/Grab.wav"); pSound->SetOption(false);
	pSound = Load<CSound>(L"Grass", L"Sound/Grass.wav"); pSound->SetOption(false);

	pSound = Load<CSound>(L"Menu_Close", L"Sound/Menu_Close.wav"); pSound->SetOption(false);
	pSound = Load<CSound>(L"Menu_Open", L"Sound/Menu_Open.wav"); pSound->SetOption(false);
	pSound = Load<CSound>(L"Menu_Tick", L"Sound/Menu_Tick.wav"); pSound->SetOption(false);

	pSound = Load<CSound>(L"MorningBgm", L"Sound/MorningBgm.wav"); pSound->SetOption(true);
	pSound = Load<CSound>(L"NightBgm", L"Sound/NightBgm.wav"); pSound->SetOption(true);
	pSound = Load<CSound>(L"TitleBgm", L"Sound/TitleBgm.wav"); pSound->SetOption(true);

	pSound = Load<CSound>(L"Player_Hit_0", L"Sound/Player_Hit_0.wav"); pSound->SetOption(false);
	pSound = Load<CSound>(L"Player_Hit_1", L"Sound/Player_Hit_1.wav"); pSound->SetOption(false);
	pSound = Load<CSound>(L"Player_Hit_2", L"Sound/Player_Hit_2.wav"); pSound->SetOption(false);

	pSound = Load<CSound>(L"SwayItem", L"Sound/SwayItem.wav"); pSound->SetOption(false);

	pSound = Load<CSound>(L"Zombie_0", L"Sound/Zombie_0.wav"); pSound->SetOption(false);
	pSound = Load<CSound>(L"Zombie_1", L"Sound/Zombie_1.wav"); pSound->SetOption(false);
	pSound = Load<CSound>(L"Zombie_2", L"Sound/Zombie_2.wav"); pSound->SetOption(false);
	pSound = Load<CSound>(L"Zombie_3", L"Sound/Zombie_3.wav"); pSound->SetOption(false);
	pSound = Load<CSound>(L"Zombie_4", L"Sound/Zombie_4.wav"); pSound->SetOption(false);

	pSound = Load<CSound>(L"Zombie_Hit", L"Sound/Zombie_Hit.wav"); pSound->SetOption(false);
	pSound = Load<CSound>(L"Zombie_Killed", L"Sound/Zombie_Killed.wav"); pSound->SetOption(false);
	pSound = Load<CSound>(L"Slime_Hit", L"Sound/Slime_Hit.wav"); pSound->SetOption(false);
	pSound = Load<CSound>(L"Slime_Killed", L"Sound/Slime_Killed.wav"); pSound->SetOption(false);

}

void CResMgr::CreateDefaultMesh()
{
	vector<Vertex> vecVtx;
	vector<UINT> vecIdx;


	// RectMesh

	Ptr<CMesh> pMesh = new CMesh;


	// =============	// 0 --- 1
	// Rect Mesh		// |  \  |
	//					// 3 --- 2
	// =============	

	Vertex v;
	// 1. 입력 조립기 단계에 전달할, 정점 3개로 구성된 삼각형 1개
	v.tPos = Vec3(-0.5f, 0.5f, 0.f);
	v.tColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.tUVPos = Vec2(0.f, 0.f);
	vecVtx.push_back(v);

	v.tPos = Vec3(0.5f, 0.5f, 0.1f);
	v.tColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.tUVPos = Vec2(1.f, 0.f);


	vecVtx.push_back(v);

	v.tPos = Vec3(0.5f, -0.5f, 0.f);
	v.tColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.tUVPos = Vec2(1.f, 1.f);

	vecVtx.push_back(v);

	v.tPos = Vec3(-0.5f, -0.5f, 0.f);
	v.tColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.tUVPos = Vec2(0.f, 1.f);

	vecVtx.push_back(v);

	vecIdx.push_back(0); vecIdx.push_back(1); vecIdx.push_back(2);
	vecIdx.push_back(0); vecIdx.push_back(2); vecIdx.push_back(3);

	pMesh->Create(&vecVtx[0], sizeof(Vertex), (UINT)vecVtx.size(), D3D11_USAGE_DEFAULT
		, &vecIdx[0], DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), D3D11_USAGE_DEFAULT);

	AddRes(L"RectMesh", pMesh);

	// Collider Rect Mesh // 0 --- 1
						  // |     |
						  // 3 --- 2

	pMesh = new CMesh;

	vecIdx.clear();
	vecIdx.push_back(0); vecIdx.push_back(1);
	vecIdx.push_back(2); vecIdx.push_back(3); 
	vecIdx.push_back(0);

	pMesh->Create(&vecVtx[0], sizeof(Vertex), (UINT)vecVtx.size(), D3D11_USAGE_DEFAULT
		, &vecIdx[0], DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), D3D11_USAGE_DEFAULT, 
		D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);

	AddRes(L"ColRectMesh", pMesh);




	// Circle Mesh

	vecVtx.clear();
	vecIdx.clear();

	pMesh = new CMesh;

	// 원의 중심점을 넣어준다.
	v.tPos = Vec3(0.f, 0.f, 0.f);
	v.tColor = Vec4(0.f, 0.2f, 0.2f, 1.f);
	v.tUVPos = Vec2(0.5f, 0.5f);
	
	vecVtx.push_back(v);

	int iSlideCount = 30;
	float fRadius = 0.5f;
	float fSlideTheta = XM_2PI / iSlideCount;
	// 한 조각의 각도를 구한다.

	float fCurTheta = 0.f;
	for (int i = 0; i < iSlideCount + 1; ++i)
	{
		v.tPos = Vec3(fRadius * cosf(fCurTheta), fRadius * sinf(fCurTheta), 0.f);
		v.tColor = Vec4(1.f, 0.2f, 0.2f, 1.f);
		v.tUVPos = Vec2(0.5f * cosf(fCurTheta), 0.5f * sinf(fCurTheta));

		v.tUVPos.x += 0.5f;
		v.tUVPos.y = (0.5f - v.tUVPos.y);

		vecVtx.push_back(v);

		fCurTheta += fSlideTheta;
	}

	// Count + 1만큼의 인덱스에 접근할 수 있도록 한다.

	for (int i = 0; i < iSlideCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
		// 마지막 i + 2의 경우 인덱스의 범위를 넘어가므로,
		// 위에서 같은 위치의 정점을 하나 더 넣어준 것이다.
	}


	pMesh->Create(&vecVtx[0], sizeof(Vertex), (UINT)vecVtx.size(), D3D11_USAGE_DEFAULT
		, &vecIdx[0], DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), D3D11_USAGE_DEFAULT);

	AddRes(L"CircleMesh", pMesh);


	// Collider Circle Mesh

	// 0 번 중심, 나머지는 원을 도는 정점들..  1 ~ n 

	vecIdx.clear();

	pMesh = new CMesh;
	

	// 1부터 시작하여 SlideCount만큼 (1 + SliceCound 보다 작다)
	// 돌리면, 마지막 점을 잇지 못하게 된다. 따라서, 위에서 시작점을 하나 더 넣어줬으므로,
	// 다음 인덱스는 시작점을 가리키게 된다. 따라서, SlideCount + 2 를 통해서, 인덱스를 하나 더 넣어주고,
	// 원을 그려내는 Line Strip Index를 구한다.

	for (UINT i = 1; i < iSlideCount + 2; ++i)
	{
		vecIdx.push_back(i);
	}

	pMesh->Create(&vecVtx[0], sizeof(Vertex), (UINT)vecVtx.size(), D3D11_USAGE_DEFAULT
		, &vecIdx[0], DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), D3D11_USAGE_DEFAULT,
		D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);

	AddRes(L"ColCircleMesh", pMesh);

}

void CResMgr::CreateDefaultShader()
{
	// Layout정보를 설정한다.
	CShader::CreateLayoutDesc();


	Ptr<CShader>	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Color", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Color", "ps_5_0");

	AddRes(L"ColorShader", pShader);

	// Texture Shader
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Tex", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Tex", "ps_5_0");

	// Blend State 설정
	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));

	pShader->AddShaderParam(ShaderParam(L"Output Texture Idx", SHADER_PARAM::INT_0));
	pShader->AddShaderParam(ShaderParam(L"RGB Divide", SHADER_PARAM::FLOAT_0));
	pShader->AddShaderParam(ShaderParam(L"DayLight Use", SHADER_PARAM::INT_1));
	pShader->AddShaderParam(ShaderParam(L"Texture_0", SHADER_PARAM::TEX_0));
	pShader->AddShaderParam(ShaderParam(L"Texture_1", SHADER_PARAM::TEX_1));
	pShader->AddShaderParam(ShaderParam(L"Texture_2", SHADER_PARAM::TEX_2));
	pShader->AddShaderParam(ShaderParam(L"Texture_3", SHADER_PARAM::TEX_3));

	AddRes(L"TexShader", pShader);

	// Collider 2D Shader
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Collider2D", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Collider2D", "ps_5_0");

	pShader->AddShaderParam(ShaderParam(L"RED : 1 GREEN : 0", SHADER_PARAM::INT_0));

	AddRes(L"Collider2DShader", pShader);



	// Standard 2D Shader
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Standard2D", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Standard2D", "ps_5_0");

	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));
	pShader->AddShaderParam(ShaderParam(L"Output Texture", SHADER_PARAM::TEX_0));
	pShader->AddShaderParam(ShaderParam(L"Texture Reverse", SHADER_PARAM::INT_0));
	pShader->AddShaderParam(ShaderParam(L"Game Light", SHADER_PARAM::FLOAT_0));
	pShader->AddShaderParam(ShaderParam(L"Color Divide Num", SHADER_PARAM::FLOAT_1));

	AddRes(L"Standard2DShader", pShader);

	/*
	상수 버퍼 말고, Material을 통해서 Shader에 전달한 값을 의미한다.
	*/

	// BackGround Shader
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Standard2D", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_BackGround", "ps_5_0");

	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));
	pShader->AddShaderParam(ShaderParam(L"Output Texture", SHADER_PARAM::TEX_0));

	pShader->AddShaderParam(ShaderParam(L"White Weight", SHADER_PARAM::FLOAT_0));
	pShader->AddShaderParam(ShaderParam(L"Color Divide", SHADER_PARAM::FLOAT_1));

	pShader->AddShaderParam(ShaderParam(L"R Weight. r : use, g: value", SHADER_PARAM::VEC2_0));
	pShader->AddShaderParam(ShaderParam(L"G Weight. r : use, g: value", SHADER_PARAM::VEC2_1));
	pShader->AddShaderParam(ShaderParam(L"B Weight. r : use, g: value", SHADER_PARAM::VEC2_2));


	AddRes(L"BackGroundShader", pShader);

	// Tile Shader
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Standard2D", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Tile", "ps_5_0");

	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));
	pShader->AddShaderParam(ShaderParam(L"TILE TEXTURE", SHADER_PARAM::TEX_0));

	pShader->AddShaderParam(ShaderParam(L"TILE X Y SIZE", SHADER_PARAM::VEC2_0));
	pShader->AddShaderParam(ShaderParam(L"TILE IMAGE SIZE (NOT ZERO)", SHADER_PARAM::VEC2_1));
	pShader->AddShaderParam(ShaderParam(L"TILE IMAGE INTERVAL", SHADER_PARAM::VEC2_2));
	pShader->AddShaderParam(ShaderParam(L"TILEMAP TILE SIZE UV (Static)", SHADER_PARAM::VEC2_3));


	AddRes(L"TileShader", pShader);


	// Panel Shader
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Standard2D", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Panel", "ps_5_0");

	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));
	pShader->AddShaderParam(ShaderParam(L"TEXTURE", SHADER_PARAM::TEX_0));

	pShader->AddShaderParam(ShaderParam(L"R Weight : (default 1)", SHADER_PARAM::FLOAT_0));
	pShader->AddShaderParam(ShaderParam(L"G Weight : (default 1)", SHADER_PARAM::FLOAT_1));
	pShader->AddShaderParam(ShaderParam(L"B Weight : (default 1)", SHADER_PARAM::FLOAT_2));
	pShader->AddShaderParam(ShaderParam(L"A Data : (default 1)", SHADER_PARAM::FLOAT_3));

	AddRes(L"PanelShader", pShader);

	// Shining Shader
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Standard2D", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Shining", "ps_5_0");

	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));
	pShader->AddShaderParam(ShaderParam(L"TEXTURE", SHADER_PARAM::TEX_0));

	pShader->AddShaderParam(ShaderParam(L"R Weight : (default 1)", SHADER_PARAM::FLOAT_0));
	pShader->AddShaderParam(ShaderParam(L"G Weight : (default 1)", SHADER_PARAM::FLOAT_1));
	pShader->AddShaderParam(ShaderParam(L"B Weight : (default 1)", SHADER_PARAM::FLOAT_2));
	pShader->AddShaderParam(ShaderParam(L"rgb Divide Data : (default 1)", SHADER_PARAM::FLOAT_3));

	AddRes(L"ShiningShader", pShader);

	// Particle Shader
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Particle", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Particle", "ps_5_0");

	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));
	pShader->AddShaderParam(ShaderParam(L"Particle Texture", SHADER_PARAM::TEX_0));
	pShader->AddShaderParam(ShaderParam(L"Ref Idx X", SHADER_PARAM::INT_0));
	pShader->AddShaderParam(ShaderParam(L"Ref Idx Y", SHADER_PARAM::INT_1));
	pShader->AddShaderParam(ShaderParam(L"Idx Max X", SHADER_PARAM::INT_2));
	pShader->AddShaderParam(ShaderParam(L"Idx Max Y", SHADER_PARAM::INT_3));
	pShader->AddShaderParam(ShaderParam(L"Life Ratio", SHADER_PARAM::FLOAT_0));

	AddRes(L"ParticleShader", pShader);

	// Heart Shader
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Standard2D", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Heart", "ps_5_0");

	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));
	pShader->AddShaderParam(ShaderParam(L"Texture", SHADER_PARAM::TEX_0));
	pShader->AddShaderParam(ShaderParam(L"HP", SHADER_PARAM::INT_0));
	pShader->AddShaderParam(ShaderParam(L"Max HP", SHADER_PARAM::INT_1));

	AddRes(L"HeartShader", pShader);

	// MonsterUI Shader
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Standard2D", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_MonsterUI", "ps_5_0");

	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));
	pShader->AddShaderParam(ShaderParam(L"Texture", SHADER_PARAM::TEX_0));
	pShader->AddShaderParam(ShaderParam(L"HP", SHADER_PARAM::INT_0));
	pShader->AddShaderParam(ShaderParam(L"Max HP", SHADER_PARAM::INT_1));

	AddRes(L"MonsterUIShader", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Standard2D", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Title", "ps_5_0");

	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));
	pShader->AddShaderParam(ShaderParam(L"Texture", SHADER_PARAM::TEX_0));
	pShader->AddShaderParam(ShaderParam(L"RGB Divide", SHADER_PARAM::FLOAT_0));
	pShader->AddShaderParam(ShaderParam(L"Layer Speed Weight(Default : 1.f)", SHADER_PARAM::FLOAT_1));

	AddRes(L"TitleShader", pShader);


}

void CResMgr::CreateDefaultMaterial()
{
	Ptr<CMaterial>	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CShader>(L"ColorShader"));
	pMtrl->DisableFileSave();
	AddRes(L"ColorMtrl", pMtrl);

	int a = 0;
	// 위에서 만든 TexShader를 가지고 있는 TexMaterial을 만든다.
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CShader>(L"TexShader"));
	pMtrl->DisableFileSave();
	pMtrl->SetData(SHADER_PARAM::INT_0, &a);

	AddRes(L"TexMtrl", pMtrl);


	a = 1;
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CShader>(L"Collider2DShader"));
	pMtrl->SetData(SHADER_PARAM::INT_0, &a);
	pMtrl->DisableFileSave();
	AddRes(L"Collider2DMtrl_Red", pMtrl);

	a = 0;
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CShader>(L"Collider2DShader"));
	pMtrl->SetData(SHADER_PARAM::INT_0, &a);
	pMtrl->DisableFileSave();
	AddRes(L"Collider2DMtrl_Green", pMtrl);

	// Standard2D Mtrl
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CShader>(L"Standard2DShader"));
	pMtrl->DisableFileSave();
	AddRes(L"Standard2DMtrl", pMtrl);

	// BackGround Mtrl
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CShader>(L"BackGroundShader"));
	pMtrl->DisableFileSave();
	AddRes(L"BackGroundMtrl", pMtrl);

	// Tile Mtrl
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CShader>(L"TileShader"));
	pMtrl->DisableFileSave();
	AddRes(L"TileMtrl", pMtrl);

	// Panel Mtrl
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CShader>(L"PanelShader"));
	pMtrl->DisableFileSave();
	float fWeight = 1.f;
	pMtrl->SetData(SHADER_PARAM::FLOAT_0, &a);
	pMtrl->SetData(SHADER_PARAM::FLOAT_1, &a);
	pMtrl->SetData(SHADER_PARAM::FLOAT_2, &a);
	pMtrl->SetData(SHADER_PARAM::FLOAT_3, &a);

	AddRes(L"PanelMtrl", pMtrl);

	// Shining Mtrl
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CShader>(L"ShiningShader"));
	pMtrl->DisableFileSave();
	pMtrl->SetData(SHADER_PARAM::FLOAT_0, &a);
	pMtrl->SetData(SHADER_PARAM::FLOAT_1, &a);
	pMtrl->SetData(SHADER_PARAM::FLOAT_2, &a);
	pMtrl->SetData(SHADER_PARAM::FLOAT_3, &a);

	AddRes(L"ShiningMtrl", pMtrl);


	// Particle Mtrl
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CShader>(L"ParticleShader"));
	pMtrl->DisableFileSave();
	AddRes(L"ParticleMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CShader>(L"HeartShader"));
	pMtrl->DisableFileSave();
	AddRes(L"HeartMtrl", pMtrl);


	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CShader>(L"MonsterUIShader"));
	pMtrl->DisableFileSave();
	AddRes(L"MonsterUIMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CShader>(L"TitleShader"));
	pMtrl->DisableFileSave();
	a = 1.f;
	pMtrl->SetData(SHADER_PARAM::FLOAT_1, &a);

	AddRes(L"TitleMtrl", pMtrl);


	// Default Mtrl의 이름을 받아두기로 한다.
	map<wstring, CResource*>::iterator iter;
	map<wstring, CResource*>::iterator iterEnd = m_mapRes[(UINT)RES_TYPE::MATERIAL].end();

	for (iter = m_mapRes[(UINT)RES_TYPE::MATERIAL].begin(); iter != iterEnd; ++iter)
		m_vecDefMtrl.push_back(iter->first);

}

bool CResMgr::Init()
{

	// FMOD 

	// System을 생성한다.
	System_Create(&m_pSoundSys);

	// System을 초기화한다.
	m_pSoundSys->init(FMOD_MAX_CHANNEL_WIDTH, FMOD_INIT_NORMAL, nullptr);

	// 게임에서 사용하는 Sound를 Load한다.
	CreateSound();

	CreateDefaultMesh();

	CreateDefaultShader();

	CreateDefaultMaterial();

	return true;
}

void CResMgr::SoundUpdate()
{
	m_pSoundSys->update();
}

void CResMgr::SetVolume(CHANNEL_TYPE eType, float fVolume)
{
	if (m_arrChannel[(UINT)eType])
	{
		m_arrChannel[(UINT)eType]->setVolume(fVolume); // 0.f ~ 1.f
	}
}

bool CResMgr::IsPlaying(CHANNEL_TYPE eType) const
{
	bool	bPlay = false;

	if (m_arrChannel[(UINT)eType])
	{
		m_arrChannel[(UINT)eType]->isPlaying(&bPlay);
	}

	return bPlay;
}

void CResMgr::Stop(CHANNEL_TYPE eType)
{
	// Stop은 채널단위로 이루어 지는데, 여러 채널을 거쳐서 사운드를 재생하고 있다가도 다 끊어지게 하는 방식이다.

	// 해당 flag로 할당받은 채널이 있다면, 이 처리를 해준다.
	if (m_arrChannel[(UINT)eType])
	{
		// 플레이 중인 경우에만 처리를 해야 한다.
		bool	bPlay = false;
		m_arrChannel[(UINT)eType]->isPlaying(&bPlay);

		// 기본적으로 false를 주고 playing 상태라면 true가 될 것이다.
		// 해당 채널에서 사운드가 재생중이라면, stop함수를 통해서 채널을 꺼버린다.

		if (bPlay)
			m_arrChannel[(UINT)eType]->stop();
	}
}

void CResMgr::Play(const wstring & strName, CHANNEL_TYPE eType, float fVolume)
{
	map<wstring, CResource*>::iterator iter = m_mapRes[(UINT)RES_TYPE::SOUND].find(strName);

	// 해당이름의 Sound가 없다. assert
	if (iter == m_mapRes[(UINT)RES_TYPE::SOUND].end())
		assert(nullptr);

	CSound* pSound = (CSound*)iter->second;

	m_pSoundSys->playSound(pSound->GetSound(), nullptr, false, &m_arrChannel[(UINT)eType]);
	// eType에서 범위를 넘어가는 경우에는 터지게 된다. 따라서, 그것은 체크할 필요가 없고,
	// 제대로 입력을 한 경우에는 해당 채널에서 Play할 것이다.

	m_arrChannel[(UINT)eType]->setVolume(fVolume);

}
