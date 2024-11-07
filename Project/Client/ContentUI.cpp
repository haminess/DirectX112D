#include "pch.h"
#include "ContentUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>

#include "CImGuiMgr.h"
#include "Inspector.h"
#include "TreeUI.h"

ContentUI::ContentUI()
	: EditorUI("Content")
{
	m_Tree = (TreeUI*)AddChildUI(new TreeUI);
	m_Tree->SetName("ContentUI");
	m_Tree->ShowRoot(false);
	m_Tree->AddDynamicSelect(this, (EUI_DELEGATE_1)&ContentUI::SelectAsset);

	RenewContent();
}

ContentUI::~ContentUI()
{
}

void ContentUI::Render_Update()
{
	if (CAssetMgr::GetInst()->IsAssetChanged())
	{
		RenewContent();
	}
}

void ContentUI::RenewContent()
{
	m_Tree->Clear();

	// 리스트를 위한 루트 생성
	TreeNode* pRootNode = m_Tree->AddItem(nullptr, "RootNode", 0);

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		TreeNode* pAssetNode = m_Tree->AddItem(pRootNode, ASSET_TYPE_STRING[i], 0, true);

		const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetAssets((ASSET_TYPE)i);

		// 에셋 리스트 추가
		for (const auto& pair : mapAsset)
		{
			m_Tree->AddItem(pAssetNode, string(pair.first.begin(), pair.first.end()), (DWORD_PTR)pair.second.Get());
		}
	}
}

void ContentUI::SelectAsset(DWORD_PTR _TreeNode)
{
	TreeNode* pNode = (TreeNode*)_TreeNode;
	Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();

	if (nullptr == pAsset)
		return;

	Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspector->SetTargetAsset(pAsset);
}
