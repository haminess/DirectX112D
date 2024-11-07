#include "pch.h"
#include "CMeshRender.h"

#include "assets.h"
#include "CTransform.h"
#include "CFlipbookPlayer.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::Render()
{
	if (FlipbookPlayer())
	{
		FlipbookPlayer()->Binding();
	}

	// ��ġ����
	Transform()->Binding();

	// ����� ���̴�
	GetMaterial()->Binding();

	// ������
	GetMesh()->Render();

	if (FlipbookPlayer())
	{
		FlipbookPlayer()->Clear();
	}
}

void CMeshRender::FinalTick()
{
}

void CMeshRender::LoadComponent(FILE* _FILE)
{
	int a = 0;
	CRenderComponent::LoadComponent(_FILE);
}
