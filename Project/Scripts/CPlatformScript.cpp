#include "pch.h"
#include "CPlatformScript.h"
#include <Engine/CRigidBody2D.h>
#include <Engine/CAssetMgr.h>

CPlatformScript::CPlatformScript()
	: CScript((UINT)SCRIPT_TYPE::PLATFORMSCRIPT)
	, m_Height(10)
	, m_SideCollidable(false)
{
}

CPlatformScript::~CPlatformScript()
{
}


void CPlatformScript::SetPlatformPos(Vector3 _StartPos, Vector3 _EndPos, float _Height)
{
	// �÷��� ���� ������Ʈ
	m_StartPos = _StartPos;
	m_EndPos = _EndPos;
	m_Height = _Height;
	Vector3 vecDir = m_EndPos - m_StartPos;

	SetInclination(vecDir);

	// �÷��� ��ġ ����
	Vector3 invDir = Vector3(-vecDir.y, vecDir.x, 0.f);
	invDir.Normalize();

	Vector3 vPos = m_StartPos + Vector3(vecDir / 2.f) - Vector3(invDir * m_Height / 2.f);
	Vector3 vScale = Vector3(vecDir.Length(), m_Height, 1.f);
	Vector3 vRot = Vector3(0.f, 0.f, atan(m_vecInclination.y / m_vecInclination.x));
	Transform()->SetWorldPos(vPos);
	Transform()->SetRelativeScale(vScale);
	Transform()->SetRelativeRotation(vRot);

	// Begin ȣ�� ���� ������ ���� �ȵǰ� �ϱ�
	if(Collider2D())
		Collider2D()->SetScale(Vector2(vScale.x, vScale.y));
}

void CPlatformScript::BlockOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (!_Collider->IsCollidable() || !_OtherCollider->IsCollidable())
		return;

	static Vector3 arrRect[4] =
	{
		Vector3(-0.5f, 0.5f, 0.f),
		Vector3(0.5f, 0.5f, 0.f),
		Vector3(0.5f, -0.5f, 0.f),
		Vector3(-0.5f, -0.5f, 0.f)
	};

	Matrix matColLeft = _Collider->GetColliderWorldMat();
	Matrix matColRight = _OtherCollider->GetColliderWorldMat();

	// �� ������ ���ϱ�
	Vector3 arrProj[4] = {};
	arrProj[0] = XMVector3TransformCoord(arrRect[1], matColLeft) - XMVector3TransformCoord(arrRect[0], matColLeft);	// ���� ������
	arrProj[1] = XMVector3TransformCoord(arrRect[0], matColLeft) - XMVector3TransformCoord(arrRect[3], matColLeft);	// ���� ������
	arrProj[2] = XMVector3TransformCoord(arrRect[1], matColRight) - XMVector3TransformCoord(arrRect[0], matColRight);
	arrProj[3] = XMVector3TransformCoord(arrRect[0], matColRight) - XMVector3TransformCoord(arrRect[3], matColRight);

	// ����������� �� �浹ü�� �߽��� ���� ����
	Vector3 vCenter = XMVector3TransformCoord(Vector3(0.f, 0.f, 0.f), matColRight) - XMVector3TransformCoord(Vector3(0.f, 0.f, 0.f), matColLeft);

	float fOverDist[2] = {};
	for (int i = 0; i < 2; ++i)
	{
		Vector3 vProj = arrProj[i];
		vProj.Normalize();

		// �� �簢���� ������ - �߽ɺ��� ������ = ��ģ �κ�
		float fCenter = fabs(vCenter.Dot(vProj));

		// �� �簢���� ������ ���ϱ�
		float fDist = 0.f;
		for (int j = 0; j < 4; ++j)
		{
			fDist += fabs(vProj.Dot(arrProj[j]));
		}
		fDist /= 2.f;

		fOverDist[i] = abs(fCenter - fDist);
	}

	// ���η� �浹
	Vector3 vPos = _OtherObject->Transform()->GetWorldPos();
	Vector3 colPos = vPos + _OtherCollider->GetOffset();		// �浹 ������Ʈ�� �ڽ��� ��� ����ó�� 
	Vector2 colScale = _OtherCollider->GetScale();
	Vector3 platPos = Transform()->GetWorldPos();
	Vector3 platScale = Transform()->GetWorldScale();
	if (fOverDist[0] >= fOverDist[1])
	{
		// ���� �ִ�
		if (vCenter.y >= 0)
		{
			//vPos.y += fOverDist[1] * 2.f;
			vPos.y = (int)(platPos.y + platScale.y / 2.f + colScale.y / 2.f - 10);
		}

		// �Ʒ��� �ִ�
		else
		{
			//vPos.y -= fOverDist[1] * 2.f;
			vPos.y = platPos.y - platScale.y / 2.f - colScale.y / 2.f - 1;
		}
	}

	// ���η� �浹
	else
	{
		// �����ʿ� �ִ�
		if (vCenter.x >= 0)
		{
			//vPos.x += fOverDist[0] * 2.f;
			vPos.x = platPos.x + platScale.x / 2.f + colScale.x / 2.f - 1;
		}

		// ���ʿ� �ִ�
		else
		{
			//vPos.x -= fOverDist[0] * 2.f;
			vPos.x = platPos.x - platScale.x / 2.f - colScale.x / 2.f - 1;
		}
	}

	_OtherObject->Transform()->SetWorldPos(vPos);
}

void CPlatformScript::Init()
{
	// �÷����� �ʿ��� ������Ʈ �߰�
	CGameObject* pObject = GetOwner();
	pObject->SetName(L"Platform");
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);
	pObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"WaterTex"));

	if (!Collider2D()->IsIndependentScale())
	{
		Collider2D()->SetIndependentScale(true);
	}
	Collider2D()->SetCollidable(true);

	SetPlatformPos(m_StartPos, m_EndPos, m_Height);
}

void CPlatformScript::Begin()
{

}

void CPlatformScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (L"Player" == _OtherObject->GetName())
	{
		//BlockOverlap(_Collider, _OtherObject, _OtherCollider);
	}
}
void CPlatformScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (L"Player" == _OtherObject->GetName())
	{
		//BlockOverlap(_Collider, _OtherObject, _OtherCollider);
	}
}

void CPlatformScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (L"Player" == _OtherObject->GetName())
	{
	}
}