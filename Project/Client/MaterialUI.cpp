#include "pch.h"
#include "MaterialUI.h"

#include <Engine/CPathMgr.h>
#include <Engine/CAssetMgr.h>

#include "CImGuiMgr.h"
#include "ListUI.h"
#include "TreeUI.h"

MaterialUI::MaterialUI()
	: AssetUI("Material", ASSET_TYPE::MATERIAL)
{
}

MaterialUI::~MaterialUI()
{
}

void MaterialUI::Render_Update()
{
	AssetTitle();

	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
	Ptr<CGraphicShader> pShader = pMtrl->GetShader();

	string ShaderName;

	if (nullptr == pShader)
		ShaderName = "None";
	else
	{
		ShaderName = string(pShader->GetKey().begin(), pShader->GetKey().end());
	}

	ImGui::Text("Shader");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150);
	ImGui::InputText("##MeshName", (char*)ShaderName.c_str(), ShaderName.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		if (ImGui::AcceptDragDropPayload("ContentUI"))
		{
			const ImGuiPayload* pPayload = ImGui::GetDragDropPayload();
			TreeNode* pNode = *((TreeNode**)pPayload->Data);
			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (pAsset->GetAssetType() == ASSET_TYPE::GRAPHIC_SHADER)
			{
				pMtrl->SetShader((CGraphicShader*)pAsset.Get());
				ImGui::SetWindowFocus(nullptr);
			}
		}

		ImGui::EndDragDropTarget();
	}


	ImGui::SameLine();
	if (ImGui::Button("##ShaderBtn", ImVec2(18.f, 18.f)))
	{
		// ListUI �� Ȱ��ȭ ��Ű��
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->SetName("GraphicShader");
		pListUI->SetActive(true);

		// ListUI �� �־��� ���ڿ� ���� ��������
		pListUI->AddItem("None");

		vector<wstring> vecAssetNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::GRAPHIC_SHADER, vecAssetNames);
		pListUI->AddItem(vecAssetNames);

		// ���� Ŭ�� �� ȣ���ų �Լ� ���
		pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&MaterialUI::SelectGraphicShader);
	}

	// ������ ���Ϸ� �����ϱ�
	if (ImGui::Button("SAVE"))
	{
		Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
		assert(pMtrl.Get());

		wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + pMtrl->GetKey();
		pMtrl->Save(strFilePath);
	}
}




void MaterialUI::SelectGraphicShader(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	// ����Ʈ���� ����ų���� �׸��� �̸��� �޾ƿ´�.
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;

	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
	if (nullptr == pMtrl)
		return;

	if (*pStr == "None")
	{
		pMtrl->SetShader(nullptr);
		return;
	}

	// �ش� �׸� ������ ã�Ƽ�, MeshRenderComponent �� �ش� �޽ø� �����ϰ� �Ѵ�.
	Ptr<CGraphicShader> pShader = CAssetMgr::GetInst()->FindAsset<CGraphicShader>(wstring(pStr->begin(), pStr->end()));
	if (nullptr == pShader)
		return;

	pMtrl->SetShader(pShader);
}