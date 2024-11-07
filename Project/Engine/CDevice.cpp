#include "pch.h"
#include "CDevice.h"

#include "CAssetMgr.h"
#include "CConstBuffer.h"

CDevice::CDevice()
	: m_ViewPort{}
	, m_arrCB{}
	, m_RSState{}
	, m_BSState{}
	, m_Sampler{}
{}

CDevice::~CDevice()
{
	for (UINT i = 0; i < (UINT)CB_TYPE::END; ++i)
	{
		DELETE(m_arrCB[i]);
	}
}

int CDevice::Init(HWND _hWnd, Vector2 _Resolution)
{
	m_hWnd = _hWnd;
	m_Resolution = _Resolution;

	UINT iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL Level = D3D_FEATURE_LEVEL_11_0;

	// 1. Devcie, Context �����ϱ�
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr
		, iFlag, nullptr, 0, D3D11_SDK_VERSION
		, m_Device.GetAddressOf(), &Level, m_Context.GetAddressOf())))
	{
		return E_FAIL;
	}


	// 2. SwapChain �����ϱ�
	if (FAILED(CreateSwapChain())) return E_FAIL;

	// 3. RenderTarget, DepthStencilTex, View �����ϱ�
	if (FAILED(CreateView())) return E_FAIL;

	// 4. State �����ϱ�
	if (FAILED(CreateRasterizerState())) return E_FAIL;
	if (FAILED(CreateSamplerState())) return E_FAIL;
	if (FAILED(CreateBlendState())) return E_FAIL;
	if (FAILED(CreateDepthStencilState())) return E_FAIL;


	// 5. ViewPort ���� - Window ȭ�� ���� ����
	// Viewport�� 3D �������� ��ü�� ������ ȭ�鿡 ǥ�õǴ� ����
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;

	m_ViewPort.Width = m_Resolution.x;
	m_ViewPort.Height = m_Resolution.y;

	m_ViewPort.MinDepth = 0.f;
	m_ViewPort.MaxDepth = 1.f;

	m_Context->RSSetViewports(1, &m_ViewPort);

	// Rendering �������� ����
	m_Context->OMSetRenderTargets(1, m_RTTex->GetRTV().GetAddressOf(), m_DSTex->GetDSV().Get());

	// 6. ������� ����
	if (FAILED(CreateConstBuffer())) return E_FAIL;

	g_Data.RenderResolution = m_Resolution;

	return S_OK;
}

int CDevice::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC Desc = {};

	Desc.BufferCount = 1;                               // ����� ����
	Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Swapchain ����� �ؽ����� �뵵 ����

	Desc.BufferDesc.Width = m_Resolution.x;             // ����� �ػ� Width
	Desc.BufferDesc.Height = m_Resolution.y;            // ����� �ػ� Height
	Desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	Desc.BufferDesc.RefreshRate.Denominator = 1;        // ȭ�� ���� �ӵ�, �и�
	Desc.BufferDesc.RefreshRate.Numerator = 60;         // ȭ�� ���� �ӵ�, ����

	Desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	Desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	Desc.OutputWindow = m_hWnd;                         // ����ۿ� �׷��� �̹����� ��½�ų ������
	Desc.Windowed = true;                               // â���, ��üȭ�� ���

	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;

	Desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;         // ������ �׷��� �̹����� �������� ����
	Desc.Flags = 0;

	ComPtr<IDXGIDevice>    IDXGIDevice = nullptr;
	ComPtr<IDXGIAdapter>   pAdapter = nullptr;
	ComPtr<IDXGIFactory>   pFactory = nullptr;

	// 128 ��Ʈ
	// GUID (Global Unique ID)
	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)IDXGIDevice.GetAddressOf());
	IDXGIDevice->GetAdapter(pAdapter.GetAddressOf());
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

	if (FAILED(pFactory->CreateSwapChain(m_Device.Get(), &Desc, m_SwapChain.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int CDevice::CreateView()
{
	// 1. RenderTarget �ؽ���   
	ComPtr<ID3D11Texture2D> pTex2D = nullptr;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pTex2D.GetAddressOf());
	m_RTTex = CAssetMgr::GetInst()->CreateTexture(L"RenderTargetTex", pTex2D);

	// 2. DepthStencil �ؽ���
	m_DSTex = CAssetMgr::GetInst()->CreateTexture(L"DepthStencilTex", m_Resolution.x, m_Resolution.y, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);

	return S_OK;
}

int CDevice::CreateConstBuffer()
{
	m_arrCB[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer(CB_TYPE::TRANSFORM);
	m_arrCB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(TransInfo));

	m_arrCB[(UINT)CB_TYPE::MATERIAL] = new CConstBuffer(CB_TYPE::MATERIAL);
	m_arrCB[(UINT)CB_TYPE::MATERIAL]->Create(sizeof(MtrlConst));

	m_arrCB[(UINT)CB_TYPE::SPRITE] = new CConstBuffer(CB_TYPE::SPRITE);
	m_arrCB[(UINT)CB_TYPE::SPRITE]->Create(sizeof(SpriteInfo));

	m_arrCB[(UINT)CB_TYPE::GLOBAL] = new CConstBuffer(CB_TYPE::GLOBAL);
	m_arrCB[(UINT)CB_TYPE::GLOBAL]->Create(sizeof(GlobalData));

	return S_OK;
}

int CDevice::CreateRasterizerState()
{
	// CULL_BACK
	m_RSState[(UINT)RS_TYPE::CULL_BACK] = nullptr;

	// CULL_FRONT
	D3D11_RASTERIZER_DESC Desc = {};
	Desc.CullMode = D3D11_CULL_FRONT;
	Desc.FillMode = D3D11_FILL_SOLID;
	DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());

	// CULL_NONE
	Desc.CullMode = D3D11_CULL_NONE;
	Desc.FillMode = D3D11_FILL_SOLID;
	DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());

	// WIRE_FRAME
	Desc.CullMode = D3D11_CULL_NONE;
	Desc.FillMode = D3D11_FILL_WIREFRAME;
	DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());

	return S_OK;
}

