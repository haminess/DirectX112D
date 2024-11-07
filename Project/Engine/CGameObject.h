#pragma once
#include "CEntity.h"

class CComponent;
class CRenderComponent;
class CScript;

class CGameObject :
    public CEntity
{
private:
    CComponent*             m_arrCom[(UINT)COMPONENT_TYPE::END];
    CRenderComponent*       m_RenderCom;    // 1개의 게임오브젝트 객체는 1개의 렌더 컴포넌트를 가질 수 있다.
    vector<CScript*>        m_vecScripts;   // 보유 스크립트들

    CGameObject*            m_Parent;       // 부모 오브젝트
    vector<CGameObject*>    m_vecChild;     // 자식 오브젝트들

    int                     m_LayerIdx;     // 오브젝트가 속해있는 레이어 인덱스 번호, -1 : 무소속, Level에 쓰이지 않음
    bool                    m_Active;
    bool                    m_Dead;         // 오브젝트의 삭제 예정인 상태값


public:
    void Begin();
    void Tick();
    void FinalTick();
    void Render();


public:
    void SetActive(bool _bActive);
    bool GetActive() { return m_Active; }

    void AddComponent(CComponent* _Component);
    void AddChild(CGameObject* _Child);

    CGameObject* GetParent() { return m_Parent; }
    vector<CGameObject*> GetChild() { return m_vecChild; }
    CComponent* GetComponent(COMPONENT_TYPE _Type) { return m_arrCom[(UINT)_Type]; }
    CRenderComponent* GetRenderComponent() { return m_RenderCom; }

    template<typename T>
    T* GetScript() 
    { 
        T* pScript = nullptr;

        for (int i = 0; i < m_vecScripts.size(); ++i)
        {
            pScript = dynamic_cast<T*>(m_vecScripts[i]);
            if (pScript)
                return pScript;
        }
        return nullptr;
    }

    int GetLayerIdx() { return m_LayerIdx; }
    void SetLayerIdx(int _idx);
    bool IsDead() { return m_Dead; }
    bool IsValid() { return !m_Dead; }
    bool IsAncestor(CGameObject* _Other);


    const vector<CScript*>& GetScripts() { return m_vecScripts; }
    class CTransform* Transform() { return (CTransform*)GetComponent(COMPONENT_TYPE::TRANSFORM); }
    class CMeshRender* MeshRender() { return (CMeshRender*)GetComponent(COMPONENT_TYPE::MESHRENDER); }
    class CCamera* Camera() { return (CCamera*)GetComponent(COMPONENT_TYPE::CAMERA); }
    class CCollider2D* Collider2D() { return (CCollider2D*)GetComponent(COMPONENT_TYPE::COLLIDER2D); }
    class CFlipbookPlayer* FlipbookPlayer() { return (CFlipbookPlayer*)GetComponent(COMPONENT_TYPE::FLIPBOOKPLAYER); }
    class CTileMap* TileMap() { return (CTileMap*)GetComponent(COMPONENT_TYPE::TILEMAP); }
    class CLight2D* Light2D() { return (CLight2D*)GetComponent(COMPONENT_TYPE::LIGHT2D); }
    class CRigidBody2D* RigidBody2D() { return (CRigidBody2D*)GetComponent(COMPONENT_TYPE::RIGIDBODY2D); }
    class CParticleSystem* ParticleSystem() { return (CParticleSystem*)GetComponent(COMPONENT_TYPE::PARTICLE_SYSTEM); }

private:
    void DisconnectWithLayer();
    void DisconnectWithParent();
    void RegisterAsParent();

public:
    CLONE(CGameObject);
    CGameObject();
    CGameObject(const CGameObject& _Origin);
    ~CGameObject();

    friend class CLayer;
    friend class CTaskMgr;
};

