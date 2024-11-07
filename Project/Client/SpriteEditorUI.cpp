#include "pch.h"
#include "SpriteEditorUI.h"
#include "func_ui.h"
#include <Engine/CTexture.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>

SpriteEditorUI::SpriteEditorUI()
    : EditorUI("SpriteEditor")
    , m_ScaleFactor(50.f)
{
}

SpriteEditorUI::~SpriteEditorUI()
{
}

void SpriteEditorUI::Render()
{
    ImGui::SetNextWindowSize(ImVec2(1400, 600), ImGuiCond_FirstUseEver);
    EditorUI::Render();
}

void SpriteEditorUI::Render_Update()
{
    // 메뉴바
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Load Sprite")) {}
            if (ImGui::MenuItem("Save Sprite")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // 좌우 분할 레이아웃
    ImGui::Columns(2);
    ImVec2 vecWinSize = ImGui::GetWindowSize();
    ImGui::SetColumnWidth(0, vecWinSize.x * 0.7f); // 왼쪽 80%

    // 왼쪽 영역 (텍스처 뷰어)
    {

        ImGui::BeginChild("Texture Viewer");
        ImVec2 min = ImGui::GetWindowPos();
        ImVec2 max = ImVec2(min.x + ImGui::GetWindowSize().x, min.y + ImGui::GetWindowSize().y);
        if (ImGui::IsMouseHoveringRect(min, max))
        {
            if (KEY_TAP(KEY::WHEEL_UP))
            {
                m_ScaleFactor += DT * 10000.f;
            }
            if (KEY_TAP(KEY::WHEEL_DOWN))
            {
                m_ScaleFactor -= DT * 10000.f;
            }
        }


        if (KEY_PRESSED(KEY::MBTN))
        {
            m_AtlasPos += CKeyMgr::GetInst()->GetMouseDir();
        }
        Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(L"JumpAtlas", L"");
        ImVec2 uv_min = ImVec2(0.0f, 0.0f);
        ImVec2 uv_max = ImVec2(1.0f, 1.0f);
        ImVec4 tint_col = ImVec4(0.f, 1.f, 0.f, 1.f);
        ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
        float height = 50.f, width = 50.f;
        ImTextureID tex = nullptr;
        if (nullptr != pTex)
        {
            height = pTex->GetHeight();
            width = pTex->GetWidth();
            tex = pTex->GetSRV().Get();
        }
        ImGui::SetCursorPos(ImVec2(m_AtlasPos.x, m_AtlasPos.y));
        ImGui::Image(tex, ImVec2(m_ScaleFactor, m_ScaleFactor * height / width), uv_min, uv_max, ImVec4(1.f, 1.f, 1.f, 1.f), border_col);

        Vector2 debugPos = CKeyMgr::GetInst()->GetMousePos();//ImGui::GetCursorPos();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 windowPos = ImGui::GetWindowPos();

        // 일반 사각형
        draw_list->AddRect( ImVec2(debugPos.x, debugPos.y)
                          , ImVec2(debugPos.x + 100.f, debugPos.y + 100.f)
                          , IM_COL32(0, 255, 0, 255), 0.0f, 0, 1.f);


        ImGui::EndChild();
    }

    ImGui::NextColumn();

    // 오른쪽 영역 (설정 패널)
    {
        const char* pivots[] = { "Center", "Left" , "Right" , "Top" , "TopLeft" , "TopRight" , "Bottom" , "BottomLeft" , "BottomRight", "Custom" };

        // Atlas Info
        ImGui::BeginGroup();
        ImVec2 backSize = ImGui::GetWindowSize();
        int itemWidth = (backSize.x - 150) / 2;

        DrawBoldTextUI("Atlas Info"); 
        char atlasName[256] = "";
        string name = atlasName;
        DrawTextUI("Name", name);

        ImGui::Text("");
        ImGui::SameLine(ImGui::GetColumnWidth() - 70 * 2);
        if (ImGui::Button("Load", ImVec2(60, 0))) {}

        ImGui::SameLine(ImGui::GetColumnWidth() - 70);
        if (ImGui::Button("Save", ImVec2(60, 0))) {}
        ImGui::EndGroup();

        ImGui::Spacing();

        // Slice
        ImGui::BeginChild("Slice Settings", ImVec2(0, 170));
        {
            ImVec2 backSize = ImGui::GetWindowSize();
            int itemWidth = (ImGui::GetColumnWidth() - 150) / 2;
            DrawBoldTextUI("Slice");

            const char* types[] = { "Grid By Cell Size" , "Grid By Cell Count" };
            static int typeIndex = 0;

            DrawNameUI("Type");
            ImGui::Combo("##Type", &typeIndex, types, IM_ARRAYSIZE(types));
            
            if (typeIndex == 0)
            {
                Vector2 vPixelSize = {}, vOffset = {}, vPadding = {};
                DrawNameInt2UI("Pixel Size", "X", "Y", &vPixelSize, itemWidth);
                DrawNameInt2UI("Offset", "X", "Y", &vOffset, itemWidth);
                //DrawNameInt2UI("Padding", "X", "Y", &vPadding, itemWidth);

                static int pivotIndex = 0;

                DrawNameUI("Pivot");
                ImGui::Combo("##Pivot", &pivotIndex, pivots, IM_ARRAYSIZE(pivots));
                if (pivotIndex == 9)
                {
                    Vector2 vCustomPivot = {};
                    DrawVector2UI("Custom Pivot", &vCustomPivot);
                }
            }
            else if (typeIndex == 1)
            {
                Vector2 vPixelSize = {}, vOffset = {}, vPadding = {};
                DrawNameInt2UI("Pixel Size", "X", "Y", &vPixelSize, itemWidth);
                //DrawNameInt2UI("Offset", "X", "Y", &vOffset, itemWidth);
                //DrawNameInt2UI("Padding", "X", "Y", &vPadding, itemWidth);

                static int pivotIndex = 0;

                DrawNameUI("Pivot");
                ImGui::Combo("##Pivot", &pivotIndex, pivots, IM_ARRAYSIZE(pivots));
                if (pivotIndex == 9)
                {
                    Vector2 vCustomPivot = {};
                    DrawVector2UI("Custom Pivot", &vCustomPivot);
                } 
            }

            if (ImGui::Button("Slice", ImVec2(-1, 0))) {}
        }
        ImGui::EndChild();

        ImGui::Spacing();

        // Sprite 설정
        ImGui::BeginChild("Sprite Settings");
        {
            ImVec2 backSize = ImGui::GetWindowSize();
            int itemWidth = (ImGui::GetColumnWidth() - 130) / 2;
            DrawBoldTextUI("Sprite");

            string spriteName = "";
            DrawTextUI("Name", spriteName);

            Vector2 vPos = {}, vScale = {};

            DrawNameInt2UI("Position", "X", "Y", &vPos, itemWidth);
            DrawNameInt2UI("", "W", "H", &vScale, itemWidth);
            Vector2 vBorderLT = {};
            Vector2 vBorderRB = {};
            DrawNameInt2UI("Border", "L", "T", &vBorderLT, itemWidth);
            DrawNameInt2UI("", "R", "B", &vBorderRB, itemWidth);


            int pivotModeIndex = 9;
            DrawNameUI("Pivot");
            ImGui::Combo("##Pivot", &pivotModeIndex, pivots, IM_ARRAYSIZE(pivots));

            if (pivotModeIndex == 9)
            {
                Vector2 vCustomPivot = {};
                DrawNameInt2UI("Custom Pivot", "X", "Y", &vCustomPivot, itemWidth);
            }
        }
        ImGui::EndChild();
    }

}

void SpriteEditorUI::Load(Ptr<CTexture> _Tex)
{
    // 텍스쳐 정보 가져오기

    // 스프라이트 정보 가져오기

    // 스프라이트 정보 화면에 띄우기
    // 1. 스프라이트 LeftTop, RightBottom, Pivot

}

void SpriteEditorUI::Save(Ptr<CTexture> _Tex)
{
}
