#pragma once


class EditorUI;
class Inspector;
class CImGuiMgr
	: public singleton<CImGuiMgr>
{
	SINGLE(CImGuiMgr)
private:
	map<string, EditorUI*>	m_mapUI;
	Inspector* m_Inspector;

public:
	int Init();
	void Progress();

public:
	EditorUI* FindUI(const string& _ID);
	Inspector* GetInsepector();
	void SetActive(string _ID, bool _On);

private:
	void CreateEditorUI();

	void Tick();
	void Render();
};

