#include "pch.h"
#include "MeshRenderUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CMeshRender.h>
#include <Engine/CGameObject.h>
#include <Engine/CMesh.h>
#include <Engine/CMaterial.h>
#include <Engine/CTexture.h>

#include "CImGuiMgr.h"
#include "ListUI.h"
#include "TreeUI.h"

MeshRenderUI::MeshRenderUI()
	: ComponentUI("MeshRenderUI", COMPONENT_TYPE::MESHRENDER)
{
}

MeshRenderUI::~MeshRenderUI()
{
}

void MeshRenderUI::Render_Update()
{
	ComponentTitle("MeshRender");

	CMeshRender* pMeshRender = GetTargetObject()->MeshRender();

	Ptr<CMesh> pMesh = pMeshRender->GetMesh();

	// [�޽� ���� ���]
	string MeshName;
	if (nullptr == pMesh)
		MeshName = "None";
	else
	{
		MeshName = string(pMesh->GetKey().begin(), pMesh->GetKey().end());
	}

	ImGui::Text("Mesh");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150);
	ImGui::InputText("##MeshName", (char*)MeshName.c_str(), MeshName.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		// 1�� ���͸�: Payload ID
		if (ImGui::AcceptDragDropPayload("ContentUI"))
		{
			// ���콺 Ŭ�� ���� �� ����
			const ImGuiPayload* pPayload = ImGui::GetDragDropPayload();

			// Data�� TreeNode �ּҸ� ���� ���޸� �ּ�
			// ������ ����: pPayload -> Data -> (���� ���� ������, ���⼭�� TreeNode ������)
			TreeNode* pNode = (*(TreeNode**)(pPayload->Data));

			// �Ʒ� ������ �ȵǴ� ����, void �����ʹ� �� �ؼ��� �� �� ����
			//TreeNode* pNode = (TreeNode*)(*(pPayload->Data));	

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();

			// 2�� ���͸�: ���ϴ� ������ Ÿ��
			if (pAsset->GetAssetType() == ASSET_TYPE::MESH)
			{

			}
		}

		ImGui::EndDragDropTarget();
	}
	ImGui::SameLine();
	if (ImGui::Button("##MeshBtn", ImVec2(18.f, 18.f)))
	{
		// ListUI �� Ȱ��ȭ ��Ű��
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->SetName("Mesh");
		pListUI->SetActive(true);

		// ListUI �� �־��� ���ڿ� ���� ��������
		pListUI->AddItem("None");

		vector<wstring> vecAssetNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::MESH, vecAssetNames);
		pListUI->AddItem(vecAssetNames);

		// ���� Ŭ�� �� ȣ���ų �Լ� ���
		pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&MeshRenderUI::SelectMesh);
	}


	// [���׸��� ���� ���]
	Ptr<CMaterial> pMtrl = pMeshRender->GetMaterial();
	string MtrlName;
	if (nullptr == pMtrl)
	{
		MtrlName = "None";
	}
	else
	{
		MtrlName = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());
	}

	ImGui::Text("Material");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150);
	ImGui::InputText("##MtrlName", (char*)MtrlName.c_str(), MtrlName.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		if (ImGui::AcceptDragDropPayload("ContentUI"))
		{
			const ImGuiPayload* pPayload = ImGui::GetDragDropPayload();
			TreeNode* pNode = *((TreeNode**)pPayload->Data);
			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (pAsset->GetAssetType() == ASSET_TYPE::MATERIAL)
			{
				pMeshRender->SetMaterial((CMaterial*)pAsset.Get());
				ImGui::SetWindowFocus(nullptr);
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##MtrlBtn", ImVec2(18.f, 18.f)))
	{
		// ListUI �� Ȱ��ȭ ��Ű��
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->SetName("Material");
		pListUI->SetActive(true);

		// ListUI �� �־��� ���ڿ� ���� ��������
		pListUI->AddItem("None");

		vector<wstring> vecAssetNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::MATERIAL, vecAssetNames);
		pListUI->AddItem(vecAssetNames);

		// ���� Ŭ�� �� ȣ���ų �Լ� ���
		pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&MeshRenderUI::SelectMaterial);
	}

	if (nullptr == pMtrl)
		return;

	// [�ؽ�ó ���� ���]
	string TexName;
	Ptr<CTexture> pTex = pMtrl->GetTexture(m_TexParam);
	if (nullptr == pTex)
		TexName = "None";
	else
	{
		TexName = string(pTex->GetKey().begin(), pTex->GetKey().end());
	}

	ImGui::Text("Texture");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150);

	TexName = std::to_string((int)m_TexParam) + " : " + string(TexName.begin(), TexName.end());

	if (ImGui::BeginCombo("##TexParam", TexName.c_str(), 0))
	{
		string itemName;
		for (int i = 0; i < TEX_PARAM::TEX_END; i++)
		{
			pTex = pMtrl->GetTexture(i);
			if (nullptr == pTex)
				TexName = "None";
			else
				TexName = string(pTex->GetKey().begin(), pTex->GetKey().end());
			itemName = std::to_string(i) + " : " + TexName;
			if (ImGui::Selectable(itemName.c_str()))
			{
				m_TexParam = (TEX_PARAM)i;
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (ImGui::AcceptDragDropPayload("ContentUI"))
		{
			const ImGuiPayload* pPayload = ImGui::GetDragDropPayload();
			TreeNode* pNode = *((TreeNode**)pPayload->Data);
			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (pAsset->GetAssetType() == ASSET_TYPE::TEXTURE)
			{
				pMeshRender->GetMaterial()->SetTexParam(m_TexParam, ((CTexture*)pAsset.Get()));
				ImGui::SetWindowFocus(nullptr);
			}
		}

		ImGui::EndDragDropTarget();
	}


	ImGui::SameLine();
	if (ImGui::Button("##TexBtn", ImVec2(18.f, 18.f)))
	{
		// ListUI �� Ȱ��ȭ ��Ű��
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->SetName("Texture");
		pListUI->SetActive(true);

		// ListUI �� �־��� ���ڿ� ���� ��������
		pListUI->AddItem("None");

		vector<wstring> vecAssetNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecAssetNames);
		pListUI->AddItem(vecAssetNames);

		// ���� Ŭ�� �� ȣ���ų �Լ� ���
		pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&MeshRenderUI::SelectTexture);
	}
}

