#include "pch.h"
#include "CPathMgr.h"

CPathMgr::CPathMgr()
{

}
CPathMgr::~CPathMgr()
{

}

void CPathMgr::Init()
{
	// ���� �۾� ���͸��� �˾Ƴ���. (�����Ų *.exe ���� ��ġ OR ������Ʈ �۾����͸� ��ġ)
	
	// ���� ���� ��ġ�� ������Ʈ �۾� ���͸� ���Ͻ�Ű�� ��
	// => ������Ʈ �Ӽ� - (�Ϲ� �Ӽ� - ��� ���͸�)�� (����� �Ӽ� - �۾� ���͸�) ��ġ��Ű��

	wchar_t szCurDir[256];
	GetCurrentDirectory(256, szCurDir);

	int Len = wcslen(szCurDir);
	for (int i = Len - 1; 0 < i; --i)
	{
		if (szCurDir[i] == L'\\')
		{
			szCurDir[i] = L'\0';
			break;
		}
	}

	m_ContentPath = szCurDir;
	m_ContentPath += L"\\content\\";
}
