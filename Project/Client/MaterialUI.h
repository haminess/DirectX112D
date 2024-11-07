#pragma once
#include "AssetUI.h"
class MaterialUI :
    public AssetUI
{
private:

public:
    virtual void Render_Update() override;

private:
    void SelectGraphicShader(DWORD_PTR _ListUI, DWORD_PTR _SelectString);

public:
    MaterialUI();
    ~MaterialUI();
};

