#pragma once
#include "CComponent.h"

#include "CFlipbook.h"

class CFlipbookPlayer :
    public CComponent
{
private:
    vector<Ptr<CFlipbook>>  m_vecFlipbook;      // ��� ��� Flipbook
    Ptr<CFlipbook>          m_CurFlipbook;      // ������� Flipbook
    int                     m_SpriteIdx;        // ������� Sprite �ε���
    bool                    m_Repeat;           // �ݺ����
    float                   m_FPS;              // ����ӵ�
    float                   m_Time;             // ���� �ð�
    bool                    m_Finish;           // ��� ȸ���� ������ ��


public:
    void AddFlipbook(int _idx, Ptr<CFlipbook> _Flipbook);
    void Play(int _Idx, float _FPS, bool _Repeat)
    {
        m_CurFlipbook = m_vecFlipbook[_Idx];
        m_FPS = _FPS;
        m_Repeat = _Repeat;
        m_Time = 0.f;
        m_SpriteIdx = 0;
    }
    void Stop()
    {
        m_CurFlipbook = nullptr;
        m_FPS = 0.f;
        m_Repeat = false;
        m_Time = 0.f;
        m_SpriteIdx = 0;
    }
    void Pause()
    {
        m_Repeat = false;
        m_Finish = true;
    }

    Ptr<CSprite> GetCurrentSprite() { return m_CurFlipbook->GetSprite(m_SpriteIdx); }
    Ptr<CFlipbook> GetCurFlipbook() { return m_CurFlipbook; }
    Ptr<CFlipbook> GetFlipbook(int _idx) { return m_vecFlipbook[_idx]; }
    float GetFPS() { return m_FPS; }
    bool IsLoop() { return m_Repeat; }

    void SetLoop(bool _Loop) { m_Repeat = _Loop; }
    void SetCurSpriteIdx(int _Idx) { m_SpriteIdx = _Idx; }

    void Binding();
    void Clear();

public:
    virtual void FinalTick() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

public:
    CLONE(CFlipbookPlayer);
    CFlipbookPlayer();
    ~CFlipbookPlayer();
};

