#pragma once
#include "EditorUI.h"
class TreeUI;
class TreeNode;
class CGameObject;
class Outliner :
    public EditorUI
{
private:
	class TreeUI* m_Tree;


public:
	virtual void Render_Update() override;

public:
	void RenewGameObject();
	void AddGameObject(TreeNode* _ParentNode, CGameObject* _Object);

	void SelectGameObject(DWORD_PTR _TreeNode);
	void DragDrop(DWORD_PTR _DragNode, DWORD_PTR _DropNode);


public:
	Outliner();
	~Outliner();
};

