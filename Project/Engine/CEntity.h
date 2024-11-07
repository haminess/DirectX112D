#pragma once
class CEntity
{
private:
	static UINT g_NextID;

private:
	wstring m_Name;
	UINT m_ID;


public:
	void SetName(const wstring& _Name) { m_Name = _Name; }
	wstring GetName() { return m_Name; }
	UINT GetID() { return m_ID; }


	virtual void SaveToLevel(FILE* _File);
	virtual void LoadFromLevel(FILE* _File);


	// �θ� �����ͷ� ������Ʈ�� �����ϴ� ��찡 ����.
	// Clone�� virtual�� �������� Ư¡�� ��� �����ؾ� �Ѵ�.
	// Ŭ������ ������ �� ���� Clone �Լ��� ���� �����ڸ� �Ű��� ��
	virtual CEntity* Clone() = 0;

public:
	CEntity();
	CEntity(const CEntity& _Other);
	virtual ~CEntity();
};

