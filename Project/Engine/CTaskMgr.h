#pragma once
#include "singleton.h"
class CTaskMgr :
    public singleton<CTaskMgr>
{
    SINGLE(CTaskMgr);

private:
    vector<tTask>           m_vecTask;
    vector<CGameObject*>    m_vecGC;

    // 오브젝트 생성, 삭제 시점에 트리거
    bool                    m_LevelChanged;


public:
    void AddTask(const tTask& _task) { m_vecTask.push_back(_task); }
    bool IsLevelChanged() { return m_LevelChanged; }

public:
    void Tick();
};

