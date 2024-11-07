#pragma once
#include "CAsset.h"

// 1. Graphic Shader => ������ �ϴ� ���̴�
// 2. Compute Shader => ���������� �ʴ� ���̴�, �뷮 ���� ��Ŵ

class CShader :
    public CAsset
{
protected:
    ComPtr<ID3DBlob> m_ErrBlob;	// ������ ���� �� �޼����� �Է¹��� ��ü

public:
    virtual int Load(const wstring& _strFilepath) override { return S_OK; }
    virtual int Save(const wstring& _strFilepath) override { return S_OK; }

public:
    CShader(ASSET_TYPE _Type);
    ~CShader();
};

