#include "pch.h"
#include "CameraUI.h"

#include <Engine/CCamera.h>
#include <Engine/enum.h>
#include <Engine/CRenderMgr.h>

CameraUI::CameraUI()
    : ComponentUI("CameraUI", COMPONENT_TYPE::CAMERA)
{
}

CameraUI::~CameraUI()
{
}

void CameraUI::Render_Update()
{
    ComponentTitle("Camera");

    CCamera* pCamera = GetTargetObject()->Camera();

    ImGui::Text("Projection Type");
    ImGui::SameLine(120);

    // PROJ_TYPE
    const char* szProjType[2] = { "Perspective", "Orthographic" };
    int ProjType = pCamera->GetProjType();
    ImGui::Combo("##combo", &ProjType, szProjType, 2);
    pCamera->SetProjType((PROJ_TYPE)ProjType);

    // Far
    ImGui::Text("Far");
    ImGui::SameLine(120);
    float Far = pCamera->GetFar();
    ImGui::InputFloat("##Far", &Far);

    // Far �� Near(1) ���� �۰ų� ������ �ȵȴ�. �þ߹����� Near ���� Far �����̱� ����
    if (Far <= 1.f)
        Far = 1.1f;
    pCamera->SetFar(Far);

    // Width
    float Width = pCamera->GetWidth();
    float AspectRatio = pCamera->GetAspectRatio();
    float FOV = pCamera->GetFOV();

    ImGui::Text("Width");
    ImGui::SameLine(120);
    ImGui::InputFloat("##Width", &Width);
    pCamera->SetWidth(Width);

    ImGui::Text("AspectRatio");
    ImGui::SameLine(120);
    ImGui::DragFloat("##AspectRatio", &AspectRatio, 0.01f);
    pCamera->SetAspectRatio(AspectRatio);

    // ���� ī�޶� ���� �ɼ��� ���������̸� ��Ȱ��ȭ
    ImGui::BeginDisabled(ProjType == (int)PROJ_TYPE::ORTHOGRAPHIC);

    // �������� ����
    ImGui::Text("Field Of View");
    ImGui::SameLine(120);
    FOV = (FOV * 180.f) / XM_PI;
    ImGui::DragFloat("##FOV", &FOV, 0.1f);
    FOV = (FOV * XM_PI) / 180.f;
    pCamera->SetFOV(FOV);

    ImGui::EndDisabled();


    // ���� ī�޶� ���� �ɼ��� ���������̸� ��Ȱ��ȭ
    ImGui::BeginDisabled(ProjType == (int)PROJ_TYPE::PERSPECTIVE);

    // �������� ���� ����
    float Scale = pCamera->GetScale();

    ImGui::Text("Scale");
    ImGui::SameLine(120);
    FOV = (FOV * 180.f) / XM_PI;
    ImGui::DragFloat("##Scale", &Scale, 0.1f);
    FOV = (FOV * XM_PI) / 180.f;

    // �������� ������ 0 ���Ϸ� �������� �ȵȴ�.
    if (Scale <= 0.f)
        Scale = 0.1f;

    pCamera->SetScale(Scale);

    ImGui::EndDisabled();

    // ����    
    UINT        m_LayerCheck;   // ī�޶� ���� ���̾� ��Ʈ üũ
    int         m_Priority;     // ī�޶� �켱����, 0 : MainCamera, -1 : �̵��

    // layer check
    ImGui::Text("Layer Check");
    UINT layerCheck = pCamera->GetLayerCheck();

    if (ImGui::BeginTable("LayerCheck", 8))
    {
        for (int i = 0; i < 32; ++i)
        {
            string layerName = std::to_string(i) + "##Layer";
            bool bLayer = (layerCheck & (1 << i));

            ImGui::TableNextColumn();

            if (ImGui::Selectable(layerName.c_str(), &bLayer))
                pCamera->LayerCheck(i);


        }
        ImGui::EndTable();
    }

    // priority
    ImGui::Text("Priority");
    ImGui::SameLine(100);
    int priority = pCamera->GetCameraPriority();

    vector<CCamera*> cams = CRenderMgr::GetInst()->GetCameraList();
    wstring camName = pCamera->GetOwner()->GetName();
    string itemName = std::to_string(priority) + " : " + string(camName.begin(), camName.end());
    
    if (ImGui::BeginCombo("##Priority", itemName.c_str(), 0))
    {
        itemName = std::to_string(-1) + " : Remove";
        if (ImGui::Selectable(itemName.c_str(), (-1 == priority)))
        {
            pCamera->SetPriority(-1);
        }

        for (int i = 0; i < cams.size(); i++)
        {
            if (nullptr != cams[i])
            {
                camName = cams[i]->GetOwner()->GetName();
                itemName = std::to_string(i) + " : " + string(camName.begin(), camName.end());
                if (ImGui::Selectable(itemName.c_str(), (i == priority)))
                {
                    pCamera->SetPriority(i);
                }
            }
        }
        
        itemName = std::to_string(cams.size()) + " : New Priority";
        if (ImGui::Selectable(itemName.c_str(), (cams.size() == priority)))
        {
            pCamera->SetPriority(cams.size());
        }
        ImGui::EndCombo();
    }

}