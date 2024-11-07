#include "pch.h"
#include "CMaterial.h"
#include "CGraphicShader.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CTexture.h"


CMaterial::CMaterial(bool _EngineRes)
	: CAsset(ASSET_TYPE::MATERIAL, _EngineRes)
{
	m_Const.v4Arr[0] = Vector4(1.f, 0.f, 0.f, 1.f);
}

CMaterial::~CMaterial()
{
}

CMaterial* CMaterial::Clone()
{
	CMaterial* pClone = new CMaterial(*this);
	pClone->m_SharedMtrl = this;
	return pClone;
}

CMaterial::CMaterial(const CMaterial& _Origin)
	: CAsset(_Origin)
	, m_Shader(_Origin.m_Shader)
	, m_Const(_Origin.m_Const)
	, m_arrTex{}
	, m_SharedMtrl(_Origin.m_SharedMtrl)
{
	for (UINT i = 0; i < (UINT)TEX_PARAM::TEX_END; ++i)
	{
		m_arrTex[i] = _Origin.m_arrTex[i];
	}
}


int CMaterial::Save(const wstring& _FilePath)
{
	// m_SharedMtrl ���� ������ ����Ų�� == ���������̴�.
	// ���� ������ �����Ϸ��� �ߴ�. ==> ����
	// ���� ���� = ������ �÷��̵Ǵ� ���߿� �Ͻ������� ���� ���� ������ ����
	assert(!m_SharedMtrl.Get());

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");
	assert(pFile);

	SaveAssetRef(m_Shader, pFile);

	fwrite(&m_Const, sizeof(MtrlConst), 1, pFile);

	for (int i = 0; i < (int)TEX_PARAM::TEX_END; ++i)
	{
		SaveAssetRef(m_arrTex[i], pFile);
	}

	fclose(pFile);

	return S_OK;
}


int CMaterial::Load(const wstring& _strFilePath)
{
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");
	assert(pFile);

	LoadAssetRef(m_Shader, pFile);

	fread(&m_Const, sizeof(MtrlConst), 1, pFile);

	for (int i = 0; i < (int)TEX_PARAM::TEX_END; ++i)
	{
		LoadAssetRef(m_arrTex[i], pFile);
	}

	fclose(pFile);

	return S_OK;
}


void CMaterial::Binding()
{
	if (nullptr == m_Shader)
		return;


	// Texture ��� ���۷� ���ε�
	for (int i = 0; i < TEX_END; ++i)
	{
		if (nullptr == m_arrTex[i])
		{
			m_Const.bTex[i] = 0;
			// �ٸ� ������ۿ� ������ ���ε� �� ���� ������ �� �ֱ� ������ �ʱ�ȭ
			CTexture::Clear(i);
			continue;
		}

		// Ÿ�Կ� �´� ��� ���۷� ���ε�
		m_Const.bTex[i] = 1;
		m_arrTex[i]->Binding(i);
	}

	// ��� ������ ���ε�
	static CConstBuffer* pMtrlCB = CDevice::GetInst()->GetCB(CB_TYPE::MATERIAL);
	pMtrlCB->SetData(&m_Const);
	pMtrlCB->Binding();

	// Shader ���ε�
	m_Shader->Binding();
}

void CMaterial::SetTexParam(TEX_PARAM _Param, Ptr<CTexture> _tex)
{
	m_arrTex[_Param] = _tex;
}


Ptr<CTexture> CMaterial::GetTexture(int _idx) { return   m_arrTex[_idx]; }