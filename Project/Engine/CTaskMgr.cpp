#include "pch.h"
#include "CTaskMgr.h"
#include "CGameObject.h"
#include "CLevel.h"
#include "CLevelMgr.h"
#include "CAssetMgr.h"

CTaskMgr::CTaskMgr()
	: m_LevelChanged(false)
{}
CTaskMgr::~CTaskMgr()
{}

void CTaskMgr::Tick()
{
	m_LevelChanged = false;

	for (size_t i = 0; i < m_vecGC.size(); ++i)
	{
		delete m_vecGC[i];
		m_LevelChanged = true;
	}
	m_vecGC.clear();


	for (size_t i = 0; i < m_vecTask.size(); ++i)
	{
		const tTask& task = m_vecTask[i];
		switch (m_vecTask[i].Type)
		{
		case TASK_TYPE::ADD_CHILD:
		{
			CGameObject* pParent = (CGameObject*)task.Param0;
			CGameObject* pChild = (CGameObject*)task.Param1;
			CGameObject* pPrevParent = pChild->GetParent();

			// 레벨 오브젝트 리스트에도 반영해야 한다.

			// 이동할 부모가 없을 때
			if (nullptr == pParent)
			{
				if (pPrevParent)
				{
					pChild->DisconnectWithParent();
					pChild->RegisterAsParent();
				}
			}

			// 이동할 부모가 있을 때
			else
			{
				// 움직일 오브젝트가 자식 오브젝트이면
				if (pPrevParent)
				{
					pChild->DisconnectWithParent();
				}

				// 움직일 오브젝트가 최상위 부모 오브젝트이면
				else
				{
					int LayerIdx = pChild->GetLayerIdx();
					pChild->DisconnectWithLayer();
					pChild->m_LayerIdx = LayerIdx;
				}

				pParent->AddChild(pChild);

			}

			m_LevelChanged = true;
		}
			break;
		case TASK_TYPE::CHANGE_LEVEL:
		{
			CLevel* pNextLevel = (CLevel*)task.Param0;

			LEVEL_STATE eNextState = (LEVEL_STATE)task.Param1;

			CLevelMgr::GetInst()->ChangeLevel(pNextLevel, eNextState);

			m_LevelChanged = true;
		}
		break;
		case TASK_TYPE::CREATE_OBJECT:
		{
			CGameObject* pNewObject = (CGameObject*)task.Param0;
			int LayerIdx = (int)task.Param1;

			CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			pCurLevel->AddObject(LayerIdx, pNewObject, false);

			m_LevelChanged = true;

			// 레벨 플레이가 아닐 때 생성된 오브젝트는 Play 시작할 때 Begin 호출되어야 한다.
			// 레벨 도중에 생성된 오브젝트는 Begin 호출되어야 한다.
			if (pCurLevel->GetState() == LEVEL_STATE::PLAY || pCurLevel->GetState() == LEVEL_STATE::PAUSE)
			{
				pNewObject->Begin();
			}
		}
		break;
		case TASK_TYPE::DELETE_OBJECT:
		{
			CGameObject* pObject = (CGameObject*)task.Param0;
			if (!pObject->IsDead())
			{
				pObject->m_Dead = true;
				m_vecGC.push_back(pObject);
			}

		}
		break;
		case TASK_TYPE::CHANGE_LEVEL_STATE:
		{
			LEVEL_STATE eNextState = (LEVEL_STATE)task.Param0;
			CLevelMgr::GetInst()->ChangeLevelState(eNextState);
		}
		break;
		}
	}

	m_vecTask.clear();
}
