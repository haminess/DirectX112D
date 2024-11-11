#pragma once
#include "EditorUI.h"
#include "ListUI.h"
class CTexture;

struct tSliceInfo
{

};

class SpriteEditorUI :
    public EditorUI
{
private:
    float   m_ScaleFactor;
    Vector2 m_AtlasPos;

    Ptr<CTexture>   m_CurAtlas;

public:
    virtual void Render() override;
    virtual void Render_Update() override;

public:
    void Load(Ptr<CTexture> _Tex);
    void Save(Ptr<CTexture> _Tex);

private:
    void Slice();
    void CreateSprite();

public:
    SpriteEditorUI();
    ~SpriteEditorUI();
};

