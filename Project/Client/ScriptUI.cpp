#include "pch.h"
#include "ScriptUI.h"
#include "CImGuiMgr.h"
#include "TreeUI.h"
#include "ListUI.h"
#include <Engine/CScript.h>
#include <Engine/CTexture.h>
#include <Engine/CParticleSystemScript.h>

ScriptUI::ScriptUI()
	: ComponentUI("ScriptUI")
	, m_Script(nullptr)
	, m_Value(nullptr)
{
}

ScriptUI::~ScriptUI()
{
}


void ScriptUI::Render_Update()
{
#ifdef _DEBUG

	if (!m_Script)
		return;

	ComponentTitle(m_Script->GetScriptName());

	const auto& Property = m_Script->GetProperties();

	for (int i = 0; i < Property.size(); ++i)
	{
		void* pValue = Property[i].pValue;
		string Type = Property[i].Type;
		string Name = Property[i].Name;
		string ID = "##" + Name;
		if (nullptr == pValue)
			ID += 99;
		else
			ID += *(int*)pValue;

		if (Type == "Header")
		{
			ImGui::Text(Name.c_str());
			ImGui::SameLine(1);
			ImGui::Text(Name.c_str());
			ImGui::SameLine(1);
			ImGui::Text(Name.c_str());
		}
		else
		{
			ImGui::Text(Name.c_str());
			ImGui::SameLine(100);
		}

		if (Type == "float")
		{
			ImGui::DragFloat(ID.c_str(), (float*)pValue);
		}
		else if (Type == "int")
		{
			ImGui::DragInt(ID.c_str(), (int*)pValue);
		}
		else if (Type == "UINT")
		{
			int value = *(int*)pValue;
			if (ImGui::DragInt(ID.c_str(), &value))
			{
				if (value < 0)
					value = abs(value);
				*(UINT*)pValue = value;
			}
		}
		else if (Type == "bool")
		{
			bool value = *(bool*)pValue;
			if (ImGui::Checkbox(ID.c_str(), &value))
			{
				*(bool*)pValue = value;
			}
		}
		else if (Type == "Vector2")
		{
			ImGui::DragFloat2(ID.c_str(), *(Vector2*)pValue);
		}
		else if (Type == "Vector3")
		{
			ImGui::DragFloat3(ID.c_str(), *(Vector3*)pValue);
		}
		else if (Type == "Vector4")
		{
			ImGui::DragFloat4(ID.c_str(), *(Vector4*)pValue);
		}
		else if (Type == "Ptr<CTexture>")
		{
			Ptr<CTexture>* ppTex = (Ptr<CTexture>*)pValue;
			wstring TexNameW = L"" == ppTex->Get()->GetKey()? L"Texture" : ppTex->Get()->GetKey();
			string TexName = string(TexNameW.begin(), TexNameW.end());

			if (ImGui::BeginCombo(ID.c_str(), TexName.c_str(), 0))
			{
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
						*(Ptr<CTexture>*)pValue = (CTexture*)pAsset.Get();
						ImGui::SetWindowFocus(nullptr);
					}
				}

				ImGui::EndDragDropTarget();
			}


			ImGui::SameLine();
			if (ImGui::Button(string(ID + "_Btn").c_str(), ImVec2(18.f, 18.f)))
			{
				// ListUI 를 활성화 시키기
				ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
				pListUI->SetName("Texture");
				pListUI->SetActive(true);

				// ListUI 에 넣어줄 문자열 정보 가져오기
				pListUI->AddItem("None");

				vector<wstring> vecAssetNames;
				CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecAssetNames);
				pListUI->AddItem(vecAssetNames);

				// 더블 클릭 시 호출시킬 함수 등록
				pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&ScriptUI::SelectTexture);
				m_Value = pValue;
			}
		}
		else if (Type == "Color3")
		{
			ImGui::ColorEdit3(ID.c_str(), (float*)pValue, 0);
		}
		else if (Type == "Color4")
		{
			ImGui::ColorEdit4(ID.c_str(), (float*)pValue, 0);
		}
	}

#endif
}

void ScriptUI::SelectTexture(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	// 리스트에서 더블킬릭한 항목의 이름을 받아온다.
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;

	if (*pStr == "None")
	{
		*(Ptr<CTexture>*)m_Value = nullptr;
		return;
	}

	// 해당 항목 에셋을 찾아서, Value 포인터에 넣어준다.
	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(wstring(pStr->begin(), pStr->end()));
	if (nullptr == pTex)
		return;

	*(Ptr<CTexture>*)m_Value = pTex;
	m_Value = nullptr;
}
