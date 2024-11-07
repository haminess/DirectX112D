#pragma once

// STL
#include <vector>
#include <list>
#include <map>
#include <string>
#include <algorithm>

using std::vector;
using std::list;
using std::map;
using std::make_pair;
using std::string;
using std::wstring;

#include "singleton.h"
#include "Windows.h"

// DirectX 11 ���̺귯�� ��ġ
// ���־� ��Ʃ��� ��ġ �� �Բ� ������
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>	// ���� ����
#include <DirectXPackedVector.h>	// ���� ���� �߰�

using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

#include "SimpleMath.h"

#include "enum.h"
#include "define.h"
#include "struct.h"
#include "func.h"

#include "Ptr.h"

// ComPtr
#include <wrl.h>
using namespace Microsoft::WRL;

// Directx Tex
#include <DirectxTex/DirectxTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectxTex\\DirectXTex_debug")
#else
#pragma comment(lib, "DirectxTex\\DirectXTex")
#endif
