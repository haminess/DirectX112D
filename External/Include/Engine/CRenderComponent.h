#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"


class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>              m_Mesh;

    Ptr<CMaterial>          m_SharedMtrl;   // ���� ����
    Ptr<CMaterial>          m_CurMtrl;      // ���� ��� ���� ����
    Ptr<CMaterial>          m_DynamicMtrl;   // ���� ����

public:
    void SetMesh(Ptr<CMesh> _Mesh) { m_Mesh = _Mesh; }

    // ���� ������ ����ϴ� �Լ�
    void SetMaterial(Ptr<CMaterial> _Material);     

    Ptr<CMesh> GetMesh() { return m_Mesh; }
    Ptr<CMaterial> GetMaterial() { return m_CurMtrl; }
    Ptr<CMaterial> GetSharedMaterial();
    Ptr<CMaterial> GetDynamicMaterial();
    void CreateDynamicMaterial();


public:
    virtual void Render() = 0;

public:
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

public:
    virtual CRenderComponent* Clone() = 0;
    CRenderComponent(COMPONENT_TYPE _Type);
    CRenderComponent(const CRenderComponent& _Origin);
    ~CRenderComponent();
};

