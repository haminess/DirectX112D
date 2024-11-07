#pragma once
#include "CComponent.h"

class CCollider2D :
    public CComponent
{
protected:
    Vector2     m_Offset;
    Vector2     m_Scale;
    Vector2     m_FinalPos;
    Matrix      m_matColliderWorld;     // 크기, 회전, 이동
    bool        m_IndependentScale;

    int         m_OverlapCount;

    bool        m_IsCollidable;
    bool        m_IsMovable;


public:
    void SetOffset(Vector2 _Offset) { m_Offset = _Offset; }
    void SetScale(Vector2 _Scale) { m_Scale = _Scale; }

    Vector2 GetOffset() { return m_Offset; }
    Vector2 GetScale() { return m_Scale; }

    const Matrix& GetColliderWorldMat() { return m_matColliderWorld; }

    void SetIndependentScale(bool _Scale) { m_IndependentScale = _Scale; }
    bool IsIndependentScale() { return m_IndependentScale; }
    void SetMovable(bool _Move) { m_IsMovable = _Move; }
    void SetCollidable(bool _Collision) { m_IsCollidable = _Collision; }
    bool IsMovable() { return m_IsMovable; }
    bool IsCollidable() { return m_IsCollidable; }

public:
    virtual void FinalTick() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

public:
    void BeginOverlap(CCollider2D* _Other);
    void Overlap(CCollider2D* _Other);
    void EndOverlap(CCollider2D* _Other);

public:
    CLONE(CCollider2D);
    CCollider2D();
    CCollider2D(const CCollider2D& _Origin);
    ~CCollider2D();
};

