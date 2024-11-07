#include "pch.h"
#include "CLight2D.h"
#include "CRenderMgr.h"
#include "CTransform.h"

CLight2D::CLight2D()
	: CComponent(COMPONENT_TYPE::LIGHT2D)
	, m_LightInfo{}
{
	m_LightInfo.Type = (int)LIGHT_TYPE::DIRECTIONAL;
}

CLight2D::~CLight2D()
{

}



void CLight2D::SaveComponent(FILE* _File)
{
	fwrite(&m_LightInfo, sizeof(tLight2DInfo), 1, _File);
}

void CLight2D::LoadComponent(FILE* _File)
{
	fread(&m_LightInfo, sizeof(tLight2DInfo), 1, _File);
}



void CLight2D::FinalTick()
{
	// ��ġ ���� ����
	m_LightInfo.vWorldPos = Transform()->GetWorldPos();
	m_LightInfo.vDir = Transform()->GetWorldDir(DIR_TYPE::RIGHT);

	// ������ ȭ�� �ȿ� �ִ��� ���͸�
	// RenderMgr�� Light ���
	CRenderMgr::GetInst()->RegisterLight2D(this);
}
