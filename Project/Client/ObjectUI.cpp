#include "pch.h"
#include "ObjectUI.h"
#include "CImGuiMgr.h"
#include "Inspector.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

ObjectUI::ObjectUI()
{
	SetName("Scene");

	m_CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
}

ObjectUI::~ObjectUI()
{
}

void ObjectUI::Tick()
{
	
}


void ObjectUI::Render_Update()
{
	return;
	m_Target = CImGuiMgr::GetInst()->GetInsepector()->GetTargetObject();
	if (nullptr == m_CurLevel)
	{
		m_CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	}

	if (nullptr == m_CurLevel)
		return;

	list<CGameObject*> vecParents = m_CurLevel->GetParentObjects();
	list<CGameObject*>::iterator iter = vecParents.begin();

	for (size_t i = 0; i < vecParents.size(); ++i)
	{
		// ��� ����
		int Flags = (*iter)->GetChild().size() > 0 ? ImGuiTreeNodeFlags_OpenOnArrow : ImGuiTreeNodeFlags_Leaf;
		
		if (m_Target == *iter)
		{
			Flags |= ImGuiTreeNodeFlags_Selected;
		}

		// �̸� ����
		wstring objName = (*iter)->GetName();
		if (L"" == objName)
		{
			objName = L"GameObject" + (*iter)->GetID();
		}
		objName += L"##" + std::to_wstring((*iter)->GetID());

		string name = string(objName.begin(), objName.end());

		bool isOpen = ImGui::TreeNodeEx(name.c_str(), Flags);

		// Ŭ��
		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			CImGuiMgr::GetInst()->GetInsepector()->SetTargetObject(*iter);
		}

		// ����Ŭ��
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			CImGuiMgr::GetInst()->GetInsepector()->SetTargetObject(*iter);
		}

		if (isOpen)
		{
			// �ڽ� ������ ������
			Render_Child(*iter);
			ImGui::TreePop();
		}
		++iter;
	}
}

void ObjectUI::Render_Child(CGameObject* _Child)
{
	vector<CGameObject*> vecChild = _Child->GetChild();

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		bool isParent = false;
		if (vecChild[i]->GetChild().size() > 0)
			isParent = true;


		int Flags = isParent? ImGuiTreeNodeFlags_OpenOnArrow : ImGuiTreeNodeFlags_Leaf;

		if (m_Target == vecChild[i])
		{
			Flags |= ImGuiTreeNodeFlags_Selected;
		}

		// �̸� ����
		wstring objName = vecChild[i]->GetName();
		if (L"" == objName)
		{
			objName = L"GameObject" + vecChild[i]->GetID();
		}
		objName += L"##" + std::to_wstring(vecChild[i]->GetID());

		string name = string(objName.begin(), objName.end());

		if (ImGui::TreeNodeEx(name.c_str(), Flags))
		{
			// Ŭ��
			if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			{
				m_SelectedIdx = i;
				CImGuiMgr::GetInst()->GetInsepector()->SetTargetObject(vecChild[i]);
			}

			// ����Ŭ��
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				// ����Ŭ���� �׸� ���ڿ� ���
				m_strSelected = m_vecList[i];
				CImGuiMgr::GetInst()->GetInsepector()->SetTargetObject(vecChild[i]);
			}


			// �ڽ� ������ ������
			if (isParent)
			{
				Render_Child(vecChild[i]);
			}

			ImGui::TreePop();
		}
	}
}



void ObjectUI::Selected()
{
	string objName = GetList()[(int)GetSelectedNum()];
	CImGuiMgr::GetInst()->GetInsepector()->SetTargetObject(CLevelMgr::GetInst()->FindObjectByName(wstring(objName.begin(), objName.end())));
}

void ObjectUI::Clicked(int _itemNum)
{
	string objName = GetList()[_itemNum];
	CImGuiMgr::GetInst()->GetInsepector()->SetTargetObject(CLevelMgr::GetInst()->FindObjectByName(wstring(objName.begin(), objName.end())));
}

void ObjectUI::Activate()
{
	Clear();
}

void ObjectUI::Deactivate()
{
	Clear();
}
