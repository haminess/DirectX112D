#pragma once
#include "CEntity.h"
#include "CLayer.h"

class CLayer;
class CGameObject;

class CLevel :
	public CEntity
{
private:
	LEVEL_STATE	m_State;
	CLayer m_arrLayer[MAX_LAYER];
	list<CGameObject*> m_vecParents;

public:
	CLayer* GetLayer(int _LayerIdx) { return &m_arrLayer[_LayerIdx]; }
	CGameObject* FindObjectByName(const wstring& _Name);
	list<CGameObject*> GetParentObjects() { return m_vecParents; }
	LEVEL_STATE GetState() { return m_State; }

	void RegisterClear()
	{
		// ���������ӿ� ��Ϲ޾Ҵ� ������Ʈ ����� Clear �Ѵ�.
		// Ŭ�������� ������ regist object�Ǹ鼭
		// ������ ������Ʈ���� �����Ӹ��� ��ø�� ��
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			m_arrLayer[i].m_vecObjects.clear();
		}
	}

public:
	void Begin();
	void Tick();
	void FinalTick();

public:
	void AddObject(int _LayerIdx, CGameObject* _Object, bool _MoveWithChild);

private:
	void ChangeState(LEVEL_STATE _NextState);


public:
	virtual CLevel* Clone() { return new CLevel(*this); }
	CLevel();
	~CLevel();

	friend class CLevelMgr;
};

