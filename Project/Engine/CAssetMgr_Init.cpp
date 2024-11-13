#include "pch.h"
#include "CAssetMgr.h"
#include "CDevice.h"

void CAssetMgr::Init()
{
	CreateEngineMesh();
	CreateEngineTexture();
	CreateEngineGraphicShader();
	CreateEngineComputeShader();
	CreateEngineMaterial();
	CreateEngineSprite();
}

void CAssetMgr::CreateEngineMesh()
{
	Ptr<CMesh> pMesh = nullptr;

	// =========
	// PointMesh
	// =========
	Vertex v;
	UINT i = 0;

	pMesh = new CMesh(true);
	pMesh->Create(&v, 1, &i, 1);
	CAssetMgr::GetInst()->AddAsset(L"PointMesh", pMesh);


	// =========
	// Rect Mesh
	// 4 개의 정점의 위치 설계
	// 0 -- 1
	// | \  |
	// 3 -- 2   
	// =========	
	Vertex arrVtx[4] = {};
	arrVtx[0].vPos = Vector3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vUV = Vector2(0.f, 0.f);
	arrVtx[0].vColor = Vector4(1.f, 0.f, 0.f, 1.f);

	arrVtx[1].vPos = Vector3(0.5f, 0.5f, 0.f);
	arrVtx[1].vUV = Vector2(1.f, 0.f);
	arrVtx[1].vColor = Vector4(0.f, 0.f, 1.f, 1.f);

	arrVtx[2].vPos = Vector3(0.5f, -0.5f, 0.f);
	arrVtx[2].vUV = Vector2(1.f, 1.f);
	arrVtx[2].vColor = Vector4(0.f, 1.f, 0.f, 1.f);

	arrVtx[3].vPos = Vector3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vUV = Vector2(0.f, 1.f);
	arrVtx[3].vColor = Vector4(1.f, 0.f, 1.f, 1.f);

	// 인덱스
	UINT Idx[6] = { 0 , 1 , 2, 0 , 2 , 3 };

	// 클래스 or 구조체 + 가상함수
	const type_info& info = typeid(CMesh);
	const char* pName = info.name();

	pMesh = new CMesh(true);
	pMesh->Create(arrVtx, 4, Idx, 6);
	CAssetMgr::GetInst()->AddAsset(L"RectMesh", pMesh);

	// ==============
	// RectMesh_Debug
	// 4 개의 정점의 위치 설계
	// 0 -- 1
	// | \  |
	// 3 -- 2   
	// ==============
	UINT DebugRectIdx[5] = { 0 , 1 , 2, 3 , 0 };

	pMesh = new CMesh(true);
	pMesh->Create(arrVtx, 4, DebugRectIdx, 5);
	CAssetMgr::GetInst()->AddAsset(L"RectMesh_Debug", pMesh);

	// ==============
	// LineMesh
	// 2 개의 정점의 위치 설계
	// 0 -- 1 
	// ==============
	arrVtx[0].vPos = Vector3(0.f, 0.f, 0.f);
	arrVtx[0].vUV = Vector2(0.f, 0.f);
	arrVtx[0].vColor = Vector4(1.f, 1.f, 1.f, 1.f);

	arrVtx[1].vPos = Vector3(1.f, 0.f, 0.f);
	arrVtx[1].vUV = Vector2(1.f, 0.f);
	arrVtx[1].vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	UINT DebugLineIdx[2] = { 0 , 1 };

	pMesh = new CMesh(true);
	pMesh->Create(arrVtx, 2, DebugLineIdx, 2);
	CAssetMgr::GetInst()->AddAsset(L"LineMesh", pMesh);


	// ==========
	// CircleMesh
	// ==========
	vector<Vertex>		vecVtx;
	vector<UINT>	vecIdx;

	UINT Slice = 40;
	float Radius = 0.5f;
	float Theta = 0.f;

	// 중심점
	v.vPos = Vector3(0.f, 0.f, 0.f);
	v.vUV = Vector2(0.5f, 0.5f);
	vecVtx.push_back(v);

	for (UINT i = 0; i < Slice + 1; ++i)
	{
		v.vPos = Vector3(Radius * cosf(Theta), Radius * sinf(Theta), 0.f);
		v.vUV = Vector2(v.vPos.x + 0.5f, 1.f - (v.vPos.y + 0.5f));
		vecVtx.push_back(v);

		Theta += (XM_PI * 2.f) / Slice;
	}

	for (UINT i = 0; i < Slice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 1);
		vecIdx.push_back(i + 2);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	CAssetMgr::GetInst()->AddAsset(L"CircleMesh", pMesh);

	// =================
	// CircleMesh_Debug
	// =================	
	vecIdx.clear();
	for (size_t i = 0; i < vecVtx.size() - 1; ++i)
	{
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	CAssetMgr::GetInst()->AddAsset(L"CircleMesh_Debug", pMesh);
}

void CAssetMgr::CreateEngineTexture()
{
	// NoiseTexture
	Load<CTexture>(L"NoiseTex", L"Texture//noise//noise_03.jpg");

	Load<CTexture>(L"SkillHit", L"Texture//maple//skill_hit.png");
	Load<CTexture>(L"DamageSkin", L"Texture//maple//number//0.png");
	Load<CTexture>(L"DebugRectTex", L"Texture\\DebugRectTex.png");

	// PostPorcessTexture
	Vector2 vResolution = CDevice::GetInst()->GetRenderResolution();
	CreateTexture(L"PostProcessTex", vResolution.x, vResolution.y
		, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
}


void CAssetMgr::CreateEngineGraphicShader()
{
	Ptr<CGraphicShader> pShader = nullptr;

	// ===========================
	// Std2DShader : 표준 2D 쉐이더
	// ===========================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddScalarParam("Test Parameter", VEC2_2, true);
	pShader->AddTexParam("Output Texture", TEX_0);

	CAssetMgr::GetInst()->AddAsset(L"Std2DShader", pShader);


	// ================================================
	// Std2DAlphaBlendShader : 표준 알파블렌드 2D 쉐이더
	// ================================================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D_AlphaBlend");
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	pShader->SetDSState(DS_TYPE::NO_TEST);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	CAssetMgr::GetInst()->AddAsset(L"Std2DAlphaBlendShader", pShader);


	// =================================
	// Std2D_PaperBurn : PaperBurn 쉐이더
	// =================================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D_PaperBurn");

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	CAssetMgr::GetInst()->AddAsset(L"Std2DPaperBurnShader", pShader);


	// ==================================
	// TileMapShader : 타일맵 전용 쉐이더
	// ==================================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(L"Shader\\tilemap.fx", "PS_TileMap");

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::LESS);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);
	CAssetMgr::GetInst()->AddAsset(L"TileMapShader", pShader);

	// ===========
	// PostProcess
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Post");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_Post");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"PostProcessShader", pShader);



	// ===========
	// Distortion
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Distortion");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_Distortion");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"DistortionShader", pShader);



	// ===========
	// Vortex
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Vortex");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_Vortex");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"VortexShader", pShader);


	// ===========
	// Enlarge
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Enlarge");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_Enlarge");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"EnlargeShader", pShader);

	// ===========
	// Wave
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Wave");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_Wave");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"WaveShader", pShader);


	// =================================
	// DebugShapeShader : 디버그용 쉐이더
	// =================================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\debug.fx", "VS_DebugShape");
	pShader->CreatePixelShader(L"Shader\\debug.fx", "PS_DebugShape");
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	CAssetMgr::GetInst()->AddAsset(L"DebugShapeShader", pShader);


	// ==============
	// ParticleShader
	// ==============
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\particle.fx", "VS_Particle");
	pShader->CreateGeometryShader(L"Shader\\particle.fx", "GS_Particle");
	pShader->CreatePixelShader(L"Shader\\particle.fx", "PS_Particle");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	pShader->SetDSState(DS_TYPE::NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_PARTICLE);
	CAssetMgr::GetInst()->AddAsset(L"ParticleShader", pShader);
}

