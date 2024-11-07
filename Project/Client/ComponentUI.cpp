#include "pch.h"
#include "ComponentUI.h"

ComponentUI::ComponentUI(const string& _ID, COMPONENT_TYPE _Type)
    : EditorUI(_ID)
    , m_TargetObject(nullptr)
    , m_Type(_Type)
{
}

ComponentUI::ComponentUI(const string& _ID)
    : EditorUI(_ID)
    , m_TargetObject(nullptr)
    , m_Type(COMPONENT_TYPE::END)
{
}

ComponentUI::~ComponentUI()
{
}

void ComponentUI::SetTargetObject(CGameObject* _Target)
{
    m_TargetObject = _Target;

    if (nullptr == m_TargetObject)
    {
        SetActive(false);
    }
    else
    {
        // 본인 컴포넌트 없으면 비활성화
        CComponent* pComponent = m_TargetObject->GetComponent(m_Type);
        if (nullptr == pComponent)
        {
            SetActive(false);
        }
        else
        {
            SetActive(true);
        }
    }

}

void ComponentUI::ComponentTitle(const string& _title)
{
    ImGui::PushID(0);

    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));

    ImGui::Button(_title.c_str());

    ImGui::PopStyleColor(3);
    ImGui::PopID();
}