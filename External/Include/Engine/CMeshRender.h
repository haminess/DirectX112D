#pragma once
#include "CRenderComponent.h"

#include "assets.h"

class CMeshRender :
    public CRenderComponent
{
public:
    virtual void Render() override;
    virtual void FinalTick() override;

    virtual void LoadComponent(FILE* _FILE) override;

public:
    CLONE(CMeshRender);
    CMeshRender();
    ~CMeshRender();
};

