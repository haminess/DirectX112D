#include "pch.h"
#include "CCollisionReactScript.h"

CCollisionReactScript::CCollisionReactScript()
	: CScript((UINT)SCRIPT_TYPE::COLLISIONREACTSCRIPT)
{
}

CCollisionReactScript::~CCollisionReactScript()
{
}

void CCollisionReactScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
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

	// ��� �浹ü�� ������ ���ϱ�
 	Vector3 arrProj[4] = {};
	arrProj[0] = XMVector3TransformCoord(arrRect[1], matColRight) - XMVector3TransformCoord(arrRect[0], matColRight);	// ���� ������
	arrProj[1] = XMVector3TransformCoord(arrRect[3], matColRight) - XMVector3TransformCoord(arrRect[0], matColRight);	// ���� ������

	// ����������� �� �浹ü�� �߽��� ���� ����
	Vector3 vCenter = XMVector3TransformCoord(Vector3(0.f, 0.f, 0.f), matColLeft) - XMVector3TransformCoord(Vector3(0.f, 0.f, 0.f), matColRight);

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
 	Vector3 vPos = Transform()->GetRelativePos();
	if (fOverDist[0] >= fOverDist[1])
	{
		// �����ʿ� �ִ�
		if (vCenter.x >= 0)
		{
			vPos.x += fOverDist[1] * 2.f;
		}

		// ���ʿ� �ִ�
		else
		{
			vPos.x -= fOverDist[1] * 2.f;
		}
	}

	// ���η� �浹
	else
	{
		// ���� �ִ�
		if (vCenter.y >= 0)
		{
			vPos.y += fOverDist[0] * 2.f;
		}

		// �Ʒ� �ִ�
		else
		{
			vPos.y -= fOverDist[0] * 2.f;
		}
	}

	Transform()->SetRelativePos(vPos);
}

void CCollisionReactScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CCollisionReactScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}
//
//
//if (m_Collidable && _Other->IsCollidable())
//{
//	// �� ������ ������ ��
//	Vector2 scale = (GetScale() + _Other->GetScale()) / 2;
//
//	// ���ͳ����� ����
//	Vector2 pos = (GetFinalPos() - _Other->GetFinalPos());
//	if (pos.x < 0) pos.x *= -1;
//	if (pos.y < 0) pos.y *= -1;
//
//	// ���� ���̰� �� ª���� �浹�� ��
//	Vector2 compare = scale - pos;
//	if (compare.x < 0) compare.x *= -1;
//	if (compare.y < 0) compare.y *= -1;
//
//	Vector2 move = (GetOwner()->GetPos() - GetOwner()->GetPrePos()) * pos;
//
//	// ���η� �浹 => ���� ���� �� ���� ���ƴ�
//	if (compare.x < compare.y && move.x < 0)
//	{
//		GetOwner()->SetPos(GetOwner()->GetPrePos().x, GetOwner()->GetPos().y);
//	}
//
//	// ���η� �浹 => ���� ���� �� ���� ���ƴ�
//	else if (compare.x > compare.y && move.y < 0)
//	{
//		GetOwner()->SetPos(GetOwner()->GetPos().x, GetOwner()->GetPrePos().y);
//	}
//}