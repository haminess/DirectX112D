#include "pch.h"
#include "ParamUI.h"

#include <Engine/CAssetMgr.h>

#include "CImGuiMgr.h"
#include "imgui/imgui.h"
#include "TreeUI.h"
#include "ListUI.h"

UINT ParamUI::g_ParamID = 0;

bool ParamUI::Param_Int(const string& _Desc, int* _Data, bool _Drag)
{
    ImGui::Text(_Desc.c_str());
    ImGui::SameLine(120);

    char szID[255] = {};
    sprintf_s(szID, "##Param_Int_%d", g_ParamID++);

    if (_Drag)
    {
        if (ImGui::DragInt(szID, _Data, 1.f))
        {
            return true;
        }
    }
    else
    {
        if (ImGui::InputInt(szID, _Data, 0))
        {
            return true;
        }
    }

    return false;
}

bool ParamUI::Param_Float(const string& _Desc, float* _Data, bool _Drag)
{
    ImGui::Text(_Desc.c_str());
    ImGui::SameLine(120);

    char szID[255] = {};
    sprintf_s(szID, "##Param_Float_%d", g_ParamID++);


    if (_Drag)
    {
        if (ImGui::DragFloat(szID, _Data, 1.f))
        {
            return true;
        }
    }
    else
    {
        if (ImGui::InputFloat(szID, _Data, 0))
        {
            return true;
        }
    }

    return false;
}

bool ParamUI::Param_Vec2(const string& _Desc, Vector2* _Data, bool _Drag)
{
    ImGui::Text(_Desc.c_str());
    ImGui::SameLine(120);

    char szID[255] = {};
    sprintf_s(szID, "##Param_Vec2_%d", g_ParamID++);


    float arr[2] = { _Data->x, _Data->y };

    if (_Drag)
    {
        if (ImGui::DragFloat2(szID, arr, 1.f))
        {
            _Data->x = arr[0];
            _Data->y = arr[1];
            return true;
        }
    }
    else
    {
        if (ImGui::InputFloat2(szID, arr, 0))
        {
            _Data->x = arr[0];
            _Data->y = arr[1];
            return true;
        }
    }

    return false;
}

bool ParamUI::Param_Vec4(const string& _Desc, Vector4* _Data, bool _Drag)
{
    ImGui::Text(_Desc.c_str());
    ImGui::SameLine(120);

    char szID[255] = {};
    sprintf_s(szID, "##Param_Vec4_%d", g_ParamID++);


    float arr[4] = { _Data->x, _Data->y, _Data->z, _Data->w };

    if (_Drag)
    {
        if (ImGui::DragFloat4(szID, arr, 1.f))
        {
            *_Data = Vector4(arr[0], arr[1], arr[2], arr[3]);
            return true;
        }
    }
    else
    {
        if (ImGui::InputFloat(szID, arr, 0))
        {
            *_Data = Vector4(arr[0], arr[1], arr[2], arr[3]);
            return true;
        }
    }

    return false;
}

bool ParamUI::Param_Tex(const string& _Desc, Ptr<CTexture>& _Tex
    , EditorUI* _Inst, EUI_DELEGATE_2 _MemFunc)
{
    ImGui::Text(_Desc.c_str());

    ImVec2 uv_min = ImVec2(0.0f, 0.0f);
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);
    ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.f);
    ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
    ImGui::Image(_Tex->GetSRV().Get(), ImVec2(100.f, 100.f), uv_min, uv_max, tint_col, border_col);

    if (ImGui::BeginDragDropTarget())
    {
        if (ImGui::AcceptDragDropPayload("ContentUI"))
        {
            const ImGuiPayload* pPayload = ImGui::GetDragDropPayload();
            TreeNode* pNode = *((TreeNode**)pPayload->Data);
            Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
            if (pAsset->GetAssetType() == ASSET_TYPE::TEXTURE)
            {
                _Tex = ((CTexture*)pAsset.Get());
                ImGui::SetWindowFocus(nullptr);
                return true;
            }
        }

        ImGui::EndDragDropTarget();
    }

    ImGui::SameLine();

    if (_Inst && _MemFunc)
    {
        if (ImGui::Button("##TexBtn", ImVec2(18.f, 18.f)))
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
            pListUI->AddDynamicDoubleClicked(_Inst, (EUI_DELEGATE_2)_MemFunc);

            return true;
        }
    }

    return false;
}
