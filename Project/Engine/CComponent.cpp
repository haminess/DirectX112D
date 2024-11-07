#include "pch.h"
#include "CComponent.h"

CComponent::CComponent(COMPONENT_TYPE _Type)
	: m_Type(_Type)
{
}

CComponent::~CComponent()
{
}

void CComponent::SaveToLevel(FILE* _File)
{
	CEntity::SaveToLevel(_File);

	SaveComponent(_File);
}

void CComponent::LoadFromLevel(FILE* _File)
{
	CEntity::LoadFromLevel(_File);

	LoadComponent(_File);
}