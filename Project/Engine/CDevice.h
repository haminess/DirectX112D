#pragma once

class CConstBuffer;
class CTexture;
class CDevice
	: public singleton<CDevice>
{
	SINGLE(CDevice);

private:
	HWND								m_hWnd;			// ȭ���� �׸� ������
	Vector2								m_Resolution;

	ComPtr<ID3D11Device>				m_Device;		// GPU ����, GPU �޸𸮿� �����͸� ����, GPU�� ������ ����� ���� �� ���, ���� �Ҵ�� ����
	// ������ �͵� : shader, texture, swapchain, view, buffer, input layout

	ComPtr<ID3D11DeviceContext>			m_Context;		// GPU ����, GPU ������ ���� ���, Rendering, GPGPU(General Purpose GPU)
	// ������ ��� : clear, get/set, map, draw, copy, shader

	ComPtr<IDXGISwapChain>				m_SwapChain;	// BackBuffer ���� �� ���� ������ ����, ����/�ĸ� ���� ����

	Ptr<CTexture>						m_RTTex;        // RenderTargetTexture
	Ptr<CTexture>						m_DSTex;        // DepthStencilTexture

	// ȭ�� ǥ�� ���� ����(��ġ, ũ��, ����)
	D3D11_VIEWPORT						m_ViewPort;

	// GPU�� ������ ���ε��ϱ� ���� ��� ����
	CConstBuffer*						m_arrCB[(UINT)CB_TYPE::END];

	// �����Ͷ�����, ���� 3D -> �ȼ� 2D ��ȯ
	// ȭ�� ��ǥ�� ��ȯ(3D -> 3D NDC -> 2D �ػ� ��ǥ), �ȼ� ���� -> �ø�
	ComPtr<ID3D11RasterizerState>		m_RSState[(UINT)RS_TYPE::END];

	// ���ĺ��� ����
	ComPtr<ID3D11BlendState>			m_BSState[(UINT)BS_TYPE::END];

	// ���÷�, �ؽ��Ŀ��� �ȼ� ���̴��� ���� �����͸� �����ϴ� ��ü
	// ���� ������ ������ ���� (�ؽ��� UV ��ǥ�� ���� ���� �� �ַ� ���)
	ComPtr<ID3D11SamplerState>			m_Sampler[2];

	// ���� ����
	ComPtr<ID3D11DepthStencilState>		m_DSState[(UINT)DS_TYPE::END];	//ID3D11BlendState*			m_BSState;

public:
	void Clear();
	void Present();

public:
	ComPtr<ID3D11Device>		GetDevice() { return m_Device; }
	ComPtr<ID3D11DeviceContext> GetContext() { return m_Context; }
	D3D11_VIEWPORT GetViewport() { return m_ViewPort; }

	CConstBuffer* GetCB(CB_TYPE _Type) { return m_arrCB[(UINT)_Type]; }
	ComPtr<ID3D11RasterizerState> GetRSState(RS_TYPE _Type) { return m_RSState[(UINT)_Type]; }
	ComPtr<ID3D11BlendState> GetBSState(BS_TYPE _Type) { return m_BSState[(UINT)_Type]; }
	ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _Type) { return m_DSState[(UINT)_Type]; }

public:
	Vector2 GetRenderResolution() { return m_Resolution; }

public:
	int Init(HWND _hWnd, Vector2 _Resolution);

private:
	int CreateSwapChain();
	int CreateView();
	int CreateConstBuffer();
	int CreateRasterizerState();
	int CreateSamplerState();
	int CreateBlendState();
	int CreateDepthStencilState();
};

