#pragma once
#include "CRenderComponent.h"

#include "Ptr.h"
#include "CParticleTickCS.h"

class CStructuredBuffer;

struct tSpawnCount
{
    int SpawnCount;
    int Padding[3];
};

class CParticleSystem :
    public CRenderComponent
{
private:
    Ptr<CParticleTickCS>    m_TickCS;
    CStructuredBuffer*      m_ParticleBuffer;       // ��� ��ƼŬ ����
    CStructuredBuffer*      m_SpawnCountBuffer;     // ��ƼŬ Ȱ��ȭ ���� ���޿� ����
    CStructuredBuffer*      m_ModuleBuffer;         // Module Data Buffer

    Ptr<CTexture>           m_ParticleTex;
    float                   m_Time;                 // �����ð�    
    float                   m_BurstTime;            // SpawnBurst üũ�� ����

    int                     m_MaxParticleCount;      // ��ƼŬ �ִ� ����

    tParticleModule         m_Module;               // ��ƼŬ�� ��� ����

public:
    void SetParticleTexture(Ptr<CTexture> _Tex) { m_ParticleTex = _Tex; }
    void SetMaxCount(int _Tex) { m_MaxParticleCount = _Tex; }
    void SetModule(tParticleModule _Tex) { m_Module = _Tex; }
    void SetModuleData();
    void ClearParticle();

    tParticleModule& GetModule() { return m_Module; }

private:
    void CaculateSpawnCount();

public:
    virtual void FinalTick() override;
    virtual void Render() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

public:
    CLONE(CParticleSystem);
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _Origin);
    ~CParticleSystem();

    friend class CParticleSystemScript;
};
