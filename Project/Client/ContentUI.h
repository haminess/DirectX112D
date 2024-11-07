#pragma once
#include "EditorUI.h"

class ContentUI :
    public EditorUI
{
private:
    class TreeUI* m_Tree;

public:
    virtual void Render_Update() override;

private:
    void RenewContent();
    void SelectAsset(DWORD_PTR _TreeNode);


public:
    ContentUI();
    ~ContentUI();
};

