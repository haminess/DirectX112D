#include "pch.h"
#include "PrefabUI.h"

PrefabUI::PrefabUI()
	: AssetUI("Prefab", ASSET_TYPE::PREFAB)
{
}

PrefabUI::~PrefabUI()
{
}

void PrefabUI::Render_Update()
{
	AssetUI::Render_Update();
}
