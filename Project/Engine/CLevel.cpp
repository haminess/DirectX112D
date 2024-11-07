#include "pch.h"
#include "CLevel.h"
#include "CGameObject.h"

CLevel::CLevel()
	: m_State(LEVEL_STATE::NONE)
	, m_arrLayer{}
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].m_LayerIdx = i;
	}
}

CLevel::~CLevel()
{
}


void CLevel::Begin()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].Begin();
	}
}

void CLevel::Tick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].Tick();
	}
}

void CLevel::FinalTick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].FinalTick();
	}
}


void CLevel::AddObject(int _LayerIdx, CGameObject* _Object, bool _MoveWithChild)
{
	m_arrLayer[_LayerIdx].AddObject(_Object, _MoveWithChild);
	m_vecParents.push_back(_Object);
}

CGameObject* CLevel::FindObjectByName(const wstring& _Name)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecParents = m_arrLayer[i].GetParentObjects();

		static list<CGameObject*> queue;

		for (size_t j = 0; j < vecParents.size(); ++j)
		{
			queue.clear();
			queue.push_back(vecParents[j]);

			while (!queue.empty())
			{
				CGameObject* pObject = queue.front();
				queue.pop_front();

				if (pObject->GetName() == _Name)
				{
					return pObject;
				}

				const vector<CGameObject*>& vecChild = pObject->GetChild();
				for (size_t k = 0; k < vecChild.size(); ++k)
				{
					queue.push_back(vecChild[k]);
				}
			}
		}
	}

	return nullptr;
}

void CLevel::ChangeState(LEVEL_STATE _NextState)
{
	m_State = _NextState;
}