#include "pch.h"
#include "CRenderComponent.h"


CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
	: CComponent(_Type)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _Origin)
	: CComponent(_Origin)
	, m_Mesh(_Origin.m_Mesh)
	, m_SharedMtrl(_Origin.m_SharedMtrl)
{
	// ���� ��ü�� ���������� ���� ���
	if (nullptr == _Origin.m_DynamicMtrl)
	{
		m_CurMtrl = m_SharedMtrl;
	}

	// ���� ��ü�� ���� ������ �ִ� ���
	else
	{
		CreateDynamicMaterial();
	}
}

CRenderComponent::~CRenderComponent()
{
}


void CRenderComponent::SaveComponent(FILE* _File)
{
	SaveAssetRef(m_Mesh, _File);
	SaveAssetRef(m_SharedMtrl, _File);
	SaveAssetRef(m_CurMtrl, _File);
}

void CRenderComponent::LoadComponent(FILE* _File)
{
	LoadAssetRef(m_Mesh, _File);
	LoadAssetRef(m_SharedMtrl, _File);
	LoadAssetRef(m_CurMtrl, _File);
}



// ���� ������ ����ϴ� �Լ�
void CRenderComponent::SetMaterial(Ptr<CMaterial> _Material)
{
	// ���� ������Ʈ�� ���� ������ ���� ���� �� �ִ�.
	assert(!_Material->GetSharedMtrl().Get());

	m_CurMtrl = m_SharedMtrl = _Material;
}

Ptr<CMaterial> CRenderComponent::GetSharedMaterial()
{
	m_CurMtrl = m_SharedMtrl;
	return m_SharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	if (m_DynamicMtrl.Get())
	{
		m_CurMtrl = m_DynamicMtrl;
	}
	else
	{
		CreateDynamicMaterial();
	}

	return m_DynamicMtrl;
}

void CRenderComponent::CreateDynamicMaterial()
{
	assert(m_SharedMtrl.Get());
	m_CurMtrl = m_DynamicMtrl = m_SharedMtrl->Clone();
}

