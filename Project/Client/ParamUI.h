#pragma once


#include <Engine/Ptr.h>
#include <Engine/CTexture.h>
#include "EditorUI.h"

class ParamUI
{
private:
	static UINT g_ParamID;

public:
	static bool Param_Int(const string& _Desc, int* _Data, bool _Drag);
	static bool Param_Float(const string& _Desc, float* _Data, bool _Drag);
	static bool Param_Vec2(const string& _Desc, Vector2* _Data, bool _Drag);
	static bool Param_Vec4(const string& _Desc, Vector4* _Data, bool _Drag);

	static bool Param_Tex(const string& _Desc, Ptr<CTexture>& _Tex
		, EditorUI* _Inst = nullptr, EUI_DELEGATE_2 _MemFunc = nullptr);

	friend class CImGuiMgr;
};

