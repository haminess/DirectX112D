#include "pch.h"
#include "CEngine.h"
#include "CDevice.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CAssetMgr.h"

#include "CLevelMgr.h"
#include "CRenderMgr.h"
#include "CCollisionMgr.h"
#include "CTaskMgr.h"

CEngine::CEngine()
	: m_hMainWnd(nullptr)
{
}

CEngine::~CEngine()
{
}

int CEngine::Init(HWND _hWnd, UINT _Width, UINT _Height)
{
	// 윈도우 초기화
	m_hMainWnd = _hWnd;


	// 해상도 초기화
	m_Resolution.x = (float)_Width;
	m_Resolution.y = (float)_Height;

	RECT rt = { 0, 0, (int)m_Resolution.x, (int)m_Resolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, !!GetMenu(m_hMainWnd));

	SetWindowPos(m_hMainWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);

	// Device 초기화
	if (FAILED(CDevice::GetInst()->Init(m_hMainWnd, m_Resolution)))
	{
		return E_FAIL;
	}

	// Manager 초기화
	CPathMgr::GetInst()->Init();
	CKeyMgr::GetInst()->Init();
	CTimeMgr::GetInst()->Init();
	CAssetMgr::GetInst()->Init();
	CRenderMgr::GetInst()->Init();
	CLevelMgr::GetInst()->Init();

	return S_OK;
}

void CEngine::Progress()
{
	// Engine Tick
	CKeyMgr::GetInst()->Tick();
	CTimeMgr::GetInst()->Tick();
	CLevelMgr::GetInst()->Progress();
	CCollisionMgr::GetInst()->Tick();

	// Engine Render
	CRenderMgr::GetInst()->Render();

	// Task
	CTaskMgr::GetInst()->Tick();
}
