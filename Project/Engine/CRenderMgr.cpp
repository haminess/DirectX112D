#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CCamera.h"
#include "CTransform.h"
#include "CLight2D.h"

#include "CStructuredBuffer.h"

#include "CMeshRender.h"
#include "CAssetMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CTaskMgr.h"

CRenderMgr::CRenderMgr()
	: m_DbgObj(nullptr)
	, m_Light2DBuffer(nullptr)
	, m_EditorCam(nullptr)
	, m_CamNum(0)
	, m_IsEditor(false)
{
	m_Light2DBuffer = new CStructuredBuffer;
	m_Light2DBuffer->Create(sizeof(tLight2DInfo), 2, SB_TYPE::SRV_ONLY, true);
}

CRenderMgr::~CRenderMgr()
{
	DELETE(m_DbgObj);
	DELETE(m_Light2DBuffer);
}

void CRenderMgr::Init()
{
	m_DbgObj = new CGameObject;
	m_DbgObj->SetName(L"DebugObject");
	m_DbgObj->AddComponent(new CMeshRender);
	m_DbgObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl"));

	Vector2 vResolution = CDevice::GetInst()->GetRenderResolution();
	m_PostProcessTex = CAssetMgr::GetInst()->CreateTexture(L"PostProcessTex"
		, vResolution.x, vResolution.y
		, DXGI_FORMAT_R8G8B8A8_UNORM
		, D3D11_BIND_SHADER_RESOURCE);
}

void CRenderMgr::Render()
{
	// ������ ����
	RenderStart();

	if (m_IsEditor)
	{
		Render_Editor();
	}
	else
	{
		Render_Play();
	}

	// DebugRender
	Render_Debug();
}

void CRenderMgr::RenderStart()
{
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");

	// Target Clear
	Vector4 vClearColor = Vector4(0.f, 0.f, 0.f, 0.f);
	CONTEXT->ClearRenderTargetView(pRTTex->GetRTV().Get(), vClearColor);
	CONTEXT->ClearDepthStencilView(pDSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	Vector2 vResolution = CDevice::GetInst()->GetRenderResolution();

	// ViewPort ���� - Window ȭ�� ���� ����
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	viewport.Width = vResolution.x;
	viewport.Height = vResolution.y;

	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

	CONTEXT->RSSetViewports(1, &viewport);

	// Rendering �������� ����
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());


	// ������ �� ���ҽ� ���ε�
	Binding();
}

void CRenderMgr::Binding()
{
	static CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::GLOBAL);

	g_Data.Light2DCount = (int)m_vecLight2D.size();

	pCB->SetData(&g_Data);
	pCB->Binding();
	pCB->Binding_CS();

	// 2D ���� ���� ���ε�
	static vector<tLight2DInfo> vecLight2DInfo;
	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecLight2DInfo.push_back(m_vecLight2D[i]->GetLight2DInfo());
	}

	// �����͸� ���� ����ȭ������ ũ�Ⱑ ���ڶ��, Resize �Ѵ�.
	if (m_Light2DBuffer->GetElementCount() < vecLight2DInfo.size())
	{
		m_Light2DBuffer->Create(sizeof(tLight2DInfo), (UINT)vecLight2DInfo.size(), SB_TYPE::SRV_ONLY, true);
	}

	// ������ 1�� �̻��� ��쿡�� ����ȭ ���� �����ͷ� �̵�
	if (!m_vecLight2D.empty())
	{
		m_Light2DBuffer->SetData(vecLight2DInfo.data(), vecLight2DInfo.size());
		m_Light2DBuffer->Binding(13);
	}

	m_vecLight2D.clear();
	vecLight2DInfo.clear();
}

void CRenderMgr::Render_Play()
{
	// ī�޶�� ���� ������
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		if (nullptr == m_vecCam[i])
			continue;

		m_vecCam[i]->SortObject();
		m_vecCam[i]->Render();

		// ���� ������ ���� ī�޶� ���
		m_RenderCam = m_vecCam[i];
		break;
	}
}

void CRenderMgr::Render_Editor()
{
	m_EditorCam->SortObject();
	m_EditorCam->Render();
}

void CRenderMgr::Render_Debug()
{
	list<tDebugShapeInfo>::iterator iter = m_DbgList.begin();

	for (; iter != m_DbgList.end(); )
	{
		// ��� ����
		switch ((*iter).Shape)
		{
		case DEBUG_SHAPE::RECT:
			m_DbgObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug"));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_DbgObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug"));
			break;
		case DEBUG_SHAPE::CROSS:
			break;
		case DEBUG_SHAPE::LINE:
			m_DbgObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"LineMesh"));
			break;
		}

		// ��ġ����
		if ((*iter).matWorld == XMMatrixIdentity())
		{
			m_DbgObj->Transform()->SetRelativePos((*iter).WorldPos);
			m_DbgObj->Transform()->SetRelativeScale((*iter).Scale);
			m_DbgObj->Transform()->SetRelativeRotation((*iter).Rotation);
			m_DbgObj->Transform()->FinalTick();
		}
		else
		{
			m_DbgObj->Transform()->SetWorldMat((*iter).matWorld);
		}


		// ���� ����
		m_DbgObj->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, (*iter).Color);

		if ((*iter).DepthTest)
		{
			m_DbgObj->MeshRender()->GetMaterial()->GetShader()->SetDSState(DS_TYPE::NO_WRITE);
		}
		else
		{
			m_DbgObj->MeshRender()->GetMaterial()->GetShader()->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
		}

		// ����
		m_DbgObj->Render();

		(*iter).Time += DT;
		if ((*iter).Duration <= (*iter).Time)
		{
			iter = m_DbgList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, UINT _Priority)
{
	if (-1 == _Priority)
	{
		vector<CCamera*>::iterator iter = m_vecCam.begin();
		for (; iter != m_vecCam.end(); ++iter)
		{
			if (*iter == _Cam)
			{
				m_vecCam.erase(iter);
				return;
			}
		}
	}

	else
	{
		if (m_vecCam.size() <= _Priority)
		{
			m_vecCam.resize(_Priority + 1);
		}

		CCamera* pOther = m_vecCam[_Priority];
		int iOther = _Cam->GetCameraPriority();
		if (-1 != iOther)
		{
			m_vecCam[iOther] = pOther;
		}

		m_vecCam[_Priority] = _Cam;
	}
}

void CRenderMgr::CopyRenderTarget()
{
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	CONTEXT->CopyResource(m_PostProcessTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}
