#include "pch.h"
#include "CMissileScript.h"


CMissileScript::CMissileScript()
	: CScript((UINT)SCRIPT_TYPE::CAMERASCRIPT)
	, m_Velocity(Vector3(0.f, 500.f, 0.f))
{
}

CMissileScript::~CMissileScript()
{
}


void CMissileScript::SaveComponent(FILE* _File)
{
	fwrite(&m_Velocity, sizeof(Vector3), 1, _File);
}

void CMissileScript::LoadComponent(FILE* _File)
{
	fread(&m_Velocity, sizeof(Vector3), 1, _File);
}

void CMissileScript::Begin()
{
	// CAssetMgr �� ������ �����Ų ������ ����
	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl");	
	MeshRender()->SetMaterial(pMtrl);
	MeshRender()->GetDynamicMaterial();
	
	Ptr<CTexture> pMissileTex = CAssetMgr::GetInst()->Load<CTexture>(L"Missile", L"Texture\\missile.png");
	MeshRender()->GetMaterial()->SetTexParam(TEX_0, pMissileTex);
}

void CMissileScript::Tick()
{
	Vector3 vWorldPos = Transform()->GetRelativePos();

	vWorldPos += m_Velocity * DT;

	Transform()->SetRelativePos(vWorldPos);
}