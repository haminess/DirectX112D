#pragma once

class CTimeMgr
	: public singleton<CTimeMgr>
{
	SINGLE(CTimeMgr);

	// �ð�
	// ���� ������ 1�ʴ� 13�� ������
	// �� ������ �� �ɸ��� �ð� 13������ 1��
	// GetTickCount, 1�ʿ� 1000�� ī��Ʈ
	// GetTickCount �ѹ� ī��Ʈ�� �� 130�� ������ ī��Ʈ
	// ���� GetTickCount�δ� 0.001�� ���� �������� �� ����

	// 1. �ð� ����ȭ�� ���� DT(DeltaTime) �� ������Ʈ�ϰ� ����

private:
	// system time
	LARGE_INTEGER	m_Frequency;	// 1�ʴ� ī���� ���� ��
	LARGE_INTEGER	m_CurrentCount;	// ���� ī����
	LARGE_INTEGER	m_PrevCount;	// ���� ������ ī����

	// game time
	// Delta Time
	// �̼��� �ð���, �� �����Ӵ� �ð�
	// �׻� �޶��� �� �ִ�.
	float			m_fDT;			// DeltaTime : 1�����ӿ� �ɸ� �ð�
	float			m_Time;			// ���� �ð�

	float			m_fEngineDT;
	float			m_fEngineTime;

	double			m_Second;	// 1�� üũ �뵵 �����ð�

	UINT			m_FPS;

	bool			m_IsStop;


public:
	void Init();
	void Tick();
	void SetStopMode(bool _IsStop)
	{
		m_IsStop = _IsStop;
		if (m_IsStop)
			m_fDT = 0.f;
	}

public:
	float GetDeltaTime() { return m_fDT; }
	float GetEngineDeltaTime() { return m_fEngineDT; }
	float GetTime() { return m_fEngineDT; }
};

