#include "pch.h"
#include "TreeUI.h"

#include<Engine/CKeyMgr.h>

// =========
// TreeNode
// =========
UINT TreeNode::g_GlobalID = 0;
TreeNode::TreeNode()
	: m_Owner(nullptr)
	, m_Parent(nullptr)
	, m_Data(0)
	, m_Selected(false)
	, m_Opened(false)
	, m_BeingRenamed(false)
{
	char buff[50] = {};
	sprintf_s(buff, 50, "##%d", g_GlobalID++);
	m_ID = buff;
}

TreeNode::~TreeNode()
{
	DeleteVec(m_vecChild);
}

void TreeNode::Render_Update()
{
	// �⺻ �ɼ�
	int Flag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	string padding;

	// �߰� �ɼ�
	if (m_vecChild.empty())
		Flag |= ImGuiTreeNodeFlags_Leaf;
	if (m_Frame)
		Flag |= ImGuiTreeNodeFlags_Framed;
	if (m_Selected)
		Flag |= ImGuiTreeNodeFlags_Selected;
	if (m_vecChild.empty() && m_Frame)
		padding = "   ";

	string Name = padding + m_Name + m_ID;

	if (KEY_TAP(KEY::F2) && m_Selected)
	{
		m_BeingRenamed = true;
	}
	if (KEY_TAP(KEY::ENTER) && m_Selected)
	{
		m_BeingRenamed = false; 
		if (m_Name == "" || m_Name == " ")
		{
			wstring wname = ((CEntity*)m_Data)->GetName();
			string sname = string(wname.begin(), wname.end());
			m_Name = sname;
		}
		else
		{
			wstring wname = wstring(m_Name.begin(), m_Name.end());
			((CEntity*)m_Data)->SetName(wname);
		}

	}
	

	// ������ ��Ʈ
	if (m_BeingRenamed)
	{
		char* pChar = (char*)(m_Name.c_str());
		if (ImGui::InputText("##renamed", pChar, 255))
		{
			m_Name = string(pChar); 
		}

		return;
	}


	m_Opened = ImGui::TreeNodeEx(Name.c_str(), Flag);

	// ��带 Ŭ���ϸ�, ���� ���·� ����� �ش�.
	//if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
	if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		m_Selected = true;
		m_Owner->AddSelectedNode(this);
	}

	if (ImGui::BeginDragDropSource())
	{
		// Payload ����
		// 1�� ���͸�: ���̷ε� ���̵� ���ƾ� ��, �ַ� Ʈ�� �̸�ID ���
		// 2�� ���͸�: ����� �������� �޽� �����Ͱ� �ƴϸ� �ȵ�
		TreeNode* pThis = this;
		ImGui::SetDragDropPayload(m_Owner->GetName().c_str(), &pThis, sizeof(TreeNode*));
		ImGui::Text(m_Name.c_str()); // �巡�� �� ���콺�� Ʈ����� �̸� ���
		ImGui::EndDragDropSource();

		// TreeUI �� Drag ���
		m_Owner->SetDraggedNode(this);
	}

	// ���� �巡�� ���
	if (m_Owner->IsSelfDragDrop())
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (ImGui::AcceptDragDropPayload(m_Owner->GetName().c_str()))
			{
				const ImGuiPayload* pPayload = ImGui::GetDragDropPayload();
				TreeNode* pDragNode = *((TreeNode**)pPayload->Data);

				// TreeUI �� Drop ��� ���
				m_Owner->SetDroppedNode(this);
			}

			ImGui::EndDragDropTarget();
		}
	}


	if (m_Opened)
	{
		for (size_t i = 0; i < m_vecChild.size(); ++i)
		{
			m_vecChild[i]->Render_Update();
		}
		ImGui::TreePop();
	}



}



// ======
// TreeUI
// ======
TreeUI::TreeUI()
	: EditorUI("TreeUI")
	, m_Root(nullptr)
	, m_ShowRoot(true)
	, m_SelfDragDrop(false)
	, m_MultiSelection(false)
	, m_SelectedInst(nullptr)
	, m_SelectedFunc(nullptr)
	, m_SelfDragDropInst(nullptr)
	, m_SelfDragDropFunc(nullptr)
	, m_DraggedNode(nullptr)
	, m_DroppedNode(nullptr)
{
}

TreeUI::~TreeUI()
{
	if (nullptr != m_Root)
		delete m_Root;
}

void TreeUI::Render_Update()
{
	if (nullptr == m_Root)
		return;

	// ��Ʈ������ ��� ���� RenderUpdate
	if (m_ShowRoot)
		m_Root->Render_Update();
	else
	{
		for (size_t i = 0; i < m_Root->m_vecChild.size(); ++i)
		{
			m_Root->m_vecChild[i]->Render_Update();
		}
	}

	// ���� �巡�׵�� �۵�
	// �巡�� ��尡 �ְ�, ����� ��嵵 �ִ�
	// �巡�� ��尡 �ְ�, ���콺 ������ ������.
	if (m_DraggedNode && m_DroppedNode || m_DraggedNode && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		// ��ϵ� dragdrop delegate�� �ִٸ�
		if (m_SelfDragDropInst && m_SelfDragDropFunc)
		{
			(m_SelfDragDropInst->*m_SelfDragDropFunc)((DWORD_PTR)m_DraggedNode, (DWORD_PTR)m_DroppedNode);
		}

		m_DraggedNode = nullptr;
		m_DroppedNode = nullptr;
	}

}

void TreeUI::AddSelectedNode(TreeNode* _Node)
{
	if (m_MultiSelection)
	{
		m_vecSelected.push_back(_Node);
	}
	else
	{
		for (size_t i = 0; i < m_vecSelected.size(); ++i)
		{
			m_vecSelected[i]->m_Selected = false;
		}
		m_vecSelected.clear();
		m_vecSelected.push_back(_Node);
	}

	// ���� �ֱٿ� ���õ� ��忡 ���ؼ� Delegate �� ȣ���Ų��.
	if (m_SelectedInst && m_SelectedFunc)
	{
		(m_SelectedInst->*m_SelectedFunc)((DWORD_PTR)m_vecSelected.back());
	}
}

void TreeUI::SetDroppedNode(TreeNode* _Node)
{
	assert(m_DraggedNode);

	m_DroppedNode = _Node;
}


TreeNode* TreeUI::AddItem(TreeNode* _ParentNode, const string& _Name, DWORD_PTR _Data, bool _Frame)
{
	TreeNode* pNode = new TreeNode;

	pNode->m_Owner = this;
	pNode->m_Name = _Name;
	pNode->m_Data = _Data;
	pNode->m_Frame = _Frame;

	// Key�� �ƴ� Name���� ȭ�鿡 ���
	CEntity* pEntity = dynamic_cast<CEntity*>((CEntity*)pNode->m_Data);
	if (pEntity)
	{
		wstring name = pEntity->GetName();
		pNode->m_Name = string(name.begin(), name.end());
	}


	// _ParentNode �� nullptr �� ���
	if (nullptr == _ParentNode)
	{
		// Tree �� �߰��ϴ� �׸��� ��Ʈ�� �� ���̴�.
		assert(!m_Root);
		m_Root = pNode;
	}
	else
	{
		_ParentNode->AddChildNode(pNode);
	}

	return pNode;
}