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

    Ptr<CSprite> GetCurrentSprite() { return m_CurFlipbook->GetSprite(m_SpriteIdx); }
    Ptr<CFlipbook> GetCurFlipbook() { return m_CurFlipbook; }
    Ptr<CFlipbook> GetFlipbook(int _idx) { return m_vecFlipbook[_idx]; }

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

