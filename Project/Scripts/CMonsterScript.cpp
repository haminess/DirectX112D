#include "pch.h"
#include "CMonsterScript.h"
#include "CParticleSystemScript.h"

void CMonsterScript::Begin()
{
	m_StandTex = CAssetMgr::GetInst()->Load<CTexture>(L"SnailTex", L"Texture//maple//stand.png");
	m_HitTex = CAssetMgr::GetInst()->Load<CTexture>(L"SnailAttackedTex", L"Texture//maple//hit1.png");
	m_ButterflyTex[0] = CAssetMgr::GetInst()->Load<CTexture>(L"BlueButterfly", L"Texture//maple//butterfly//blue.png");
	m_ButterflyTex[1] = CAssetMgr::GetInst()->Load<CTexture>(L"SkyButterfly", L"Texture//maple//butterfly//sky.png");
	m_ButterflyTex[2] = CAssetMgr::GetInst()->Load<CTexture>(L"PurpleButterfly", L"Texture//maple//butterfly//purple.png");

	CGameObject* pObj = new CGameObject;

	pObj->SetName(L"DSParticle");
	pObj->AddComponent(new CParticleSystem);
	pObj->AddComponent(new CParticleSystemScript);
	m_Particle = pObj->ParticleSystem();
	pObj->SetLayerIdx(0);

	GetOwner()->AddChild(pObj);

	m_Particle->SetParticleTexture(m_ButterflyTex[0]);
	m_Particle->SetMaxCount(30);
	m_Particle->GetModule().SpaceType = 2;
	m_Particle->GetModule().VelocityAlignment = 0;
	m_Particle->GetModule().vSpawnMaxScale = Vector4(100.f, 100.f, 0.f, 0.f);
	m_Particle->GetModule().vSpawnMinScale = Vector4(30.f, 30.f, 0.f, 0.f);
	m_Particle->GetModule().SpawnShapeScale = Vector4(100.f, 100.f, 0.f, 0.f);
	m_Particle->GetModule().AddVelocityType = 0;

	m_Particle->SetModuleData();
}

void CMonsterScript::Tick()
{
	int random = (int)DT % 3;
	m_Particle->SetParticleTexture(m_ButterflyTex[random]);
	m_Particle->SetModuleData();
}

void CMonsterScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_HitTex);
}

void CMonsterScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_StandTex);
}

CMonsterScript::CMonsterScript()
	: CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT)
{
}

CMonsterScript::~CMonsterScript()
{
}