void MeshRenderUI::SelectMesh(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	// ����Ʈ���� ����ų���� �׸��� �̸��� �޾ƿ´�.
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;

	if (*pStr == "None")
	{
		GetTargetObject()->MeshRender()->SetMesh(nullptr);
		return;
	}

	// �ش� �׸� ������ ã�Ƽ�, MeshRenderComponent �� �ش� �޽ø� �����ϰ� �Ѵ�.
	Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(wstring(pStr->begin(), pStr->end()));
	if (nullptr == pMesh)
		return;

	GetTargetObject()->MeshRender()->SetMesh(pMesh);
}

void MeshRenderUI::SelectMaterial(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	// ����Ʈ���� ����ų���� �׸��� �̸��� �޾ƿ´�.
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;

	if (*pStr == "None")
	{
		GetTargetObject()->MeshRender()->SetMaterial(nullptr);
		return;
	}

	// �ش� �׸� ������ ã�Ƽ�, MeshRenderComponent �� �ش� �޽ø� �����ϰ� �Ѵ�.
	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(wstring(pStr->begin(), pStr->end()));
	if (nullptr == pMtrl)
		return;

	GetTargetObject()->MeshRender()->SetMaterial(pMtrl);
}

void MeshRenderUI::SelectTexture(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	// ����Ʈ���� ����ų���� �׸��� �̸��� �޾ƿ´�.
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;

	if (*pStr == "None")
	{
		GetTargetObject()->MeshRender()->SetMaterial(nullptr);
		return;
	}

	// �ش� �׸� ������ ã�Ƽ�, MeshRenderComponent �� �ش� �޽ø� �����ϰ� �Ѵ�.
	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(wstring(pStr->begin(), pStr->end()));
	if (nullptr == pTex)
		return;

	GetTargetObject()->MeshRender()->GetMaterial()->SetTexParam(m_TexParam, pTex);
}
