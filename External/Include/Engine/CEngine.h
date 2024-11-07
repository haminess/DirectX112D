#pragma once

class CEngine
	: public singleton<CEngine>
{
	SINGLE(CEngine)
private:
	HWND m_hMainWnd;
	Vector2 m_Resolution;

public:
	HWND GetMainWnd() { return m_hMainWnd; }

public:
	int Init(HWND _hWnd, UINT _Width, UINT _Height);
	void Progress();
};

