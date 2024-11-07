#include "pch.h"
#include "CMesh.h"
#include "CDevice.h"

CMesh::CMesh(bool _bEngineRes)
	: CAsset(ASSET_TYPE::MESH, _bEngineRes)
	, m_VtxSysMem(nullptr)
	, m_IdxSysMem(nullptr)
	, m_VBDesc{}
	, m_VtxCount(0)
	, m_IBDesc{}
	, m_IdxCount(0)
{
}


CMesh::~CMesh()
{
	DELETE_ARR(m_VtxSysMem);
	DELETE_ARR(m_IdxSysMem);
}

void CMesh::Binding()
{
	// IA
	// �������� ���� ���� ����
	UINT Stride = sizeof(Vertex);	// ���� �޸� ����, �����ڸ��� ������ ������ ũ�Ⱑ �ٸ��� ������
	UINT Offset = 0;				// ������ ������ �Ϻθ� �׸� �� ���

	CONTEXT->IASetVertexBuffers(0, 1 /*���� ����*/, m_VB.GetAddressOf(), &Stride, &Offset);
	CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT /*���� ����*/, 0);
}

void CMesh::Render()
{
	Binding();

	CONTEXT->DrawIndexed(m_IdxCount, 0, 0);
}

void CMesh::Render_Particle(int _Count)
{
	Binding();

	CONTEXT->DrawIndexedInstanced(m_IdxCount, _Count, 0, 0, 0);
}


int CMesh::Create(Vertex* _Vtx, UINT _VtxCount, UINT* _pIdx, UINT _IdxCount)
{
	// 2. ���ؽ� ���� ����
	// ���� �����͸� SysMem -> GPU Mem �� �̵�
	assert(nullptr == m_VB && nullptr == m_IB);

	m_VtxCount = _VtxCount;
	m_IdxCount = _IdxCount;

	m_VtxSysMem = new Vertex[_VtxCount];
	memcpy(m_VtxSysMem, _Vtx, sizeof(Vertex) * m_VtxCount);
	m_IdxSysMem = new UINT[_IdxCount];
	memcpy(m_IdxSysMem, _pIdx, sizeof(UINT) * _IdxCount);


	m_VBDesc.ByteWidth = sizeof(Vertex) * _VtxCount;

	// ������ �뵵�� ���� ������ �����ϴ� �뵵���� ����
	// �ڱ� ������ �ƴ� ���� ���� ���е�, ���� ó�� Ȥ�� ����ȭ�� ����?
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	//// ������ �� �ֵ��� ��, ���� ��ġ �� ������ ��� �����ؾ� �ϱ� ����
	//VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//VBDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBDesc.CPUAccessFlags = 0;	// ���� ������ ������ �� ����
	m_VBDesc.Usage = D3D11_USAGE_DEFAULT;

	// ������ų ������ �ʱⵥ���� ���޿�
	D3D11_SUBRESOURCE_DATA tSub = {};

	// �ý��� �ʱ� �޸��� �ּ�
	// ���� �ּҸ� �Ѱܿ� desc�� ���� ���� �� �Ѱ�� ������ �ڵ� ������
	tSub.pSysMem = _Vtx;

	// ���ؽ� �뵵�� D3D11 ��ü ����
	if (FAILED(DEVICE->CreateBuffer(&m_VBDesc, &tSub, m_VB.GetAddressOf())))
	{
		return E_FAIL;
	}


	// 3. Index Buffer ����
	// ��ġ�� ���� ����ȭ�ϱ� ���� ���
	m_IBDesc.ByteWidth = sizeof(Vertex) * _IdxCount;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.Usage = D3D11_USAGE_DEFAULT;

	tSub.pSysMem = _pIdx;
	if (FAILED(DEVICE->CreateBuffer(&m_IBDesc, &tSub, m_IB.GetAddressOf())))
	{
		return E_FAIL;
	}
}
