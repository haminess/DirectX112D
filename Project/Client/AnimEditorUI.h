
#pragma once
#include "EditorUI.h"
class CFlipbookPlayer;
class CFlipbook;

class AnimEditorUI :
    public EditorUI
{
private:
    float m_ScaleFactor;
    Vector2 m_PreviewPos;

    Ptr<CFlipbook>      m_CurAnim;
    CFlipbookPlayer*    m_PreviewPlayer;
    int m_CurSpriteIdx;
    //Ptr<CTexture>       m_CurAtlas;


public:
    virtual void Render() override;
    virtual void Render_Update() override;

public:
    void Load();
    void LoadFlipbook(DWORD_PTR _ListUI, DWORD_PTR _SelectString);
    void SelectSprite(DWORD_PTR _ListUI, DWORD_PTR _SelectString);
    void Save();


public:
    AnimEditorUI();
    ~AnimEditorUI();
};

