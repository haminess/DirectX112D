#include "pch.h"
#include "TestLevel.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>

#include <Engine/CCollisionMgr.h>

#include <Scripts/CPlayerScript.h>
#include <Scripts/CMissileScript.h>
#include <Scripts/CCameraScript.h>
#include <Scripts/CPlatformScript.h>
#include <Scripts/CCollisionReactScript.h>
#include <Scripts/CParticleSystemScript.h>
#include <Scripts/CMonsterScript.h>
#include <Scripts/CMouseScript.h>
#include <Engine/CClearColorCS.h>
#include <Engine/CStructuredBuffer.h>


void TestLevel::CreateTestLevel()
{
	// Texture 로딩하기
	Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(L"PlayerTex", L"Texture//Character.png");
	Ptr<CTexture> pNoiseTex = CAssetMgr::GetInst()->Load<CTexture>(L"NoiseTex", L"Texture//noise//noise_03.jpg");
	Ptr<CTexture> pAtlasTex = CAssetMgr::GetInst()->Load<CTexture>(L"TileMapTex", L"Texture//TILE.bmp");
	Ptr<CTexture> pSpaceTex = CAssetMgr::GetInst()->Load<CTexture>(L"SpaceTex", L"Texture//space.jpeg");
	Ptr<CTexture> pBeachTex = CAssetMgr::GetInst()->Load<CTexture>(L"BeachTex", L"Texture//beach.jpeg");
	Ptr<CTexture> pWaterTex = CAssetMgr::GetInst()->Load<CTexture>(L"WaterTex", L"Texture//water.jpeg");
	Ptr<CTexture> pSnailTex = CAssetMgr::GetInst()->Load<CTexture>(L"SnailTex", L"Texture//maple//stand.png");

	// Texture 생성하기
	Ptr<CTexture> pTestTex =
		CAssetMgr::GetInst()->CreateTexture(L"TestTexture", 1055, 1055
			, DXGI_FORMAT_R8G8B8A8_UNORM
			, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);

	// ComputeShader 실행하기
	Ptr<CClearColorCS> pCS = new CClearColorCS;
	pCS->SetTargetTexture(pTestTex);
	pCS->SetClearColor(Vector4(1.f, 1.f, 1.f, 1.f));
	pCS->Execute();

	// 구조화버퍼 테스트
	CStructuredBuffer* pSB = new CStructuredBuffer;
	pSB->Create(16, 1, SB_TYPE::SRV_UAV, true);

	struct Test
	{
		int a, b, c, d;
	} t1;

	t1.a = 10;
	t1.b = 11;
	t1.c = 100;
	t1.d = 1111;

	pSB->SetData(&t1);

	Test t2 = {};

	pSB->GetData(&t2);

	delete pSB;

	// 레벨 생성
	CLevel* pLevel = new CLevel;

	// 테스트 레벨을 현재 레벨로 지정
	ChangeLevel(pLevel, LEVEL_STATE::STOP);

	pLevel->GetLayer(0)->SetName(L"Background");
	pLevel->GetLayer(1)->SetName(L"Tile");
	pLevel->GetLayer(2)->SetName(L"Default");
	pLevel->GetLayer(3)->SetName(L"Player");
	pLevel->GetLayer(4)->SetName(L"PlayerObject");
	pLevel->GetLayer(5)->SetName(L"Monster");
	pLevel->GetLayer(6)->SetName(L"MonsterObject");

	// 레벨 오브젝트 생성
	CGameObject* pObject = nullptr;

	// MainCamera
	pObject = new CGameObject;
	pObject->SetName(L"MainCamera");
	pObject->AddComponent(new CCamera);
	pObject->AddComponent(new CCameraScript);

	// MainCamera 로 설정
	pObject->Camera()->SetPriority(0);

	// 모든 레이어를 찍도록 설정
	pObject->Camera()->LayerCheckAll();

	// 카메라 투영방식 설정
	pObject->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

	pLevel->AddObject(0, pObject, false);
	//CreateObject(pObject, 0, false);


	// =================
	// 광원 오브젝트 추가
	// =================
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Directional Light");
	pLightObj->AddComponent(new CLight2D);

	pLightObj->Transform()->SetRelativePos(-100.f, 0.f, 10.f);
	pLightObj->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light2D()->SetLightColor(Vector3(1.f, 1.f, 1.f));
	pLightObj->Light2D()->SetRadius(200.f);

	CreateObject(pLightObj, 0, false);





	//// TileMap
	//CGameObject* pTileMap = new CGameObject;
	//pTileMap->AddComponent(new CTileMap);
	//pTileMap->TileMap()->SetAtlasTexture(pAtlasTex);
	//pTileMap->TileMap()->SetTileAtlasSize(64, 64);
	//pTileMap->TileMap()->SetTileSize(64.f, 64.f);
	//pTileMap->TileMap()->SetTileColRow(8, 6);
	//CreateObject(pTileMap, 1, false);

	//// TextureObject
	//pObject = new CGameObject;
	//pObject->SetName(L"Texture");
	//pObject->AddComponent(new CMeshRender);
	//pObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	//pObject->Transform()->SetRelativePos(Vector3(0.f, 0.f, 0.f));
	//pObject->Transform()->SetRelativeScale(3000.f , 1920.f , 1.f);
	//pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pWaterTex);
	//pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_1, pNoiseTex);
	//m_CurLevel->AddObject(0, pObject, false);



	//// PostProcess
	//CGameObject* pMouse = new CGameObject;
	//pMouse->SetName(L"MousePostProcess");
	//pMouse->AddComponent(new CMeshRender);
	//pMouse->AddComponent(new CCollider2D);
	//pMouse->AddComponent(new CMouseScript);

	//pMouse->Transform()->SetRelativePos(Vector3(0.f, 0.f, 0.f));
	//pMouse->Transform()->SetRelativeScale(Vector3(300.f, 300.f, 1.f));

	//pMouse->Collider2D()->SetIndependentScale(false);
	//pMouse->Collider2D()->SetScale(Vector2(1.f, 1.f));

	//pMouse->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));
	//pMouse->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"WaveMtrl"));

	//pMouse->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));
	////pMouse->MeshRender()->GetMaterial()->SetTexParam(TEX_1, pNoiseTex);
	//pMouse->MeshRender()->GetMaterial()->SetScalarParam(VEC2_2, Vector2(360.f, 220.f));
	//pMouse->MeshRender()->GetMaterial()->SetScalarParam(VEC2_3, Vector2(-680.f, 260.f));
	//pMouse->MeshRender()->GetMaterial()->SetScalarParam(VEC2_2, Vector2(-285.f, -250.f));
	//pMouse->MeshRender()->GetMaterial()->SetScalarParam(VEC2_3, Vector2(840.f, -160.f));
	//pLevel->AddObject(2, pMouse, false);


	//// PostProcess2
	//pPostProcess = new CGameObject;
	//pPostProcess->SetName(L"PostProcess");
	//pPostProcess->AddComponent(new CMeshRender);
	//pPostProcess->AddComponent(new CMouseScript);

	//pPostProcess->Transform()->SetRelativePos(Vector3(0.f, 0.f, 0.f));
	//pPostProcess->Transform()->SetRelativeScale(Vector3(3000.f, 1920.f, 1.f));

	//pPostProcess->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pPostProcess->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DistortionMtrl"));

	//pPostProcess->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));
	//pPostProcess->MeshRender()->GetMaterial()->SetTexParam(TEX_1, pNoiseTex);
	//m_CurLevel->AddObject(2, pPostProcess, false);



	// Player
	CGameObject* pPlayer = new CGameObject;
	pPlayer->SetName(L"Player");
	pPlayer->AddComponent(new CCollisionReactScript);
	pPlayer->AddComponent(new CPlayerScript);
	pPlayer->AddComponent(new CCollider2D);
	pPlayer->AddComponent(new CRigidBody2D);
	pPlayer->Collider2D()->SetCollidable(true);



	pPlayer->Transform()->SetRelativePos(0.f, 0.f, 0.f);
	pPlayer->Transform()->SetRelativeScale(100.f, 100.f, 1.f);

	pPlayer->Collider2D()->SetIndependentScale(true);
	pPlayer->Collider2D()->SetOffset(Vector2(0.f, 2.5f));
	pPlayer->Collider2D()->SetScale(Vector2(5.f, 5.f));


	pPlayer->RigidBody2D()->UseGravity(true);
	pPlayer->RigidBody2D()->SetGravityAccel(2300.f);
	pPlayer->RigidBody2D()->SetFriction(1500.f);

	pObject = new CGameObject;
	pObject->SetName(L"Model");
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CFlipbookPlayer);

	pObject->Transform()->UseIndependentScale(true);
	pObject->Transform()->SetRelativePos(Vector3(0.f, 30.f, 100.f));
	pObject->Transform()->SetRelativeScale(200.f, 200.f, 1.f);
	pObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_1, pNoiseTex);
	pObject->MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, 0.0f);


	// 애니메이션 적용
	pObject->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"LinkWalkDown"));
	pObject->FlipbookPlayer()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"StandAnim"));
	pObject->FlipbookPlayer()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"WalkAnim"));
	pObject->FlipbookPlayer()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"JumpAnim"));
	pObject->FlipbookPlayer()->AddFlipbook(4, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"ProneAnim"));
	pObject->FlipbookPlayer()->Play(2, 3.f, true);
	pPlayer->AddChild(pObject);
	

	// Link Object
	CGameObject* pLink = new CGameObject;
	pLink->SetName(L"Link");
	pLink->AddComponent(new CMeshRender);
	pLink->AddComponent(new CFlipbookPlayer);
	pLink->AddComponent(new CCollider2D);

	pLink->Transform()->SetRelativePos(0.f, 0.f, 10.f);
	pLink->Transform()->SetRelativeScale(100.f, 100.f, 1.f);

	pLink->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	Ptr<CMaterial> pMtlr = CAssetMgr::GetInst()->Load<CMaterial>(L"Material\\Default Material 0.mtrl", L"Material\\Default Material 0.mtrl");
	pLink->MeshRender()->SetMaterial(pMtlr);
	pLink->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pTex);

	pLink->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->Load<CFlipbook>(L"LinkWalkDown", L"Flipbook\\LinkWalkDown.flip"));
	//pLink->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->Load<CFlipbook>(L"Flipbook\\LinkWalkDown.flip", L"Flipbook\\LinkWalkDown.flip"));
	pLink->FlipbookPlayer()->Play(0, 10, true);

	pLevel->AddObject(0, pLink, false);

	// Child
	CGameObject* pChild = new CGameObject;
	pChild->SetName(L"Child");
	pChild->AddComponent(new CMeshRender);
	pChild->AddComponent(new CFlipbookPlayer);
	pChild->AddComponent(new CCollider2D);

	pChild->Transform()->UseIndependentScale(false);
	pChild->Transform()->SetRelativePos(2.f, 0.f, 0.f);
	pChild->Transform()->SetRelativeScale(1000.f, 1000.f, 1.f);

	pChild->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pChild->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	pChild->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pTestTex);

	pChild->Collider2D()->SetIndependentScale(true);
	pChild->Collider2D()->SetScale(Vector2(5.f, 5.f));

	pChild->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"LucidAnim"));
	pChild->FlipbookPlayer()->Play(0, 4, true);


	// 계층관계 설정
	//pPlayer->AddChild(pChild);
	CreateObject(pChild, 1, false);
	CreateObject(pPlayer, 0, false);




	// Plaform
	pObject = new CGameObject;
	pObject->AddComponent(new CPlatformScript);
	pObject->GetScript<CPlatformScript>()->SetPlatformPos(Vector3(-600.f, 500.f, 0.f), Vector3(-500.f, 500.f, 0.f), 600.f);
	CreateObject(pObject, 1, false);

	pObject = new CGameObject;
	pObject->AddComponent(new CPlatformScript);
	pObject->GetScript<CPlatformScript>()->SetPlatformPos(Vector3(-500.f, -100.f, 0.f), Vector3(-100.f, -100.f, 0.f), 50.f);
	CreateObject(pObject, 1, false);

	//pObject = new CGameObject;
	//pObject->AddComponent(new CPlatformScript);
	//pObject->GetScript<CPlatformScript>()->SetPlatformPos(Vector3(-100.f, -100.f, 0.f), Vector3(0.f, 0.f, 0.f), 50.f);
	//CreateObject(pObject, 1, false);

	//pObject = new CGameObject;
	//pObject->AddComponent(new CPlatformScript);
	//pObject->GetScript<CPlatformScript>()->SetPlatformPos(Vector3(0.f, 0.f, 0.f), Vector3(100.f, 0.f, 0.f), 50.f);
	//CreateObject(pObject, 1, false);

	//pObject = new CGameObject;
	//pObject->AddComponent(new CPlatformScript);
	//pObject->GetScript<CPlatformScript>()->SetPlatformPos(Vector3(100.f, 0.f, 0.f), Vector3(200.f, -100.f, 0.f), 50.f);
	//CreateObject(pObject, 1, false);

	//pObject = new CGameObject;
	//pObject->AddComponent(new CPlatformScript);
	//pObject->GetScript<CPlatformScript>()->SetPlatformPos(Vector3(200.f, 500.f, 0.f), Vector3(300.f, 500.f, 0.f), 600.f);
	//CreateObject(pObject, 1, false);



	//// Monster
	//CGameObject* pMonster = new CGameObject;
	//pMonster->SetName(L"Monster");
	//pMonster->AddComponent(new CMeshRender);
	//pMonster->AddComponent(new CCollider2D);
	//pMonster->AddComponent(new CMonsterScript);

	//pMonster->Transform()->SetRelativePos(Vector3(400.f, 0.f, 100.f));
	//pMonster->Transform()->SetRelativeScale(Vector3(100.f, 100.f, 1.f));

	//pMonster->Collider2D()->SetIndependentScale(true);
	//pMonster->Collider2D()->SetScale(Vector2(100.f, 100.f));

	//pMonster->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pMonster->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	//pMonster->MeshRender()->CreateDynamicMaterial();
	//pMonster->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pSnailTex);


	//CreateObject(pMonster, 1, false);

	//pMonster = new CGameObject;
	//pMonster->SetName(L"Monster");
	//pMonster->AddComponent(new CMeshRender);
	//pMonster->AddComponent(new CCollider2D);
	//pMonster->AddComponent(new CMonsterScript);

	//pMonster->Transform()->SetRelativePos(Vector3(-400.f, 200.f, 100.f));
	//pMonster->Transform()->SetRelativeScale(Vector3(100.f, 100.f, 1.f));

	//pMonster->Collider2D()->SetIndependentScale(true);
	//pMonster->Collider2D()->SetScale(Vector2(100.f, 100.f));

	//pMonster->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pMonster->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	//pMonster->MeshRender()->CreateDynamicMaterial();
	//pMonster->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pSnailTex);


	//CreateObject(pMonster, 1, false);

	//pMonster = new CGameObject;
	//pMonster->SetName(L"Monster");
	//pMonster->AddComponent(new CMeshRender);
	//pMonster->AddComponent(new CCollider2D);
	//pMonster->AddComponent(new CMonsterScript);

	//pMonster->Transform()->SetRelativePos(Vector3(0.f, -200.f, 100.f));
	//pMonster->Transform()->SetRelativeScale(Vector3(100.f, 100.f, 1.f));

	//pMonster->Collider2D()->SetIndependentScale(true);
	//pMonster->Collider2D()->SetScale(Vector2(100.f, 100.f));

	//pMonster->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pMonster->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	//pMonster->MeshRender()->CreateDynamicMaterial();
	//pMonster->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pSnailTex);


	//CreateObject(pMonster, 1, false);


	//// Particle Object
	//CGameObject* pParticle = new CGameObject;
	//
	//pParticle->SetName(L"Particle");
	//pParticle->AddComponent(new CParticleSystemScript);

	//CreateObject(pParticle, 2, false);


	CCollisionMgr::GetInst()->CollisionCheck(2, 1);
	CCollisionMgr::GetInst()->CollisionCheck(0, 1);

	// 레벨 상태 변경
	::ChangeLevelState(LEVEL_STATE::PLAY);
}
