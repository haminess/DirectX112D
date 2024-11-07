#include "pch.h"
#include "Inspector.h"
#include "ComponentUI.h"
#include "AssetUI.h"

#include <Engine/CLevelMgr.h>
#include <Engine/assets.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTaskMgr.h>

Inspector::Inspector()
	: EditorUI("Inspector")
	, m_arrComUI{}
{
	CreateComponentUI();
	CreateAssetUI();

	SetTargetObject(nullptr);
	SetTargetAsset(nullptr);
}

Inspector::~Inspector()
{
}


void Inspector::SetTargetObject(CGameObject* _Target)
{
	// 자식 UI 들에게 TargetObject 를 찾아서 알려준다.
	m_TargetObject = _Target;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComUI[i])
			continue;

		m_arrComUI[i]->SetTargetObject(m_TargetObject);
	}

	for (UINT i = 0; i < m_vecScriptUI.size(); ++i)
	{
		if (nullptr == m_vecScriptUI[i])
			continue;

		m_vecScriptUI[i]->SetTargetObject(m_TargetObject);
	}

	// AssetUI 는 비활성화
	if (nullptr != _Target)
		SetTargetAsset(nullptr);

	// ScriptUI 활성화
	if (nullptr == _Target)
		return;

	for (int i = 0; i < m_vecScriptUI.size(); ++i)
	{
		m_vecScriptUI[i]->SetActive(false);
	}

#ifdef _DEBUG
	const vector<CScript*>& scripts = m_TargetObject->GetScripts();
	ScriptUI* pScriptUI = nullptr;
	int ScriptCount = 0;
	for (UINT i = 0; i < scripts.size(); ++i)
	{
		if (scripts[i]->GetScriptName() == "")
			continue;

		// scriptui 있으면 타깃만 바꿔주기
		if (m_vecScriptUI.size() > ScriptCount)
		{
			pScriptUI = m_vecScriptUI[ScriptCount];
			pScriptUI->SetActive(true);
		}

		// 없으면 생성
		else
		{
			pScriptUI = new ScriptUI;
			m_vecScriptUI.push_back(pScriptUI);

			AddChildUI(pScriptUI);
		}

		pScriptUI->SetTargetScript(scripts[i]);
		++ScriptCount;
	}
#endif

}


void Inspector::SetTargetAsset(Ptr<CAsset> _Asset)
{
	m_TargetAsset = _Asset;

	// Asset 타입에 따라 인스펙터에 출력
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		if (nullptr == m_TargetAsset || i != m_TargetAsset->GetAssetType())
		{
			m_arrAssetUI[i]->SetActive(false);
		}
		else
		{
			m_arrAssetUI[i]->SetAsset(m_TargetAsset);
			m_arrAssetUI[i]->SetActive(true);
		}
	}

	// 기존 컴포넌트 UI는 비활성화
	if (nullptr != m_TargetAsset)
	{
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (nullptr == m_arrComUI[i])
				continue;

			m_arrComUI[i]->SetActive(false);
		}
	}
}


void Inspector::Tick()
{
	if (KEY_PRESSED(KEY::CTRL) && KEY_TAP(KEY::D))
	{
		CreateObject(m_TargetObject->Clone(), 0, false);
	}
	else if (KEY_TAP(KEY::DEL))
	{
		DestroyObject(m_TargetObject);
		SetTargetObject(nullptr);
	}

}

void Inspector::Render_Update()
{
	if (CTaskMgr::GetInst()->IsLevelChanged())
		SetTargetObject(nullptr);

	if (m_TargetObject)
	{
		ImGui::Text("Enabled");
		ImGui::SameLine(100);
		bool enabled = m_TargetObject->GetActive();
		if (ImGui::Checkbox("##obj-enable", &enabled))
		{
			m_TargetObject->SetActive(enabled);
		}

		// 레이어 바꾸기 콤보박스로 구현
		ImGui::Text("Layer");
		ImGui::SameLine(100);
		ImGui::SetNextItemWidth(150);

		string LayerName = std::to_string(m_TargetObject->GetLayerIdx()) + " 번 레이어";

		if (ImGui::BeginCombo("##LayerIdx", LayerName.c_str(), 0))
		{
			string itemName;
			for (int i = 0; i < MAX_LAYER; i++)
			{
				bool CurNum = i == m_TargetObject->GetLayerIdx() ? true : false;

				LayerName = std::to_string(i) + " 번 레이어";
				if (ImGui::Selectable(LayerName.c_str(), &CurNum))
				{
					m_TargetObject->SetLayerIdx(i);
				}
			}
			ImGui::EndCombo();
		}
	}
	

	//Vector2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
	//Vector2 vMouseWorldPos = CKeyMgr::GetInst()->GetMouseWorldPos();
	//ImGui::Text("MPos");
	//ImGui::SameLine(100);
	//ImGui::DragFloat2("##mousepos", vMousePos, 1.f, 0.f, ImGuiInputTextFlags_ReadOnly);
	//ImGui::Text("MWorldPos");
	//ImGui::SameLine(100);
	//ImGui::DragFloat2("##mouseworldpos", vMouseWorldPos, 1.f, 0.f, ImGuiInputTextFlags_ReadOnly);
}



#include "TransformUI.h"
#include "Collider2DUI.h"
#include "CameraUI.h"
#include "Light2DUI.h"
#include "MeshRenderUI.h"
#include "ParticleSystemUI.h"

void Inspector::CreateComponentUI()
{
	// InspectorUI 의 자식UI 추가
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = (ComponentUI*)AddChildUI(new TransformUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]->SetChildSize(ImVec2(0.f, 150.f));

	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = (ComponentUI*)AddChildUI(new Collider2DUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetChildSize(ImVec2(0.f, 120.f));

	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA] = (ComponentUI*)AddChildUI(new CameraUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]->SetChildSize(ImVec2(0.f, 200.f));

	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = (ComponentUI*)AddChildUI(new Light2DUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]->SetChildSize(ImVec2(0.f, 150.f));

	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER] = (ComponentUI*)AddChildUI(new MeshRenderUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]->SetChildSize(ImVec2(0.f, 150.f));

	//m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLE_SYSTEM] = (ComponentUI*)AddChildUI(new ParticleSystemUI);
	//m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLE_SYSTEM]->SetChildSize(ImVec2(0.f, 0.f));
}

#include "MeshUI.h"
#include "MeshDataUI.h"
#include "TextureUI.h"
#include "SoundUI.h"
#include "PrefabUI.h"
#include "FlipbookUI.h"
#include "SpriteUI.h"
#include "MaterialUI.h"
#include "GraphicShaderUI.h"
#include "ComputeShaderUI.h"

void Inspector::CreateAssetUI()
{
	m_arrAssetUI[(UINT)ASSET_TYPE::MESH] = new MeshUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::MESH_DATA] = new MeshDataUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::TEXTURE] = new TextureUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::SOUND] = new SoundUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::PREFAB] = new PrefabUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::FLIPBOOK] = new FlipbookUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::SPRITE] = new SpriteUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::MATERIAL] = new MaterialUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::GRAPHIC_SHADER] = new GraphicShaderUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::COMPUTE_SHADER] = new ComputeShaderUI;

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		AddChildUI(m_arrAssetUI[i]);
	}

}