#include "CParticleTickCS.h"

void CAssetMgr::CreateEngineComputeShader()
{
	AddAsset<CComputeShader>(L"ParticleTickCS", new CParticleTickCS);
}

void CAssetMgr::CreateEngineMaterial()
{
	Ptr<CMaterial> pMtrl = nullptr;

	// Std2DMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"Std2DMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// Std2DAlphaBlendMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"Std2DAlphaBlendMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DAlphaBlendShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// Std2DPaperBurnMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"Std2DPaperBurnMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DPaperBurnShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// TileMapMaterial
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"TileMapMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"TileMapShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// PostProcessMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"PostProcessMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"PostProcessShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);


	// DistortionMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"DistortionMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DistortionShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);


	// VortexMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"VortexMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"VortexShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);


	// EnlargeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"EnlargeMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"EnlargeShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// WaveMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"WaveMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"WaveShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);


	//DebugShapeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"DebugShapeMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DebugShapeShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);


	//ParticleMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"ParticleMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"ParticleShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);
}

void CAssetMgr::CreateEngineSprite()
{
	// 링크 애니메이션 생성
	Ptr<CTexture> pLinkTex = Load<CTexture>(L"LinkAtlas", L"Texture\\link.png");

	Ptr<CFlipbook> pFlipbook = new CFlipbook;
	Ptr<CSprite> pSprite = nullptr;

	for (int i = 0; i < 10; ++i)
	{
		pSprite = new CSprite;
		pSprite->SetAtlasTexture(pLinkTex);
		pSprite->SetLeftTop(Vector2(i * 120.f, 520.f));
		pSprite->SetSlice(Vector2(120.f, 130.f));
		pSprite->SetBackground(Vector2(200.f, 200.f));

		if (i == 3)
			pSprite->SetOffset(Vector2(10.f, 0.f));

		wchar_t szKey[255] = {};
		swprintf_s(szKey, 255, L"LinkWalkDown_%d", i);

		// Sprite 등록
		AddAsset(szKey, pSprite);

		// flipbook 에 Sprite 추가
		pFlipbook->AddSprite(pSprite);
	}

	// flipbook 등록
	AddAsset(L"LinkWalkDown", pFlipbook);

	// 메이플 이미지 로드
	Ptr<CTexture> pStand = Load<CTexture>(L"StandAtlas", L"Texture//maple//avatar_stand1_default.png");
	Ptr<CTexture> pWalk = Load<CTexture>(L"WalkAtlas", L"Texture//maple//avatar_walk1_default.png");
	Ptr<CTexture> pProne = Load<CTexture>(L"ProneAtlas", L"Texture//maple//avatar_prone(0)_default(0).png");
	Ptr<CTexture> pJump = Load<CTexture>(L"JumpAtlas", L"Texture//maple//avatar_jump(0)_default(0).png");

	// Stand 애니메이션 생성
	pFlipbook = new CFlipbook;

	for (int i = 0; i < 3; ++i)
	{
		pSprite = new CSprite;
		pSprite->SetAtlasTexture(pStand);
		pSprite->SetLeftTop(Vector2(i * 54.f, 0.f));
		pSprite->SetSlice(Vector2(54.f, 72.f));
		pSprite->SetBackground(Vector2(200.f, 200.f));

		wchar_t szKey[255] = {};
		swprintf_s(szKey, 255, L"StandAnim_%d", i);

		// Sprite 등록
		AddAsset(szKey, pSprite);

		// flipbook 에 Sprite 추가
		pFlipbook->AddSprite(pSprite);
	}

	// flipbook 등록
	AddAsset(L"StandAnim", pFlipbook);

	// Walk 애니메이션 생성
	pFlipbook = new CFlipbook;

	for (int i = 0; i < 3; ++i)
	{
		pSprite = new CSprite;
		pSprite->SetAtlasTexture(pWalk);
		pSprite->SetLeftTop(Vector2(i * 57.f, 0.f));
		pSprite->SetSlice(Vector2(57.f, 74.f));
		pSprite->SetBackground(Vector2(200.f, 200.f));

		wchar_t szKey[255] = {};
		swprintf_s(szKey, 255, L"WalkAnim_%d", i);

		// Sprite 등록
		AddAsset(szKey, pSprite);

		// flipbook 에 Sprite 추가
		pFlipbook->AddSprite(pSprite);
	}

	// Walk 등록
	AddAsset(L"WalkAnim", pFlipbook);

	// flipbook 애니메이션 생성
	pFlipbook = new CFlipbook;

	for (int i = 0; i < 1; ++i)
	{
		// Sprite 생성
		pSprite = new CSprite;
		pSprite->SetAtlasTexture(pJump);
		pSprite->SetLeftTop(Vector2(i * 54.f, 0.f));
		pSprite->SetSlice(Vector2(54.f, 70.f));
		pSprite->SetBackground(Vector2(200.f, 200.f));

		wchar_t szKey[255] = {};
		swprintf_s(szKey, 255, L"JumpAnim_%d", i);

		// Sprite 등록
		AddAsset(szKey, pSprite);

		// flipbook 에 Sprite 추가
		pFlipbook->AddSprite(pSprite);
	}

	// flipbook 등록
	AddAsset(L"JumpAnim", pFlipbook);

	// Prone 애니메이션 생성
	pFlipbook = new CFlipbook;

	for (int i = 0; i < 1; ++i)
	{
		pSprite = new CSprite;
		pSprite->SetAtlasTexture(pProne);
		pSprite->SetLeftTop(Vector2(i * 78.f, 0.f));
		pSprite->SetSlice(Vector2(78.f, 47.f));
		pSprite->SetBackground(Vector2(200.f, 200.f));

		wchar_t szKey[255] = {};
		swprintf_s(szKey, 255, L"ProneAnim_%d", i);

		// Sprite 등록
		AddAsset(szKey, pSprite);

		// flipbook 에 Sprite 추가
		pFlipbook->AddSprite(pSprite);
	}

	// flipbook 등록
	AddAsset(L"ProneAnim", pFlipbook);

	// Prone 애니메이션 생성
	pFlipbook = new CFlipbook;

	for (int i = 0; i < 1; ++i)
	{
		pSprite = new CSprite;
		pSprite->SetAtlasTexture(pProne);
		pSprite->SetLeftTop(Vector2(i * 78.f, 0.f));
		pSprite->SetSlice(Vector2(78.f, 47.f));
		pSprite->SetBackground(Vector2(200.f, 200.f));

		wchar_t szKey[255] = {};
		swprintf_s(szKey, 255, L"ProneAnim_%d", i);

		// Sprite 등록
		AddAsset(szKey, pSprite);

		// flipbook 에 Sprite 추가
		pFlipbook->AddSprite(pSprite);
	}

	// flipbook 등록
	AddAsset(L"ProneAnim", pFlipbook);


	// lucid 애니메이션 생성
	Ptr<CTexture> pLucid = Load<CTexture>(L"LucidAtlas", L"Texture//maple//RushLucidAtlas.png");
	pFlipbook = new CFlipbook;


	for (int i = 0; i < 8; ++i)
	{
		pSprite = new CSprite;
		pSprite->SetAtlasTexture(pLucid);
		pSprite->SetLeftTop(Vector2(i * 414.f, 0.f));
		pSprite->SetSlice(Vector2(414.f, 464.f));
		pSprite->SetBackground(Vector2(1000.f, 1000.f));

		wchar_t szKey[255] = {};
		swprintf_s(szKey, 255, L"LucidAnim_%d", i);

		// Sprite 등록
		AddAsset(szKey, pSprite);

		// flipbook 에 Sprite 추가
		pFlipbook->AddSprite(pSprite);
	}

	// flipbook 등록
	AddAsset(L"LucidAnim", pFlipbook);
}
