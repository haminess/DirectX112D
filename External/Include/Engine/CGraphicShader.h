#pragma once
#include "CShader.h"
class CGraphicShader :
    public CShader
{
private:
    // ���̴�
    // �׷��� ������ ���� ä�� �־���
    ComPtr<ID3DBlob>                m_VSBlob; // �����
    ComPtr<ID3DBlob>                m_GSBlob; 
    ComPtr<ID3DBlob>                m_PSBlob; // �ش� �޸𸮵��� Ȱ���� �Ʒ� ���̴����� ���� ��

    ComPtr<ID3D11VertexShader>      m_VS; // ���̴�
    ComPtr<ID3D11GeometryShader>    m_GS;
    ComPtr<ID3D11PixelShader>       m_PS;

    // Input Layout (�Է� ����)
    // ���޽�Ű�� ������ �޸� ����
    // ������ ���� �����߱� ������ �޸� ũ�� �� �˷���� ��
    // Vector4, Vector2(Temp) => float3(shader)�� ��ȯ�ϱ� ����
    // Vtx ����ü ũ�� 36����Ʈ, DX ������� #pragma pack(1) ����Ǿ� �ֱ� ����
    ComPtr<ID3D11InputLayout> m_Layout;
    D3D11_PRIMITIVE_TOPOLOGY m_Topology;

    RS_TYPE                     m_RSType;   // �����Ͷ����� ������Ʈ �ɼ�
    BS_TYPE                     m_BSType;   // ���� ������Ʈ �ɼ�
    DS_TYPE                     m_DSType;   // DepthStencilState �ɼ�

    SHADER_DOMAIN               m_Domain;


public:
    int CreateVertexShader(const wstring& _RelativePath, const string& _FuncName);
    int CreatePixelShader(const wstring& _RelativePath, const string& _FuncName);
    int CreateGeometryShader(const wstring& _RelativePath, const string& _FuncName);

public:
    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology) { m_Topology = _Topology; }
    void SetRSState(RS_TYPE _State) { m_RSType = _State; }
    void SetBSState(BS_TYPE _State) { m_BSType = _State; }
    void SetDSState(DS_TYPE _Type) { m_DSType = _Type; }

    void SetDomain(SHADER_DOMAIN _Domain) { m_Domain = _Domain; }
    SHADER_DOMAIN GetDomain() { return m_Domain; }

    void Binding();

public:
    CLONE_DISABLE(CGraphicShader);
    CGraphicShader();
    ~CGraphicShader();
};

