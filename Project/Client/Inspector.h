#pragma once
#include "EditorUI.h"
#include "ScriptUI.h"

#include <Engine/Ptr.h>
#include <Engine/assets.h>


class CGameObject;
class ComponentUI;
class AssetUI;

class Inspector :
    public EditorUI
{
private:
    CGameObject* m_TargetObject;
    Ptr<CAsset>     m_TargetAsset;

    ComponentUI* m_arrComUI[(UINT)COMPONENT_TYPE::END];
    vector<ScriptUI*> m_vecScriptUI;
    AssetUI* m_arrAssetUI[(UINT)ASSET_TYPE::END];

public:
    void SetTargetObject(CGameObject* _Target);
    CGameObject* GetTargetObject() { return m_TargetObject; }
    void SetTargetAsset(Ptr<CAsset> _Asset);
    Ptr<CAsset> GetTargetAsset() { return m_TargetAsset; }

public:
    virtual void Tick() override;
    virtual void Render_Update() override;

private:
    void CreateComponentUI();
    void CreateAssetUI();

public:
    Inspector();
    ~Inspector();
};

