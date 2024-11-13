#include "pch.h"
#include "AnimEditorUI.h"
#include "func_ui.h"
#include "ListUI.h"
#include "CImGuiMgr.h"
#include <Engine/CTexture.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CPathMgr.h>
#include <Engine/CSprite.h>
#include <Engine/CTexture.h>
#include <Engine/CFlipbook.h>
#include <Engine/CFlipbookPlayer.h>

AnimEditorUI::AnimEditorUI()
	: EditorUI("Animation Editor")
	, m_ScaleFactor(100.f)
	, m_PreviewPlayer(nullptr)
{
	m_PreviewPlayer = new CFlipbookPlayer;
	m_CurAnim = CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"LinkWalkDown");
	m_PreviewPlayer->AddFlipbook(0, m_CurAnim);
	m_PreviewPlayer->Play(0, 10, true);
}

AnimEditorUI::~AnimEditorUI()
{
	delete m_PreviewPlayer;
}

void AnimEditorUI::Render()
{
	m_PreviewPlayer->FinalTick();

	//ImGui::SetNextWindowPos(ImVec2(100.f, 100.f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(1400, 600), ImGuiCond_FirstUseEver);
	EditorUI::Render();
}


void AnimEditorUI::Render_Update()
{

	//
	//	// 메뉴바
	//	if (ImGui::BeginMenuBar())
	//	{
	//		if (ImGui::BeginMenu("File"))
	//		{
	//			if (ImGui::MenuItem("Load Sprite")) {}
	//			if (ImGui::MenuItem("Save Sprite")) {}
	//			ImGui::EndMenu();
	//		}
	//		ImGui::EndMenuBar();
	//	}

		// 좌우 분할 레이아웃
	ImGui::Columns(2);
	ImVec2 vecWinSize = ImGui::GetWindowSize();
	ImGui::SetColumnWidth(0, vecWinSize.x * 0.7f); // 왼쪽 70%


	// 왼쪽 영역 (텍스처 뷰어)
	{
		ImGui::BeginChild("Texture Viewer", ImVec2(0, 0), true,
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		ImVec2 winMin = ImGui::GetWindowPos();
		ImVec2 winMax = ImVec2(winMin.x + ImGui::GetWindowSize().x, winMin.y + ImGui::GetWindowSize().y);

		// 화면 확대
		if (ImGui::IsMouseHoveringRect(winMin, winMax))
		{
			float wheelDelta = ImGui::GetIO().MouseWheel;
			if (wheelDelta > 0.f)
			{
				m_ScaleFactor += DT * 10000.f;
			}
			if (wheelDelta < 0.f)
			{
				m_ScaleFactor -= DT * 10000.f;
			}
		}

		// 화면 이동
		if (KEY_PRESSED(KEY::MBTN))
		{
			m_PreviewPos += CKeyMgr::GetInst()->GetMouseDir();
		}






		// 애니메이션 출력
		Ptr<CSprite> pCurSprite = m_PreviewPlayer->GetCurrentSprite();

		ImTextureID tex = nullptr;
		if (nullptr != pCurSprite)
		{
			tex = pCurSprite->GetAtlasTexture()->GetSRV().Get();

			Vector2 uvLeftTop = pCurSprite->GetLeftTop();
			Vector2 uvSlice = pCurSprite->GetSlice();
			ImVec2 uv_min = ImVec2(uvLeftTop.x, uvLeftTop.y);
			ImVec2 uv_max = ImVec2(uvLeftTop.x + uvSlice.x, uvLeftTop.y + uvSlice.y);
			float aspect_ratio = 1.f;
			ImGui::SetCursorPos(ImVec2(m_PreviewPos.x, m_PreviewPos.y));

			// 윈도우상 아틀라스 시작 위치 구하기
			Vector2 vAtlasPos = Vector2(winMin.x + m_PreviewPos.x, winMin.y + m_PreviewPos.y);
			// 윈도우상 아틀라스 크기 구하기
			Vector2 vAtlasScale = Vector2(m_ScaleFactor, m_ScaleFactor / aspect_ratio);
			// 마우스 좌표 가져오기
			Vector2 mousePos = Vector2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);

			ImGui::Image(tex, ImVec2(vAtlasScale.x, vAtlasScale.y), uv_min, uv_max, ImVec4(1.f, 1.f, 1.f, 1.f), ImGui::GetStyleColorVec4(ImGuiCol_Border));
		}


		// 애니메이션 재생 버튼
		float buttonWidth = 100.f;
		float buttonHeight = 30.f;
		float buttonSpacing = 10.f;  // 버튼 사이의 간격

		// 윈도우 중앙 계산
		float windowWidth = ImGui::GetWindowSize().x;
		float windowHeight = ImGui::GetWindowSize().y;
		float totalButtonWidth = (buttonWidth * 2) + buttonSpacing;

		// 버튼의 Y 위치 (하단에서 약간 위)
		float buttonsY = windowHeight - buttonHeight - 20.f;

		// Play 버튼 위치 설정
		ImGui::SetCursorPos(ImVec2(
			(windowWidth - totalButtonWidth) * 0.5f,
			buttonsY
		));
		if (ImGui::Button("Play", ImVec2(buttonWidth, buttonHeight))) {
			// Play 버튼 클릭 시 동작
			//m_CurAnim = CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"LinkWalkDown");
			m_PreviewPlayer->Play(0, 10, true);
		}

		// Stop 버튼 위치 설정
		ImGui::SetCursorPos(ImVec2(
			(windowWidth - totalButtonWidth) * 0.5f + buttonWidth + buttonSpacing,
			buttonsY
		));
		if (ImGui::Button("Stop", ImVec2(buttonWidth, buttonHeight))) {
			// Stop 버튼 클릭 시 동작
			m_PreviewPlayer->Pause();
		}


		ImGui::EndChild();
	}

	ImGui::NextColumn();

	// 오른쪽 영역 (설정 패널)
	{
		const char* pivots[] = { "Center", "Left" , "Right" , "Top" , "TopLeft" , "TopRight" , "Bottom" , "BottomLeft" , "BottomRight", "Custom" };

		// Atlas Info
		{
			ImGui::BeginChild("Anim Info", ImVec2(0, 200));
			ImVec2 backSize = ImGui::GetWindowSize();
			int itemWidth = (backSize.x - 150) / 2;

			DrawBoldTextUI("Animation");
			if (nullptr != m_CurAnim)
			{
				// name
				wstring wname = m_CurAnim->GetName();
				string name = string(wname.begin(), wname.end());
				if (DrawTextUI("Name", name))
				{
					wname = wstring(name.begin(), name.end());
					m_CurAnim->SetName(wname);
				}

				// frame count
				string animInfo = "Frame Count : " + std::to_string(m_CurAnim->GetMaxSprite());
				ImGui::Text(animInfo.c_str());
				animInfo = "Duration : " + std::to_string(m_CurAnim->GetMaxSprite() * m_PreviewPlayer->GetFPS()) + " sec";
				ImGui::Text(animInfo.c_str());

				ImGui::Text("");

				// button
				ImGui::SameLine(ImGui::GetColumnWidth() - 70 * 2);
				if (ImGui::Button("Load", ImVec2(60, 0))) 
				{
					// ListUI 를 활성화 시키기
					ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
					pListUI->SetName("Flipbook");
					pListUI->SetActive(true);

					// ListUI 에 넣어줄 문자열 정보 가져오기
					pListUI->AddItem("None");

					vector<wstring> vecAssetNames;
					CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::FLIPBOOK, vecAssetNames);
					pListUI->AddItem(vecAssetNames);

					// 더블 클릭 시 호출시킬 함수 등록
					pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&AnimEditorUI::LoadFlipbook);
				}

				ImGui::SameLine(ImGui::GetColumnWidth() - 70);
				if (ImGui::Button("Save", ImVec2(60, 0))) { Save(); }
			}


			ImGui::EndChild();
		}
		
		// Frame
		{
			ImGui::BeginChild("Frame Edit", ImVec2(0, 170));
			{

				// frame list
				ImGui::Text("");
				ImGui::Text("");
				wstring curFrameName;
				DrawBoldTextUI("Frame Info");
				if (ImGui::BeginListBox("##FrameList_animeditor", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
				{
					for (int i = 0; i < m_CurAnim->GetMaxSprite(); i++)
					{
						Ptr<CSprite> pSprite = m_CurAnim->GetSprite(i);
						wstring spriteName = pSprite->GetName();
						if (spriteName == L"")
							spriteName = pSprite->GetKey();
						string items = std::to_string(i) + " : " + string(spriteName.begin(), spriteName.end()) + "##animeditor";
						bool is_selected = (m_PreviewPlayer->GetCurrentSprite() == pSprite);
						if (ImGui::Selectable(items.c_str(), is_selected, 0))
						{
							m_PreviewPlayer->SetCurSpriteIdx(i);
							m_PreviewPlayer->Pause();
						}

						if (is_selected)
						{
							m_CurSpriteIdx = i;
							ImGui::SetItemDefaultFocus();
							curFrameName = pSprite->GetName();
						}
					}
					ImGui::EndListBox();
					
					if (curFrameName == L"")
						curFrameName = L"None";
					string strCurFrameName = string(curFrameName.begin(), curFrameName.end());
					DrawTextUI("Sprite Name", strCurFrameName);
					ImGui::SameLine();
					if (ImGui::Button("##TexBtn", ImVec2(18.f, 18.f)))
					{
						// ListUI 를 활성화 시키기
						ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
						pListUI->SetName("Sprite");
						pListUI->SetActive(true);

						// ListUI 에 넣어줄 문자열 정보 가져오기
						pListUI->AddItem("None");

						vector<wstring> vecAssetNames;
						CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::SPRITE, vecAssetNames);
						pListUI->AddItem(vecAssetNames);

						// 더블 클릭 시 호출시킬 함수 등록
						pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&AnimEditorUI::SelectSprite);
					}





					// button
					ImGui::Text("");
					ImGui::SameLine(ImGui::GetColumnWidth() - 70 * 2);
					if (ImGui::Button("Add", ImVec2(60, 0))) 
					{ 
						Ptr<CSprite> pSprite = m_CurAnim->GetSprite(m_CurAnim->GetMaxSprite() - 1);
						m_CurAnim->AddSprite(pSprite);
					}

					ImGui::SameLine(ImGui::GetColumnWidth() - 70);
					if (ImGui::Button("Del", ImVec2(60, 0))) {}
				}

				//				DrawBoldTextUI("Slice");
				//
				//				ImVec2 backSize = ImGui::GetWindowSize();
				//				int itemWidth = (ImGui::GetColumnWidth() - 150) / 2;
				//
				//				const char* types[] = { "Grid By Cell Size" , "Grid By Cell Count" };
				//				static int typeIndex = 0;
				//
				//				// slice type
				//				DrawNameUI("Type");
				//				ImGui::Combo("##Type", &typeIndex, types, IM_ARRAYSIZE(types));
				//
				//				// slice by size
				//				if (typeIndex == 0)
				//				{
				//					//Vector2 vPixelSize = {}, vOffset = {}, vPadding = {};
				//					DrawNameInt2UI("Pixel Size", "X", "Y", &m_SliceInput, itemWidth);
				//					//DrawNameInt2UI("Offset", "X", "Y", &vOffset, itemWidth);
				//					//DrawNameInt2UI("Padding", "X", "Y", &vPadding, itemWidth);
				//
				//					//static int pivotIndex = 0;
				//
				//					//DrawNameUI("Pivot");
				//					//ImGui::Combo("##Pivot", &pivotIndex, pivots, IM_ARRAYSIZE(pivots));
				//					//if (pivotIndex == 9)
				//					//{
				//					//	Vector2 vCustomPivot = {};
				//					//	DrawVector2UI("Custom Pivot", &vCustomPivot);
				//					//}
				//				}
				//
				//				// slice by count
				//				else if (typeIndex == 1)
				//				{
				//					//Vector2 vCount = {}, vOffset = {}, vPadding = {};
				//					DrawNameInt2UI("Pixel Count", "X", "Y", &m_SliceInput, itemWidth);
				//					//DrawNameInt2UI("Offset", "X", "Y", &vOffset, itemWidth);
				//					//DrawNameInt2UI("Padding", "X", "Y", &vPadding, itemWidth);
				//
				//					//static int pivotIndex = 0;
				//
				//					//DrawNameUI("Pivot");
				//					//ImGui::Combo("##Pivot", &pivotIndex, pivots, IM_ARRAYSIZE(pivots));
				//					//if (pivotIndex == 9)
				//					//{
				//					//	Vector2 vCustomPivot = {};
				//					//	DrawVector2UI("Custom Pivot", &vCustomPivot);
				//					//}
				//				}
				//
				//				if (ImGui::Button("Slice", ImVec2(-1, 0)))
				//				{
				//					if (typeIndex == 0)
				//						SliceBySize(m_SliceInput);
				//					else if (typeIndex == 1)
				//						SliceByCount(m_SliceInput);
				//				}
				//			}
				ImGui::EndChild();
			}
			//
			//		ImGui::Spacing();
			//
			//		// Sprite 설정
			//		{
			//			if (nullptr != m_CurSprite)
			//			{
			//				ImGui::BeginChild("Sprite Settings");
			//				{
			//					ImVec2 backSize = ImGui::GetWindowSize();
			//					int itemWidth = (ImGui::GetColumnWidth() - 130) / 2;
			//					DrawBoldTextUI("Sprite");
			//
			//					wstring spriteWName = m_CurSprite->GetName();
			//					string spriteName = string(spriteWName.begin(), spriteWName.end());
			//					DrawTextUI("Name", spriteName);
			//
			//					Vector2 vPos = m_CurSprite->GetLeftTop() * m_CurAtlas->GetResolution(), vScale = m_CurSprite->GetSlice() * m_CurAtlas->GetResolution();
			//
			//					DrawNameInt2UI("Position", "X", "Y", &vPos, itemWidth);
			//					DrawNameInt2UI("", "W", "H", &vScale, itemWidth);
			//					//Vector2 vBorderLT = {};
			//					//Vector2 vBorderRB = {};
			//					//DrawNameInt2UI("Border", "L", "T", &vBorderLT, itemWidth);
			//					//DrawNameInt2UI("", "R", "B", &vBorderRB, itemWidth);
			//
			//
			//					//int pivotModeIndex = 9;
			//					//DrawNameUI("Pivot");
			//					//ImGui::Combo("##Pivot", &pivotModeIndex, pivots, IM_ARRAYSIZE(pivots));
			//
			//					//if (pivotModeIndex == 9)
			//					//{
			//					//	Vector2 vCustomPivot = {};
			//					//	DrawNameInt2UI("Custom Pivot", "X", "Y", &vCustomPivot, itemWidth);
			//					//}
			//				}
			//				ImGui::EndChild();
			//			}
			//			else
			//			{
			//				ImGui::Text("Select a Sprite");
			//			}
		}
	}

}