int CDevice::CreateSamplerState()
{
	D3D11_SAMPLER_DESC Desc = {};

	// 1. �ؽ��� ���� ���
	// �ؽ�ó ��� �ۿ� �ִ�([0.0, 1.0] ������ ���) �ؽ�ó ��ǥ ���� ����
	D3D11_TEXTURE_ADDRESS_MODE mode;
	mode = D3D11_TEXTURE_ADDRESS_WRAP;			// �ؽ�ó�� Ÿ��ȭ
	mode = D3D11_TEXTURE_ADDRESS_MIRROR;		// �ؽ�ó�� ��Ī �̵�
	mode = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;	// 
	mode = D3D11_TEXTURE_ADDRESS_CLAMP;			// 0.0 �Ǵ� 1.0�� �ؽ�ó ������ ����
	mode = D3D11_TEXTURE_ADDRESS_BORDER;		// desc �Ǵ� HLSL �ڵ忡 ������ �׵θ� ������ ����

	// 2. �̹漺 ���͸� ���÷�
	// 2-1) ������ �ؼ� ������ ���
	Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.Filter = D3D11_FILTER_ANISOTROPIC;
	DEVICE->CreateSamplerState(&Desc, m_Sampler[0].GetAddressOf());
	
	// 2-2) MIN_MAG_POINT ����
	// ��� �� Ȯ�븦 ���� ���� ���� ���
	Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	DEVICE->CreateSamplerState(&Desc, m_Sampler[1].GetAddressOf());

	CONTEXT->VSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->HSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->DSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->GSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->PSSetSamplers(0 /*���÷� ���� ��ȣ*/, 1 /*���÷� ��*/, m_Sampler[0].GetAddressOf());
	CONTEXT->CSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());

	CONTEXT->VSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->HSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->DSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->GSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->PSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->CSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());

	return S_OK;
}

int CDevice::CreateBlendState()
{
	m_BSState[(UINT)BS_TYPE::DEFAULT] = nullptr;

	D3D11_BLEND_DESC Desc = {};

	Desc.AlphaToCoverageEnable = false;

	// �������� ����Ÿ�ٿ� ���� ���� ������ ������ �ϰ� ������ ������
	// ���� ������ ���̸� ����Ÿ�ٸ��� �ɼ��� �������־�� �Ѵ�.
	// �ϰ� ������ ���̸� 0�� ����Ÿ�ٸ� ����
	Desc.IndependentBlendEnable = false;
	Desc.RenderTarget[0].BlendEnable = true;                // ���� ��� ����

	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;          // ���� ����� ���󳢸� ���ϱ� ������� ���
	// Source : PixelShader return ��(RGB), �� ���� ��� ����� ����
	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;      // ����� Alpha�� �Ͽ� RGB�� Alpha�� ���ؼ� ����
	// Destination : ������ ��µ� ����Ÿ��, ���� �� ���� RGB�� ����
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // RGB�� (1-Alpha)�� ���ؼ� ����

	// ������ RGBA���� Alpha�� �� �� ���
	// ũ�� �ǹ� ���� ������ �ƹ� ���̳� ����
	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO /*D3D11_BLEND_ONE�� ����*/;
	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;


	if (FAILED(DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ALPHABLEND].GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int CDevice::CreateDepthStencilState()
{
	// Less
	m_DSState[(UINT)DS_TYPE::LESS] = nullptr;

	// LessEqual
	D3D11_DEPTH_STENCIL_DESC Desc = {};

	Desc.StencilEnable = false;
	Desc.DepthEnable = true;                            // �������� ����
	Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;       // �۰ų� ������� ���
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // ���� ���

	DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf());

	// Greater
	Desc.StencilEnable = false;
	Desc.DepthEnable = true;                            // �������� ����
	Desc.DepthFunc = D3D11_COMPARISON_GREATER;          // ���̰� ū ��� ���
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // ���� ���

	DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::GREATER].GetAddressOf());

	// NoTest
	Desc.StencilEnable = false;
	Desc.DepthEnable = false;                           // �������� �������� ����
	Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;           // �׻� ���
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // ���� ���

	DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST].GetAddressOf());

	// NoWrite
	Desc.StencilEnable = false;
	Desc.DepthEnable = true;                            // �������� ����
	Desc.DepthFunc = D3D11_COMPARISON_LESS;             // ������� ���
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;  // ���� ������� ����

	DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_WRITE].GetAddressOf());

	// NoTest NoWrite
	Desc.StencilEnable = false;
	Desc.DepthEnable = false;                           // �������� X
	Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;           // �׻� ���
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;  // ���� ������� ����

	DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());

	return S_OK;
}



void CDevice::Clear()
{
	// RenderTarget �� DepthStencilTexture Ŭ����
	Vector4 vClearColor = Vector4(0.5f, 0.5f, 0.5f, 0.f);
	m_Context->ClearRenderTargetView(m_RTTex->GetRTV().Get(), vClearColor);
	m_Context->ClearDepthStencilView(m_DSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void CDevice::Present()
{
	// ȭ��(������) �� RenderTarget �� �׷��� �̹����� ��½�Ų��.
	m_SwapChain->Present(0, 0);
}