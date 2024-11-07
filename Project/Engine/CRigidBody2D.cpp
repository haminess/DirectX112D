#include "pch.h"
#include "CRigidBody2D.h"
#include "CTimeMgr.h"
#include "CTransform.h"

CRigidBody2D::CRigidBody2D()
	:CComponent(COMPONENT_TYPE::RIGIDBODY2D)
	, m_Mode(RIGIDBODY_MODE::PLATFOMER)
	, m_MaxSpeed(0.f)
	, m_IsMove(false)
	, m_Friction(1000.f)
	, m_UseGravity(true)
	, m_GravityAccel(1500.f)
	, m_JumpSpeed(800.f)
	, m_IsGround(false)
	, m_Mass(1.f)
	, m_GravityMaxSpeed(3000.f)
{
}

CRigidBody2D::CRigidBody2D(const CRigidBody2D& _Origin)
	: CComponent(_Origin)
	, m_Mode(_Origin.m_Mode)
	, m_Mass(_Origin.m_Mass)
	, m_Friction(_Origin.m_Friction)
	, m_MaxSpeed(_Origin.m_MaxSpeed)
	, m_GravityMaxSpeed(_Origin.m_GravityMaxSpeed)
	, m_IsGround(false)
	, m_UseGravity(_Origin.m_UseGravity)
	, m_JumpSpeed(_Origin.m_JumpSpeed)
{
}

CRigidBody2D::~CRigidBody2D()
{
}


void CRigidBody2D::SaveComponent(FILE* _File)
{
	fwrite(&m_Mode, sizeof(RIGIDBODY_MODE), 1, _File);
	fwrite(&m_Mass, sizeof(float), 1, _File);
	fwrite(&m_Friction, sizeof(float), 1, _File);
	fwrite(&m_MaxSpeed, sizeof(float), 1, _File);
	fwrite(&m_GravityForce, sizeof(Vector3), 1, _File);
	fwrite(&m_GravityMaxSpeed, sizeof(float), 1, _File);
	fwrite(&m_UseGravity, sizeof(bool), 1, _File);
	fwrite(&m_JumpSpeed, sizeof(float), 1, _File);
}

void CRigidBody2D::LoadComponent(FILE* _File)
{
	fread(&m_Mode, sizeof(RIGIDBODY_MODE), 1, _File);
	fread(&m_Mass, sizeof(float), 1, _File);
	fread(&m_Friction, sizeof(float), 1, _File);
	fread(&m_MaxSpeed, sizeof(float), 1, _File);
	fread(&m_GravityForce, sizeof(Vector3), 1, _File);
	fread(&m_GravityMaxSpeed, sizeof(float), 1, _File);
	fread(&m_UseGravity, sizeof(bool), 1, _File);
	fread(&m_JumpSpeed, sizeof(float), 1, _File);
}



void CRigidBody2D::FinalTick()
{
	// ���ӵ� ���ϱ�
	// F == M x A
	// F / M == A
	Vector3 Accel = m_Force / m_Mass;

	// ���ӵ� �������� ���ӵ��� ũ�⸸ŭ �ӵ��� ������
	m_Velocity += Accel * DT;

	// �ִ�ӷ� üũ, 0.f == m_MaxSpeed ==> �ӷ� ������ ����
	if (0.f != m_MaxSpeed)
	{
		if (m_MaxSpeed < m_Velocity.Length())
		{
			m_Velocity.Normalize();
			m_Velocity *= m_MaxSpeed;
		}
	}

	// �־��� ���� ���ٸ�, �ݴ�������� �������� �ش�.
	if (m_Force == Vector3(0.f, 0.f, 0.f) && m_Velocity != Vector3(0.f, 0.f, 0.f))
	{
		Vector3 vFriction = -m_Velocity;
		vFriction.Normalize();
		vFriction *= m_Friction * m_Mass * DT;

		if (m_Velocity.Length() < vFriction.Length())
		{
			m_Velocity = Vector3(0.f, 0.f, 0.f);
		}
		else
		{
			m_Velocity += vFriction;
		}
	}


	Vector3 vFinalVelocity = m_Velocity;

	// RigidBody �� Plarformer ����ΰ��
	if (m_Mode == RIGIDBODY_MODE::PLATFOMER)
	{
		// �߷±�� On
		if (m_UseGravity && !m_IsGround)
		{
			m_GravityVelocity += Vector3(0.f, -1.f, 0.f) * m_GravityAccel * DT;
			if (m_GravityMaxSpeed < m_GravityVelocity.Length())
			{
				m_GravityVelocity.Normalize();
				m_GravityVelocity *= m_GravityMaxSpeed;
			}

			vFinalVelocity += m_GravityVelocity;
		}
		// �߷±�� Off
		else
		{
			m_GravityVelocity = Vector3(0.f, 0.f, 0.f);
		}
	}


	// �ӵ��� ���� ��ü�� �̵� �߻�
	Vector3 vObjPos = GetOwner()->Transform()->GetWorldPos();
	GetOwner()->Transform()->SetWorldPos(vObjPos + vFinalVelocity * DT);

	// ������Ʈ�� ������ üũ
	if (vObjPos == GetOwner()->Transform()->GetWorldPos())
		m_IsMove = false;
	else
		m_IsMove = true;

	// �̹��� ���� ���� 0 ���� �ʱ�ȭ
	m_Force = Vector3(0.f, 0.f, 0.f);

	// �ӵ��� ������ �ð�ȭ
	DrawDebugLine(Vector4(1.f, 1.f, 0.f, 1.f), vObjPos, vObjPos + m_Velocity);
	//printf(std::to_string(m_Velocity.Length()).c_str());
	//printf("\n");
}

void CRigidBody2D::SetGround(bool _Ground)
{
	m_IsGround = _Ground;

	if (m_IsGround)
	{
		m_GravityVelocity = Vector3(0.f, 0.f, 0.f);
	}
}

void CRigidBody2D::Jump()
{
	m_GravityVelocity += (Vector3(0.f, 1.f, 0.f) * m_JumpSpeed);
	m_IsGround = false;
}