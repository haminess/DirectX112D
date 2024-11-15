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
	// 기본 옵션
	int Flag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	string padding;

	// 추가 옵션
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
	

	// 렌더링 파트
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

	// 노드를 클릭하면, 선택 상태로 만들어 준다.
	//if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
	if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		m_Selected = true;
		m_Owner->AddSelectedNode(this);
	}

	if (ImGui::BeginDragDropSource())
	{
		// Payload 개념
		// 1차 필터링: 페이로드 아이디가 같아야 함, 주로 트리 이름ID 사용
		// 2차 필터링: 날라온 컨텐츠가 메쉬 데이터가 아니면 안됨
		TreeNode* pThis = this;
		ImGui::SetDragDropPayload(m_Owner->GetName().c_str(), &pThis, sizeof(TreeNode*));
		ImGui::Text(m_Name.c_str()); // 드래그 시 마우스에 트리노드 이름 출력
		ImGui::EndDragDropSource();

		// TreeUI 에 Drag 등록
		m_Owner->SetDraggedNode(this);
	}

	// 셀프 드래그 드랍
	if (m_Owner->IsSelfDragDrop())
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (ImGui::AcceptDragDropPayload(m_Owner->GetName().c_str()))
			{
				const ImGuiPayload* pPayload = ImGui::GetDragDropPayload();
				TreeNode* pDragNode = *((TreeNode**)pPayload->Data);

				// TreeUI 에 Drop 노드 등록
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

	// 루트노드부터 모든 노드들 RenderUpdate
	if (m_ShowRoot)
		m_Root->Render_Update();
	else
	{
		for (size_t i = 0; i < m_Root->m_vecChild.size(); ++i)
		{
			m_Root->m_vecChild[i]->Render_Update();
		}
	}

	// 셀프 드래그드랍 작동
	// 드래그 노드가 있고, 드랍된 노드도 있다
	// 드래그 노드가 있고, 마우스 왼쪽이 떼졌다.
	if (m_DraggedNode && m_DroppedNode || m_DraggedNode && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		// 등록된 dragdrop delegate가 있다면
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

	// 가장 최근에 선택된 노드에 대해서 Delegate 를 호출시킨다.
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

	// Key가 아닌 Name으로 화면에 출력
	CEntity* pEntity = dynamic_cast<CEntity*>((CEntity*)pNode->m_Data);
	if (pEntity)
	{
		wstring name = pEntity->GetName();
		pNode->m_Name = string(name.begin(), name.end());
	}


	// _ParentNode 가 nullptr 인 경우
	if (nullptr == _ParentNode)
	{
		// Tree 에 추가하는 항목이 루트가 될 것이다.
		assert(!m_Root);
		m_Root = pNode;
	}
	else
	{
		_ParentNode->AddChildNode(pNode);
	}

	return pNode;
}