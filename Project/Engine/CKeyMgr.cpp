#include "pch.h"
#include "CKeyMgr.h"
#include "CEngine.h"
#include "CCamera.h"
#include "CLevelMgr.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CTaskMgr.h"

int g_arrKeyValue[(int)KEY::END]
=
{
	// 0
	'W', 'S', 'A', 'D',
	'T', 'G', 'F', 'H',
	'I', 'K', 'J', 'L',
	'Q', 'E', 'R', 'Y',
	'Z', 'X', 'C', 'V',
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 189, 187,

	// 4
	VK_UP,
	VK_DOWN,
	VK_LEFT,
	VK_RIGHT,

	// 8
	VK_SPACE,
	VK_RETURN,

	// 10
	VK_NUMPAD0,
	VK_NUMPAD1,
	VK_NUMPAD2,
	VK_NUMPAD3,
	VK_NUMPAD4,
	VK_NUMPAD5,
	VK_NUMPAD6,
	VK_NUMPAD7,
	VK_NUMPAD8,
	VK_NUMPAD9,

	VK_F1,
	VK_F2,
	VK_F3,
	VK_F4,
	VK_F5,
	VK_F6,
	VK_F7,
	VK_F8,
	VK_F9,
	VK_F10,
	VK_F11,
	VK_F12,

	// 20
	VK_LBUTTON,
	VK_RBUTTON,
	VK_DELETE,
	VK_LSHIFT,
	VK_RSHIFT,
	VK_CONTROL,
	VK_ESCAPE,
	VK_TAB,
	VK_MBUTTON,
	WM_SYSKEYDOWN,
	WM_SYSKEYDOWN,
	WM_SYSKEYDOWN,
};


CKeyMgr::CKeyMgr()
{

}
CKeyMgr::~CKeyMgr()
{

}
void CKeyMgr::Init()
{
	for (int i = 0; i < (int)KEY::END; ++i)
	{
		m_vecKey.push_back(tKeyInfo{KEY_STATE::NONE, false});
	}
}
void CKeyMgr::Tick()
{
	// ���� �����찡 ��Ŀ�� ���� ���� Ű ���� ����� �����ϵ��� �Ѵ�.
	// **GetFocus �޼���
	// ���� ��Ŀ�� ���� ������ �ڵ��� ��ȯ�Ѵ�.

	// ����
	// ��Ŀ�� ���� �����찡 �ִ� �߿���
	if (GetFocus())
	{
		for (int i = 0; i < (int)KEY::END; ++i)
		{
			// Ű ����
			if (GetAsyncKeyState(g_arrKeyValue[i]) && 0x8001 || m_vecKey[i].NextPressed)
			{
				// Ű�� �����ְ�, �������� �������� �ʾҴ�.
				if (false == m_vecKey[i].PrevPressed)
				{
					m_vecKey[i].State = KEY_STATE::TAP;
				}

				// Ű�� �����ְ�, �������� �����־���.
				else
				{
					m_vecKey[i].State = KEY_STATE::PRESSED;
				}
				m_vecKey[i].PrevPressed = true;
				m_vecKey[i].NextPressed = false;
			}

			// Ű �ȴ���
			else
			{
				// Ű�� �����ִٰ� ���ʷ� �ô�.
				if (m_vecKey[i].PrevPressed)
				{
					m_vecKey[i].State = KEY_STATE::RELEASED;
				}

				// �ƹ� ���� �ƴ�
				else
				{
					m_vecKey[i].State = KEY_STATE::NONE;
				}

				m_vecKey[i].PrevPressed = false;
			}
		}

		m_MousePrevPos = m_MousePos;

		// ���콺 ��ǥ ����
		POINT Pos;

		// ����� ��ǥ ��ȯ �޼���
		GetCursorPos(&Pos);

		// ����� ��ǥ���� ������ ��ǥ ��ȯ �޼���
		ScreenToClient(CEngine::GetInst()->GetMainWnd(), &Pos);
		m_MousePos = Vector2((float)Pos.x, (float)Pos.y);
		m_MouseDir = m_MousePos - m_MousePrevPos;
		
		CCamera* pCam = CRenderMgr::GetInst()->GetRenderCamera();
		if (pCam && !CTaskMgr::GetInst()->IsLevelChanged())
		{
			Vector3 vCamPos = pCam->Transform()->GetWorldPos();
			Vector2 vResol = CDevice::GetInst()->GetRenderResolution() / 2;
			m_MouseWorldPos = Vector2(vCamPos.x + m_MousePos.x - vResol.x, vCamPos.y - m_MousePos.y + vResol.y);
		}

		g_Data.MousePos = m_MousePos;
		g_Data.MouseWorldPos = m_MouseWorldPos;

		CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::GLOBAL);
	}

	// ��Ŀ�� ���� �����찡 ���� ���
	else
	{
		for (int i = 0; i < (int)KEY::END; ++i)
		{
			if (KEY_STATE::TAP == m_vecKey[i].State || KEY_STATE::PRESSED == m_vecKey[i].State)
			{
				m_vecKey[i].State = KEY_STATE::PRESSED;
			}

			else if (KEY_STATE::RELEASED == m_vecKey[i].State)
			{
				m_vecKey[i].State = KEY_STATE::NONE;
			}

			m_vecKey[i].PrevPressed = false;
		}
	}

}
