#pragma once

void CreateObject(class CGameObject* _NewObj, int LayerIdx, bool _bMoveChild);
void DestroyObject(class CGameObject* _TargetObj);
void ChangeLevelState(LEVEL_STATE _NextState);
void ChangeLevel(class CLevel* Level, LEVEL_STATE NextState);
void AddChild(CGameObject* _Parent, CGameObject* _Child);


void DrawDebugRect(Vector4 _Color, Vector3 _Pos, Vector2 _Scale, Vector3 _Rotation, bool _DepthTest = false , float _Duration = 0.f);
void DrawDebugRect(Vector4 _Color, const Matrix& _matWorld, bool _DepthTest = false, float _Duration = 0.f);

void DrawDebugCircle(Vector4 _Color, Vector3 _Pos, float _Radius, bool _DepthTest = false, float _Duration = 0.f);

void DrawDebugLine(Vector4 _Color, Vector3 _Start, Vector3 _End, bool _DepthTest = false, float _Duration = 0.f);

void TestLibrary();


void SaveWString(const wstring& _str, FILE* _File);
void LoadWString(wstring& _str, FILE* _File);

#include "Ptr.h"
#include "CAsset.h"

template<typename T>
void SaveAssetRef(Ptr<T> _Asset, FILE* _File)
{
	// 에셋에 대한 참조 정보를 저장
	bool bAsset = _Asset.Get();
	fwrite(&bAsset, sizeof(bool), 1, _File);

	if (bAsset)
	{
		SaveWString(_Asset->GetName(), _File);
		SaveWString(_Asset->GetKey(), _File);
		SaveWString(_Asset->GetRelativePath(), _File);
	}
}

template<typename T>
void LoadAssetRef(Ptr<T>& _Asset, FILE* _File)
{
	bool bAsset;
	fread(&bAsset, sizeof(bool), 1, _File);

	if (bAsset)
	{
		wstring Name, Key, Path;
		LoadWString(Name, _File);
		LoadWString(Key, _File);
		LoadWString(Path, _File);

		_Asset = CAssetMgr::GetInst()->Load<T>(Key, Path);
		_Asset->SetName(Name);
	}
}

template<typename T>
T* LoadAssetRef(FILE* _File)
{
	Ptr<T> pAsset = nullptr;

	bool bAsset;
	fread(&bAsset, sizeof(bool), 1, _File);

	if (bAsset)
	{
		wstring Name, Key, Path;
		LoadWString(Name, _File);
		LoadWString(Key, _File);
		LoadWString(Path, _File);
		pAsset = CAssetMgr::GetInst()->Load<T>(Key, Path);
	}

	return pAsset.Get();
}




template<typename T, int Count>
void DeleteArray(T* (&arr)[Count])
{
	for (UINT i = 0; i < Count; ++i)
	{
		DELETE(arr[i]);
	}

}


template<typename T>
void DeleteVec(vector<T>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		DELETE(_vec[i]);
	}
	_vec.clear();
}

template<typename T1, typename T2>
void DeleteMap(map<T1, T2>& _map)
{
	for (const auto& pair : _map)
	{
		delete pair.second;
	}
	_map.clear();
}
