#pragma once
#include "CCollider2D.h"
class CLineCollider2D :
	public CCollider2D
{
private:
	float m_Slope;          // ����
	Vector2 m_StartPos;     // ���� ��ġ
	Vector2 m_EndPos;       // �� ��ġ

public:
	float   GetSlope() { return m_Slope; }
	Vector2 GetStartPos() { return m_StartPos; }
	Vector2 GetEndPos() { return m_EndPos; }

	void	SetSlope(float   _Slope) { m_Slope = _Slope; }
	void	SetStartPos(Vector2 _StartPos) { m_StartPos = _StartPos; }
	void	SetEndPos(Vector2 _EndPos) { m_EndPos = _EndPos; }

public:
	virtual void FinalTick() override;

public:
	CLineCollider2D();
	~CLineCollider2D();
};

