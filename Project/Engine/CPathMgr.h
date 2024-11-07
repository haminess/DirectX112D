#pragma once

class CPathMgr
	: public singleton<CPathMgr>
{
	// 프로젝트에서 상대경로로 리소스를 다룰 수 있도록
	// 1. 리소스 절대경로를 업데이트하고 관리한다.

	SINGLE(CPathMgr);
private:
	wstring m_ContentPath;

public:
	const wstring& GetContentPath() { return m_ContentPath; }
	wstring GetRelativePath(const wstring& _FullPath)
	{
		return _FullPath.substr(GetContentPath().length());
	}
	wstring GetFolderFromPath(const wstring& _Path)
	{
		size_t lastSlash = _Path.find_last_of(L"/\\");

		return _Path.substr(0, lastSlash + 1);
	}
	wstring GetNameFromPath(const wstring& _Path)
	{
		size_t lastDot = _Path.find_last_of('.');
		size_t lastSlash = _Path.find_last_of(L"/\\");

		if (0 == lastDot)
		{
			return _Path.substr(lastSlash + 1);
		}

		if(0 == lastSlash)
		{
			return _Path.substr(0, lastDot - 1);
		}

		return _Path.substr(lastSlash + 1, lastDot - lastSlash - 1);
	}
	wstring GetNameWithPath(const wstring& _Path)
	{
		size_t lastDot = _Path.find_last_of('.');

		return _Path.substr(0, lastDot);
	}

public:
	void Init();
};

