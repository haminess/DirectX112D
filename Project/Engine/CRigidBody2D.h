#pragma once
#include "CComponent.h"
enum class RIGIDBODY_MODE
{
    TOPVIEW,
    PLATFOMER,
};


class CRigidBody2D :
    public CComponent
{
private:
    RIGIDBODY_MODE      m_Mode;
    Vector3             m_Force;
    Vector3             m_Velocity;
    float               m_Mass;
    float               m_Friction;
    float               m_MaxSpeed;

    Vector3             m_GravityForce;
    Vector3             m_GravityVelocity;
    float               m_GravityMaxSpeed;
    float               m_GravityAccel;

    bool                m_IsGround;
    bool                m_IsMove;
    bool                m_UseGravity;

    float m_JumpSpeed;

public:

    void AddForce(Vector3 _Force) { m_Force += _Force; }
    void AddForce(Vector2 _Force) { m_Force += Vector3(_Force.x, _Force.y, 0.f); }
    void AddVelociy(Vector3 _Velocity) { m_Velocity += _Velocity; }
    void AddVelociy(Vector2 _Velocity) { m_Force += Vector3(_Velocity.x, _Velocity.y, 0.f); }

    RIGIDBODY_MODE      GetMode() { return m_Mode; }
    Vector3             GetForce() { return m_Force; }
    Vector3             GetVelocity() { return m_Velocity; }
    float               GetMass() { return m_Mass; }
    float               GetFriction() { return m_Friction; }
    float               GetMaxSpeed() { return m_MaxSpeed; }
    Vector3             GetGravityForce() { return m_GravityForce; }
    Vector3             GetGravityVelocity() { return m_GravityVelocity; }
    float               GetGravityMaxSpeed() { return m_GravityMaxSpeed; }
    float               GetGravityAccel() { return m_GravityAccel; }
    bool                IsGround() { return m_IsGround; }
    bool                IsMove() { return m_IsMove; }
    bool                GetUseGravity() { return m_UseGravity; }
    float               GetJumpSpeed() { return m_JumpSpeed; }

    void SetMode(RIGIDBODY_MODE _Mode) { m_Mode = _Mode; }
    void SetForce(Vector3 _Force) { m_Force = _Force; }
    void SetVelocity(Vector3 _Velocity) { m_Velocity = _Velocity; }
    void SetMass(float _Mass) { m_Mass = _Mass; }
    void SetFriction(float _Friction) { m_Friction = _Friction; }
    void SetMaxSpeed(float _MaxSpeed) { m_MaxSpeed = _MaxSpeed; }
    void SetGravityForce(Vector3 _GravityForce) { m_GravityForce = _GravityForce; }
    void SetGravityVelocity(Vector3 _GravityVelocity) { m_GravityVelocity = _GravityVelocity; }
    void SetGravityMaxSpeed(float _GravityMaxSpeed) { m_GravityMaxSpeed = _GravityMaxSpeed; }
    void SetGravityAccel(float _GravityAccel) { m_GravityAccel = _GravityAccel; }
    void SetGround(bool _GrGroundd);
    void SetMovable(bool _IsMove) { m_IsMove = _IsMove; }
    void UseGravity(bool _Gravity) { m_UseGravity = _Gravity; }
    void SetJumpSpeed(float _JumpSpeed) { m_JumpSpeed = _JumpSpeed; }

public:
    virtual void FinalTick() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

public:
    void Jump();

public:
    CLONE(CRigidBody2D);
    CRigidBody2D();
    CRigidBody2D(const CRigidBody2D& _Origin);
    ~CRigidBody2D();
};

