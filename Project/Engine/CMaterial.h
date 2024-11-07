#pragma once
#include "CAsset.h"
#include "CGraphicShader.h"
class CGraphicShader;
class CTexture;
class CMaterial :
    public CAsset
{
private:
    Ptr<CGraphicShader> m_Shader;
    MtrlConst           m_Const;
    Ptr<CTexture>       m_arrTex[(UINT)TEX_PARAM::TEX_END];

    // ���� ������ ����Ű�� ������(nullptr)�� ��� ������ ����(����)
    Ptr<CMaterial>      m_SharedMtrl;


public:
    void Binding();

public:
    virtual int Save(const wstring& _FilePath) override;

private:
    virtual int Load(const wstring& _strFilePath) override;

public:
    template <typename T>
    void SetScalarParam(SCALAR_PARAM _Type, const T& _Data);
    void SetTexParam(TEX_PARAM _Param, Ptr<CTexture> _tex);

    void SetShader(Ptr<CGraphicShader> _Shader) { m_Shader = _Shader; }
    Ptr<CGraphicShader> GetShader() { return m_Shader; }

    Ptr<CMaterial> GetSharedMtrl() { return   m_SharedMtrl; }
    Ptr<CTexture> GetTexture(int _idx);

public:
    virtual CMaterial* Clone() override;

private:
    CMaterial(const CMaterial& _Other);

public:
    CMaterial(bool _EngineRes = false);
    ~CMaterial();
};


template<typename T>
inline void CMaterial::SetScalarParam(SCALAR_PARAM _Type, const T& _Data)
{
    switch (_Type)
    {
    case INT_0:
    case INT_1:
    case INT_2:
    case INT_3:
        if constexpr (std::is_same_v<T, int>)
        {
            m_Const.iArr[_Type] = _Data;
        }

        break;

    case FLOAT_0:
    case FLOAT_1:
    case FLOAT_2:
    case FLOAT_3:
        if constexpr (std::is_same_v<T, float>)
        {
            m_Const.fArr[_Type - FLOAT_0] = _Data;
        }
        break;

    case VEC2_0:
    case VEC2_1:
    case VEC2_2:
    case VEC2_3:
        if constexpr (std::is_same_v<T, Vector2>)
        {
            m_Const.v2Arr[_Type - VEC2_0] = _Data;
        }
        break;

    case VEC4_0:
    case VEC4_1:
    case VEC4_2:
    case VEC4_3:
        if constexpr (std::is_same_v<T, Vector4>)
        {
            m_Const.v4Arr[_Type - VEC4_0] = _Data;
        }
        break;

    case MAT_0:
    case MAT_1:
        if constexpr (std::is_same_v<T, Matrix>)
        {
            m_Const.mat[_Type - MAT_0] = _Data;
        }
        break;
    }
}
