#pragma once
#include "EditorUI.h"
class CTexture;
class CSprite;

class SpriteEditorUI :
    public EditorUI
{
private:
    float   m_ScaleFactor;
    Vector2 m_AtlasPos;

    int m_CurAtlasNum;
    Ptr<CTexture>   m_CurAtlas;
    vector<CSprite> m_EditSprites;
    Ptr<CSprite>    m_CurSprite;

    // Edit
    bool m_OnCreate;
    Vector2 m_SliceInput;
    

public:
    virtual void Render() override;
    virtual void Render_Update() override;

public:
    void Load(Ptr<CTexture> _Tex);
    void Save();

private:
    void SliceByCount(Vector2 _Count);
    void SliceBySize(Vector2 _Size);
    void CreateSpriteByEditor(Vector2 _WinPos, Vector2 _WinScale);
    void CreateSpriteByPixel(Vector2 _Pos, Vector2 _Scale);
    void DeleteSprite(Ptr<CSprite> _DelSprite);

public:
    SpriteEditorUI();
    ~SpriteEditorUI();
};