void AnimEditorUI::Load()
{
	// 텍스쳐 정보 가져오기

	// 스프라이트 정보 가져오기

	// 스프라이트 정보 화면에 띄우기
	// 1. 스프라이트 LeftTop, RightBottom, Pivot

}

void AnimEditorUI::LoadFlipbook(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	// 리스트에서 더블클릭한 항목의 이름을 받아온다.
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;

	if (*pStr == "None")
	{
		m_CurAnim = nullptr;
		return;
	}

	// 해당 항목 에셋을 찾아서, MeshRenderComponent 가 해당 메시를 참조하게 한다.
	m_CurAnim = CAssetMgr::GetInst()->FindAsset<CFlipbook>(wstring(pStr->begin(), pStr->end()));
	m_PreviewPlayer->AddFlipbook(0, m_CurAnim);
}

void AnimEditorUI::SelectSprite(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	//// 리스트에서 더블클릭한 항목의 이름을 받아온다.
	//ListUI* pListUI = (ListUI*)_ListUI;
	//string* pStr = (string*)_SelectString;

	//if (*pStr == "None")
	//{
	//	m_CurAnim = nullptr;
	//	return;
	//}

	//// 해당 항목 에셋을 찾아서, MeshRenderComponent 가 해당 메시를 참조하게 한다.
	//Ptr<CSprite> 
	//m_CurAnim->GetSprite(m_CurSpriteIdx) = 
	//m_PreviewPlayer->AddFlipbook(0, m_CurAnim);
}

void AnimEditorUI::Save()
{
	if (nullptr == m_CurAnim)
		return;

	wstring path = CPathMgr::GetInst()->GetContentPath() + L"Flipbook\\";

	wstring fullPath = path + m_CurAnim->GetKey() + L".flip";
	m_CurAnim->Save(fullPath);
}

