#pragma once
#include "CComponent.h"

class CGameObject;
class CCamera :
    public CComponent
{
private:
    PROJ_TYPE m_ProjType;   // ���� ���
    float m_Far;            // �þ� �ִ� �Ÿ�
    float m_AspectRatio;    // ���� ������ ���� ��Ⱦ��, ���� / ����
    UINT m_LayerCheck;      // ī�޶� ���� ���̾� ��Ʈ üũ

    // �������� (Perspective)
    float m_FOV;            // FOV(Field Of View) �þ߰�

    // �������� (Orthographic)
    float m_Width;          // �������� ���α���
    float m_Scale;          // �������� ����, �󸶳� �а� ������ ���̳�(Ŭ ���� ����)

    // ��ȯ���
    Matrix m_matView;       // View ���
    Matrix m_matProj;       // Projection ���

    int m_Priority;         // ī�޶� �켱����, 0 : maincamera, -1: �̵��(������ ����)

    // ��ü �з�
    vector<CGameObject*> m_vecOpaque;
    vector<CGameObject*> m_vecMask;
    vector<CGameObject*> m_vecTransparent;
    vector<CGameObject*> m_vecParticle;
    vector<CGameObject*> m_vecPostprocess;



public:
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }
    PROJ_TYPE GetProjType() { return m_ProjType; }

    void SetWidth(float _Width) { m_Width = _Width; }
    void SetAspectRatio(float _AR) { m_AspectRatio = _AR; }

    void SetScale(float _Scale) { m_Scale = _Scale; }
    float GetScale() { return m_Scale; }

    void SetFar(float _Far) { m_Far = _Far; }
    void SetFOV(float _FOV) { m_FOV = _FOV; }

    void SetPriority(int _Priority);
    
    void LayerCheck(int _LayerIdx);
    void LayerCheckAll() { m_LayerCheck = 0xffffffff; }
    void LayerCheckClear() { m_LayerCheck = 0; }
    UINT GetLayerCheck() { return m_LayerCheck; }


    float GetFar() { return m_Far; }
    float GetFOV() { return m_FOV; }
    float GetWidth() { return m_Width; }
    float GetAspectRatio() { return m_AspectRatio; }
    int GetCameraPriority() { return m_Priority; }

public:
    virtual void Begin() override;
    virtual void FinalTick() override;

    void SortObject();
    void Render();

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

public:
    CLONE(CCamera);
    CCamera();
    CCamera(const CCamera& _Origin);
    ~CCamera();
};

