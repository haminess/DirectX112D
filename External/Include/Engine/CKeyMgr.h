#pragma once

enum class KEY
{
	// key
	W, S, A, D, 
	T, G, F, H, 
	I, K, J, L, 
	Q, E, R, Y,
	Z, X, C, V,
	N1, N2, N3, N4, N5, N6, N7, N8, N9, N0, MINUS, EQUAL,

	UP, DOWN,
	LEFT, RIGHT,

	SPACE, ENTER,

	NUM0, NUM1, NUM2, NUM3, NUM4, 
	NUM5, NUM6, NUM7, NUM8, NUM9,

	F1,	F2,	F3,	F4,
	F5,	F6,	F7,	F8,
	F9,	F10, F11, F12,

	// mouse
	LBTN, RBTN,
	DEL, LSHIFT, RSHIFT, CTRL, ESC, TAB,
	MBTN, WHEEL_UP, WHEEL_DOWN,
	ANY,

	END
};

enum class KEY_STATE
{
	TAP,		// ���� �� ����
	PRESSED,	// ��� �����ִ� ����
	RELEASED,	// Ű �� ���� ����
	NONE,
};

struct tKeyInfo
{
	KEY_STATE	State;			// Ű ����
	bool		PrevPressed;	// �������� ���� ���� �־�����
	bool		NextPressed;	// ���� �����ӿ� ������ Ʈ���� �� ��
};

class CKeyMgr
	: public singleton<CKeyMgr>
{
	// Ű ����ȭ �ϱ� ����
	// Ű ����ȭ �ϴ� ����?

	SINGLE(CKeyMgr);

private:
	vector<tKeyInfo>	m_vecKey;

	Vector2				m_MousePos;
	Vector2				m_MousePrevPos;
	Vector2				m_MouseDir;

	Vector2				m_MouseWorldPos;

public:
	Vector2 GetMousePos() { return m_MousePos; }
	Vector2 GetMouseWorldPos() { return m_MouseWorldPos; }
	void SetMouseWorldPos(Vector2 _Pos) { m_MouseWorldPos = _Pos; }
	Vector2 GetMouseDir() { return m_MouseDir; }
	void UpdateKeyState(KEY _Key) { m_vecKey[(int)_Key].NextPressed = true; }
	
public:
	void Init();
	void Tick();

	KEY_STATE GetKeyState(KEY _Key) { return m_vecKey[(int)_Key].State; }
	bool IsThisKeyPressed(KEY _Key) { return m_vecKey[(int)_Key].PrevPressed; }
};

