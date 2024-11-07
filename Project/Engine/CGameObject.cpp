#include "pch.h"
#include "CGameObject.h"
#include "components.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CScript.h"
#include "CRenderComponent.h"
#include "CLevelMgr.h"
#include "CLayer.h"
#include "CLevel.h"


CGameObject::CGameObject()
	: m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_LayerIdx(-1)
	, m_Active(true)
	, m_Dead(false)
{
	// Transform ������Ʈ�� ������ ������ �Ǵ� �⺻ ������Ʈ
	AddComponent(new CTransform);
}

CGameObject::CGameObject(const CGameObject& _Origin)
	: CEntity(_Origin)
	, m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_LayerIdx(-1)
	, m_Active(true)
	, m_Dead(false)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == _Origin.m_arrCom[i])
			continue;

		AddComponent(_Origin.m_arrCom[i]->Clone());
	}

	for (size_t i = 0; i < _Origin.m_vecScripts.size(); ++i)
	{
		AddComponent(_Origin.m_vecScripts[i]->Clone());
	}

	for (size_t i = 0; i < _Origin.m_vecChild.size(); ++i)
	{
		AddChild(_Origin.m_vecChild[i]->Clone());
	}
}

CGameObject::~CGameObject()
{
	// ���� ����
	//DeleteArray<CComponent>(m_arrCom);
	DeleteArray(m_arrCom);
	DeleteVec(m_vecScripts);
	DeleteVec(m_vecChild);
}


void CGameObject::Begin()
{
	if (!m_Active)
		return;

	// component
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (!m_arrCom[i])
			continue;

		m_arrCom[i]->Begin();
	}

	// script component
	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		m_vecScripts[i]->Begin();
	}

	for (UINT i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Begin();
	}
}

void CGameObject::Tick()
{
	if (!m_Active)
		return;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (!m_arrCom[i])
			continue;

		m_arrCom[i]->Tick();
	}

	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		m_vecScripts[i]->Tick();
	}

	for (UINT i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Tick();
	}
}

void CGameObject::FinalTick()
{
	if (!m_Active)
		return;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (!m_arrCom[i])
			continue;

		m_arrCom[i]->FinalTick();
	}

	vector<CGameObject*>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end(); )
	{
		(*iter)->FinalTick();

		if ((*iter)->IsDead())
		{
			iter = m_vecChild.erase(iter);
		}
		else
		{
			++iter;
		}
	}


	// Level ���
	CLevelMgr::GetInst()->RegisterObject(this);
	
}

void CGameObject::Render()
{
	if (!m_Active)
		return;

	m_RenderCom->Render();
}


void CGameObject::SetActive(bool _bActive)
{
	if (m_Active == _bActive)
		return;

	m_Active = _bActive;

	//if (m_Active)
	//	Activate();
	//else
	//	Deactivate();
}

void CGameObject::AddComponent(CComponent* _Component)
{
	COMPONENT_TYPE Type = _Component->GetType();

	if (COMPONENT_TYPE::SCRIPT == Type)
	{
		m_vecScripts.push_back((CScript*)_Component);
	}
	else
	{
		// �Է����� ������ ������Ʈ�� �̹� ������ ������Ʈ�� ������Ʈ�� ������ �ִ� ���
		assert(!m_arrCom[(UINT)Type]);

		// �Է����� ������ ������Ʈ�� �������� ���õ� ������Ʈ�̸� 1���� ���� �� �ֵ��� ����ó�� �Ѵ�.
		if (dynamic_cast<CRenderComponent*>(_Component))
		{
			assert(!m_RenderCom);

			m_RenderCom = (CRenderComponent*)_Component;
		}


		// �Էµ� ������Ʈ�� �ּҸ� ����
		m_arrCom[(UINT)Type] = _Component;
	}

	// ������Ʈ�� ����������Ʈ�� ����
	_Component->m_Owner = this;

	// ������Ʈ �ʱ�ȭ
	_Component->Init();
}

void CGameObject::AddChild(CGameObject* _Child)
{
	m_vecChild.push_back(_Child);
	_Child->m_Parent = this;
}

bool CGameObject::IsAncestor(CGameObject* _Other)
{
	CGameObject* pParent = m_Parent;

	while (pParent)
	{
		if (pParent == _Other)
			return true;

		pParent = pParent->m_Parent;
	}

	return false;
}
void CGameObject::DisconnectWithLayer()
{
	// �Ҽ� ���̾ ���ٸ�
	if (-1 == m_LayerIdx)
		return;

	CLayer* pLayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(m_LayerIdx);
	pLayer->DisconnectObject(this);
}

void CGameObject::DisconnectWithParent()
{
	if (nullptr == m_Parent)
		return;

	vector<CGameObject*>::iterator iter = m_Parent->m_vecChild.begin();

	for (; iter != m_Parent->m_vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			m_Parent->m_vecChild.erase(iter);
			m_Parent = nullptr;
			return;
		}
	}

	assert(nullptr);
}

void CGameObject::RegisterAsParent()
{
	// �Ҽ� ���̾ ���ٸ�
	if (-1 == m_LayerIdx)
		return;

	CLayer* pLayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(m_LayerIdx);
	pLayer->RegisterAsParent(this);

}

void CGameObject::SetLayerIdx(int _idx)
{
	m_LayerIdx = _idx;
}