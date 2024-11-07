#pragma once
#include "pch.h"
#include "func_ui.h"

#include "imgui/imgui.h"
#include "TreeUI.h"
#include "ListUI.h"
#include "CImGuiMgr.h"
#include <Engine/CTexture.h>
#include <Engine/CAssetMgr.h>

// return string id
string DrawNameUI(const string& _Name)
{
	ImGui::Text(_Name.c_str());
	ImGui::SameLine(100);
	return "##" + _Name;
}

bool DrawTextUI(const string& _Name, string& _Value)
{
	const string& Id = DrawNameUI(_Name);
	return ImGui::InputText(Id.c_str(), (char*)_Value.c_str(), _Value.length());
}

bool DrawTextReadOnlyUI(const string& _Name, const string& _Value)
{
	const string& Id = DrawNameUI(_Name);
	return ImGui::InputText(Id.c_str(), (char*)_Value.c_str(), _Value.length(), ImGuiInputTextFlags_ReadOnly);
}


bool DrawVector2UI(const string& _Name, Vector2* _Value)
{
	const string& Id = DrawNameUI(_Name);
	return ImGui::DragFloat2(Id.c_str(), *_Value);
}

bool DrawNameInt2UI(const string& _Name, const string& _Name1, const string& _Name2, Vector2* _Value, int width)
{
	string Id = "##" + _Name + _Name1;
	DrawNameUI(_Name);
	ImGui::SameLine(100);
	ImGui::Text(_Name1.c_str());
	ImGui::SameLine(115);
	ImGui::SetNextItemWidth(width);
	ImGui::DragInt(Id.c_str(), (int*)&(_Value->x));

	Id = "##" + _Name + _Name2;
	ImGui::SameLine(120 + width);
	ImGui::Text(_Name2.c_str());
	ImGui::SameLine(135 + width);
	ImGui::SetNextItemWidth(width);
	return ImGui::DragInt(Id.c_str(), (int*)&(_Value->y));
}

bool DrawNameFloat2UI(const string& _Name, const string& _Name1, const string& _Name2, Vector2* _Value, int width)
{
	string Id = "##" + _Name + _Name1;
	DrawNameUI(_Name);
	ImGui::SameLine(100);
	ImGui::Text(_Name1.c_str());
	ImGui::SameLine(110);
	ImGui::SetNextItemWidth(75);
	ImGui::DragFloat(Id.c_str(), &(_Value->x));

	Id = "##" + _Name + _Name2;
	ImGui::SameLine(120 + width);
	ImGui::Text(_Name2.c_str());
	ImGui::SameLine(135 + width);
	ImGui::SetNextItemWidth(width);
	return ImGui::DragFloat(Id.c_str(), &(_Value->y));
}

bool DrawVector3UI(const string& _Name, Vector3* _Value)
{
	const string& Id = DrawNameUI(_Name);
	return ImGui::DragFloat3(Id.c_str(), *_Value);
}

bool DrawVector4UI(const string& _Name, Vector4* _Value)
{
	const string& Id = DrawNameUI(_Name);
	return ImGui::DragFloat4(Id.c_str(), *_Value);
}

void DrawBoldTextUI(const string& _Name)
{
	ImGui::Text(_Name.c_str());
	ImGui::SameLine(1);
	ImGui::Text(_Name.c_str());
	ImGui::SameLine(1);
	ImGui::Text(_Name.c_str());
}

bool DrawFloatUI(const string& _Name, float* _Value)
{
	const string& Id = DrawNameUI(_Name);
	return ImGui::DragFloat(Id.c_str(), _Value);
}

bool DrawIntUI(const string& _Name, int* _Value)
{
	const string& Id = DrawNameUI(_Name);
	return ImGui::InputInt(Id.c_str(), _Value);
}

bool DrawUINTUI(const string& _Name, UINT* _Value)
{
	const string& Id = DrawNameUI(_Name);
	bool bResult = ImGui::InputInt(Id.c_str(), (int*)_Value);
	if (bResult)
	{
		if (*_Value < 0)
			*_Value = abs(*(int*)_Value);
	}
	return bResult;
}

bool DrawColorUI(const string& _Name, Vector3* _Value)
{
	const string& Id = DrawNameUI(_Name);
	return ImGui::ColorEdit3(Id.c_str(), (float*)_Value, 0);
}

bool DrawColorUI(const string& _Name, Vector4* _Value)
{
	const string& Id = DrawNameUI(_Name);
	return ImGui::ColorEdit4(Id.c_str(), (float*)_Value, 0);
}