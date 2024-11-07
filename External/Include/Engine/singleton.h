#pragma once


typedef void(*EXIT)(void);

template<typename T>
class singleton
{
private:
	static T* m_Inst;

public:
	static T* GetInst()
	{
		// �̱��� 1. ���� ���� ���� ���
		// ����: �Ҹ꿡 �Ű澵 �ʿ� ���� ����
		// ����: �׻� �޸� ����
		//static CEngine engine;
		//return &engine;


		// �̱��� 2. Ŭ���� ���� ������ ���� ���
		// ����: �ʿ��� �� �Ҵ�, �Ҹ� ����
		// ����: ���� �Ҹ����־�� �Ѵ�.
		if (nullptr == m_Inst)
		{
			m_Inst = new T;

			// ���� �Լ��� ����� �� ���������� ȣ���ų �Լ� ���
			// => �� �̱��� ������ ������ Ȱ�� ����
			atexit((EXIT)&singleton<T>::Destroy);
		}

		return m_Inst;
	}

	static void Destroy()
	{
		if (nullptr != m_Inst)
		{
			delete m_Inst;
			m_Inst = nullptr;
		}
	}
};

template<typename T>
T* singleton<T>::m_Inst = nullptr;


#define SINGLE(TYPE) private:\
						TYPE();\
						TYPE(const TYPE& _Other) = delete;\
						~TYPE();\
						TYPE& operator =(const TYPE& _Other) = delete;\
						friend class singleton<TYPE>;