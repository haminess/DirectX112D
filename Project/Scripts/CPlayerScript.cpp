#include "pch.h"
#include "CPlayerScript.h"
#include "CMissileScript.h"
#include "CPlatformScript.h"

#include <Engine/CTaskMgr.h>
#include <Engine/CRigidBody2D.h>


CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_PlayerSpeed(300.f)
	, m_PaperBurnIntence(0.f)
	, m_Platform(nullptr)
{
}

CPlayerScript::~CPlayerScript()
{
}


void CPlayerScript::SaveComponent(FILE* _File)
{
	fwrite(&m_PlayerSpeed, sizeof(float), 1, _File);
	fwrite(&m_PaperBurnIntence, sizeof(float), 1, _File);
}

void CPlayerScript::LoadComponent(FILE* _File)
{
	fread(&m_PlayerSpeed, sizeof(float), 1, _File);
	fread(&m_PaperBurnIntence, sizeof(float), 1, _File);
}



void CPlayerScript::Init()
{
	SERIALIZABLE(CPlayerScript);

	HEADER("[MOVE]");
	PROPERTY(float, m_PlayerSpeed);
	HEADER("[POSITION]");
	PROPERTY(Vector3, m_PlayerPos);
}

void CPlayerScript::Begin()
{
}

void CPlayerScript::Tick()
{
	m_PlayerPos = Transform()->GetWorldPos();

	KEY_STATE Left = CKeyMgr::GetInst()->GetKeyState(KEY::LEFT);
	KEY_STATE Right = CKeyMgr::GetInst()->GetKeyState(KEY::RIGHT);
	KEY_STATE Up = CKeyMgr::GetInst()->GetKeyState(KEY::UP);
	KEY_STATE Down = CKeyMgr::GetInst()->GetKeyState(KEY::DOWN);

	Vector3 vPos = Transform()->GetRelativePos();

	
	// 플레이어 이동 - RigidBody 사용
	if (KEY_TAP(KEY::LEFT))
		RigidBody2D()->SetVelocity(Vector3(-m_PlayerSpeed, 0.f, 0.f));
	if (KEY_TAP(KEY::RIGHT))
		RigidBody2D()->SetVelocity(Vector3(m_PlayerSpeed, 0.f, 0.f));
	if (KEY_TAP(KEY::UP))
		RigidBody2D()->SetVelocity(Vector3(0.f, m_PlayerSpeed, 0.f));
	if (KEY_TAP(KEY::DOWN))
		RigidBody2D()->SetVelocity(Vector3(0.f, -m_PlayerSpeed, 0.f));

	Vector3 vVelo = RigidBody2D()->GetVelocity();
	if (KEY_PRESSED(KEY::LEFT))
		RigidBody2D()->AddVelociy(Vector2(-m_PlayerSpeed, vVelo.y));
	if (KEY_PRESSED(KEY::RIGHT))
		RigidBody2D()->AddVelociy(Vector2(m_PlayerSpeed, vVelo.y));

	// animation
	if (GetOwner()->GetChild()[0]->FlipbookPlayer())
	{
		CGameObject* pModel = GetOwner()->GetChild()[0];
		CFlipbookPlayer* pFP = pModel->FlipbookPlayer();


		if (RigidBody2D()->IsGround())
		{
			// 땅이면 기본
			if (pFP->GetCurFlipbook() == pFP->GetFlipbook(3) &&
				KEY_NONE(KEY::LEFT) && KEY_NONE(KEY::RIGHT))
			{
				pFP->Play(1, 3.f, true);
			}

			// 걷기
			if (KEY_TAP(KEY::LEFT))
			{
				pModel->Transform()->SetRelativeRotation(0.f, 0.f, 0.f);
				pFP->Play(2, 3.f, true);
			}
			if (KEY_TAP(KEY::RIGHT))
			{
				pModel->Transform()->SetRelativeRotation(0.f, Radian(180.f), 0.f);
				pFP->Play(2, 3.f, true);
			}
			// 걷기 종료
			if (KEY_RELEASED(KEY::LEFT) && KEY_NONE(KEY::RIGHT) ||
				KEY_RELEASED(KEY::RIGHT) && KEY_NONE(KEY::LEFT))
				pFP->Play(1, 3.f, true);
			// 눕기
			if (KEY_TAP(KEY::DOWN))
				pFP->Play(4, 3.f, true);
			// 눕기 종료
			if (KEY_RELEASED(KEY::DOWN))
				pFP->Play(1, 3.f, true);
			// 점프
			if (KEY_TAP(KEY::SPACE))
				pFP->Play(3, 3.f, true);
		}

		// 땅이 아니면 점프 애니메이션
		else
		{
			if (pFP->GetCurFlipbook() != pFP->GetFlipbook(3))
			{
				pFP->Play(3, 3.f, true);
			}
		}
	}


	if (KEY_STATE::PRESSED == CKeyMgr::GetInst()->GetKeyState(KEY::Z))
	{
		// 라디안 단위로 계산
		// 3.14가 360도
		Vector3 vScale = Transform()->GetRelativeScale();
		vScale.x += 200.f * DT;
		vScale.y += 200.f * DT;
		Transform()->SetRelativeScale(vScale);

		Vector3 vRot = Transform()->GetRelativeRotation();
		vRot.z += XM_PI * DT;
		Transform()->SetRelativeRotation(vRot);
	}

	if (KEY_STATE::PRESSED == CKeyMgr::GetInst()->GetKeyState(KEY::NUM0))
	{
		Vector3 vScale = Transform()->GetRelativeScale();
		vScale.x += DT * 0.1f;
		vScale.y += DT * 0.1f;
		Transform()->SetRelativeScale(vScale);
	}

	if (KEY_TAP(KEY::NUM9))
		DrawDebugRect(Vector4(0.f, 1.f, 0.f, 0.5f), Transform()->GetRelativePos(), Vector2(200.f, 200.f), Vector3(0.f, 0.f, 0.f), true, 0.f);

	// 미사일 발사
	if (KEY_TAP(KEY::TAB))
	{
		CGameObject* pMissileObject = new CGameObject;
		pMissileObject->AddComponent(new CMeshRender);
		pMissileObject->AddComponent(new CMissileScript);
		pMissileObject->AddComponent(new CCollider2D);
		CCollider2D* pCol = (CCollider2D*)pMissileObject->GetComponent(COMPONENT_TYPE::COLLIDER2D);
		pCol->SetScale(Vector2(1.f, 1.f));

		pMissileObject->Transform()->SetRelativePos(Transform()->GetRelativePos());
		pMissileObject->Transform()->SetRelativeScale(Vector3(50.f, 100.f, 1.f));
		pMissileObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));

		CreateObject(pMissileObject, 0, false);
	}
	

	if (KEY_TAP(KEY::SPACE))
	{
		// 점프
		GetOwner()->RigidBody2D()->Jump();
		//GetOwner()->RigidBody2D()->AddVelociy(Vector2(0.f, 500.f));

		// 미사일 발사
		//CGameObject* pMissileObject = new CGameObject;
		//pMissileObject->AddComponent(new CMeshRender);
		//pMissileObject->AddComponent(new CMissileScript);
		//pMissileObject->AddComponent(new CCollider2D);
		//CCollider2D* pCol = (CCollider2D*)pMissileObject->GetComponent(COMPONENT_TYPE::COLLIDER2D);
		//pCol->SetScale(Vector2(1.f, 1.f));

		//pMissileObject->Transform()->SetRelativePos(Transform()->GetRelativePos());
		//pMissileObject->Transform()->SetRelativeScale(Vector3(50.f, 100.f, 1.f));
		//pMissileObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));

		//tTask task = {};
		//task.Type = TASK_TYPE::CREATE_OBJECT;
		//task.Param0 = (DWORD_PTR)pMissileObject;
		//task.Param1 = 0;

		//CTaskMgr::GetInst()->AddTask(task);
	}

	// 중력 적용
	if (KEY_TAP(KEY::G))
	{
		CRigidBody2D* pRigid = GetOwner()->RigidBody2D();
		pRigid->UseGravity(!pRigid->GetUseGravity());
		//CGameObject* pMissileObject = new CGameObject;
		//pMissileObject->AddComponent(new CMeshRender);
		//pMissileObject->AddComponent(new CMissileScript);
		//pMissileObject->AddComponent(new CCollider2D);
		//CCollider2D* pCol = (CCollider2D*)pMissileObject->GetComponent(COMPONENT_TYPE::COLLIDER2D);
		//pCol->SetScale(Vector2(1.f, 1.f));

		//pMissileObject->Transform()->SetRelativePos(Transform()->GetRelativePos());
		//pMissileObject->Transform()->SetRelativeScale(Vector3(50.f, 100.f, 1.f));
		//pMissileObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));

		//tTask task = {};
		//task.Type = TASK_TYPE::CREATE_OBJECT;
		//task.Param0 = (DWORD_PTR)pMissileObject;
		//task.Param1 = 0;

		//CTaskMgr::GetInst()->AddTask(task);
	}


	m_PaperBurnIntence += DT * 0.4f;

	if (GetOwner()->GetRenderComponent())
	{
		Ptr<CMaterial> pMtrl = GetOwner()->GetRenderComponent()->GetMaterial();

		if (nullptr != pMtrl)
		{
			pMtrl->SetScalarParam(FLOAT_0, m_PaperBurnIntence);
		}
	}
}

void CPlayerScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (L"Platform" == _OtherObject->GetName())
	{
		RigidBody2D()->SetGround(true);

		// 한번에 한 개의 플랫폼만 충돌 반응될 수 있도록 관리한다.
		m_Platform = _OtherObject;
		
		//// 플레이어를 플랫폼 윗면으로 밀어낸다 -> *플랫폼 내부에 있는 경우 예외처리*
		CPlatformScript* pPlat = _OtherObject->GetScript<CPlatformScript>();
		Vector3 vecDir = pPlat->GetInclination();
		Vector3 invDir = Vector3(-vecDir.y, vecDir.x, 0.f);
		Vector3 vecDist = Transform()->GetWorldPos() - pPlat->GetEndPos();
		float dist = abs(invDir.Dot(vecDist)) - 2;
		invDir.Normalize();

		Transform()->SetWorldPos(Transform()->GetWorldPos() + invDir * dist);

		Vector3 velo = RigidBody2D()->GetVelocity();
		float moveScale = vecDir.Dot(velo);
		RigidBody2D()->SetVelocity(pPlat->GetInclination() * moveScale);
	}
}

void CPlayerScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (L"Platform" == _OtherObject->GetName() && m_Platform == _OtherObject)
	{
		// 플레이어 velocity를 플랫폼과 투영시켜 이동방향을 변경
		CPlatformScript* pPlat = _OtherObject->GetScript<CPlatformScript>();
		Vector3 velo = RigidBody2D()->GetVelocity();
		float moveScale = pPlat->GetInclination().Dot(velo);
		RigidBody2D()->SetVelocity(pPlat->GetInclination() * moveScale);
	}
}

void CPlayerScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (L"Platform" == _OtherObject->GetName() && m_Platform == _OtherObject)
	{
		RigidBody2D()->SetGround(false);
		m_Platform = nullptr;
	}
}
