#pragma once
#include "CEntity.h"
#include "CGameObject.h"

// C##은 Type에 C를 붙여주겠다는 의미
#define GET_OTHER_COMPONENT(Type) C##Type* Type() { return m_Owner->Type(); }


class CComponent :
    public CEntity
{
private:
    const COMPONENT_TYPE m_Type;
    CGameObject* m_Owner;
    bool m_Active;
    
public:
    COMPONENT_TYPE GetType() { return m_Type; }
    CGameObject* GetOwner() { return m_Owner; }
    void Active() { m_Active = true; }
    void SetActive(bool _Set) { m_Active = _Set; }
    void Deactive() { m_Active = false; }
    bool IsActive() { return m_Active; }

    GET_OTHER_COMPONENT(Transform);
    GET_OTHER_COMPONENT(MeshRender);
    GET_OTHER_COMPONENT(Camera);
    GET_OTHER_COMPONENT(Collider2D);
    GET_OTHER_COMPONENT(FlipbookPlayer);
    GET_OTHER_COMPONENT(TileMap);
    GET_OTHER_COMPONENT(Light2D);
    GET_OTHER_COMPONENT(RigidBody2D);
    GET_OTHER_COMPONENT(ParticleSystem);

public:
    virtual void Init() {};
    virtual void Begin() {};
    virtual void Tick() {};
    virtual void FinalTick() = 0;

public:
    virtual CComponent* Clone() = 0;

    virtual void SaveToLevel(FILE* _File) override;
    virtual void LoadFromLevel(FILE* _File) override;

    virtual void SaveComponent(FILE* _File) = 0;
    virtual void LoadComponent(FILE* _File) = 0;

    CComponent(COMPONENT_TYPE _Type);
    ~CComponent();

    friend class CGameObject;
};

