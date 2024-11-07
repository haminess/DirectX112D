#pragma once
#include "ComponentUI.h"

#include <Engine/CScript.h>

class ScriptUI :
    public ComponentUI
{
private:
    CScript* m_Script;
    
    void*   m_Value;
    CComponent*         m_SetInst;
    EUI_DELEGATE_1      m_SetterFunc;

public:
    void SetTargetScript(CScript* _Script) { m_Script = _Script; }

public:
    virtual void Render_Update() override;

private:
    void SelectTexture(DWORD_PTR _ListUI, DWORD_PTR _SelectString);

public:
    ScriptUI();
    ~ScriptUI();
};

