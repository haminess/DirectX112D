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

	// 상대 충돌체의 투영축 구하기
 	Vector3 arrProj[4] = {};
	arrProj[0] = XMVector3TransformCoord(arrRect[1], matColRight) - XMVector3TransformCoord(arrRect[0], matColRight);	// 가로 투영축
	arrProj[1] = XMVector3TransformCoord(arrRect[3], matColRight) - XMVector3TransformCoord(arrRect[0], matColRight);	// 세로 투영축

	// 월드공간에서 두 충돌체의 중심을 이은 벡터
	Vector3 vCenter = XMVector3TransformCoord(Vector3(0.f, 0.f, 0.f), matColLeft) - XMVector3TransformCoord(Vector3(0.f, 0.f, 0.f), matColRight);

	float fOverDist[2] = {};
	for (int i = 0; i < 2; ++i)
	{
		Vector3 vProj = arrProj[i];
		vProj.Normalize();

		// 두 사각형의 투영값 - 중심벡터 투영값 = 겹친 부분
		float fCenter = fabs(vCenter.Dot(vProj));

		// 두 사각형의 투영값 구하기
		float fDist = 0.f;
		for (int j = 0; j < 4; ++j)
		{
			fDist += fabs(vProj.Dot(arrProj[j]));
		}
		fDist /= 2.f;

		fOverDist[i] = abs(fCenter - fDist);
	}

	// 가로로 충돌
 	Vector3 vPos = Transform()->GetRelativePos();
	if (fOverDist[0] >= fOverDist[1])
	{
		// 오른쪽에 있다
		if (vCenter.x >= 0)
		{
			vPos.x += fOverDist[1] * 2.f;
		}

		// 왼쪽에 있다
		else
		{
			vPos.x -= fOverDist[1] * 2.f;
		}
	}

	// 세로로 충돌
	else
	{
		// 위에 있다
		if (vCenter.y >= 0)
		{
			vPos.y += fOverDist[0] * 2.f;
		}

		// 아래 있다
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
//	// 두 도형의 길이의 합
//	Vector2 scale = (GetScale() + _Other->GetScale()) / 2;
//
//	// 센터끼리의 길이
//	Vector2 pos = (GetFinalPos() - _Other->GetFinalPos());
//	if (pos.x < 0) pos.x *= -1;
//	if (pos.y < 0) pos.y *= -1;
//
//	// 센터 길이가 더 짧으면 충돌된 것
//	Vector2 compare = scale - pos;
//	if (compare.x < 0) compare.x *= -1;
//	if (compare.y < 0) compare.y *= -1;
//
//	Vector2 move = (GetOwner()->GetPos() - GetOwner()->GetPrePos()) * pos;
//
//	// 가로로 충돌 => 세로 면이 더 많이 겹쳤다
//	if (compare.x < compare.y && move.x < 0)
//	{
//		GetOwner()->SetPos(GetOwner()->GetPrePos().x, GetOwner()->GetPos().y);
//	}
//
//	// 세로로 충돌 => 가로 면이 더 많이 겹쳤다
//	else if (compare.x > compare.y && move.y < 0)
//	{
//		GetOwner()->SetPos(GetOwner()->GetPos().x, GetOwner()->GetPrePos().y);
//	}
//}