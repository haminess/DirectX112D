#include "pch.h"
#include "CTransform.h"
#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_IndependentScale(false)
{
}

CTransform::~CTransform()
{
}


void CTransform::SaveComponent(FILE* _File)
{
	fwrite(&m_RelativePos, sizeof(Vector3), 1, _File);
	fwrite(&m_RelativeScale, sizeof(Vector3), 1, _File);
	fwrite(&m_RelativeRotation, sizeof(Vector3), 1, _File);
	fwrite(&m_IndependentScale, sizeof(bool), 1, _File);
}

void CTransform::LoadComponent(FILE* _FILE)
{
	fread(&m_RelativePos, sizeof(Vector3), 1, _FILE);
	fread(&m_RelativeScale, sizeof(Vector3), 1, _FILE);
	fread(&m_RelativeRotation, sizeof(Vector3), 1, _FILE);
	fread(&m_IndependentScale, sizeof(bool), 1, _FILE);
}

void CTransform::FinalTick()
{
	// ���� ���
	// ����, �׵���� ����
	//	|1	 0	 0	 0|
	//	|0	 1	 0	 0|
	//	|0	 0	 1	 0|
	//	|0	 0	 0	 1|
	m_matWorld = XMMatrixIdentity();

	// ���� �߿�!!!
	// ũ�� ����� �밢��
	//				|Sx	 0	 0	 0|		
	//	|a, b, c|	|0	 Sy	 0	 0|	=>	|a * Sx, b * Sy, c * Sz, 1|
	//				|0	 0	 Sz	 0|		
	//				|0	 0	 0	 1|		
	Matrix matScale = XMMatrixScaling(m_RelativeScale.x, m_RelativeScale.y, m_RelativeScale.z);



	// ȸ�� ���
	// (x, y) ��ǥ�� ���� �踸ŭ ȸ�� -> (xcos - ysin, xsin + ycos)
	//				|cos	cos	0	0|		xcos - ysin
	//	|x, y, z|	|sin	sin	0	0|	=>	xsin + ycos
	//				|0		0		1|		z		z
	//				|0		0		0|		1		1
	// �� �࿡ ���� ����� ������
	Matrix matRot = XMMatrixRotationX(m_RelativeRotation.x)
				  * XMMatrixRotationY(m_RelativeRotation.y)
				  * XMMatrixRotationZ(m_RelativeRotation.z);


	// �̵� ����� 4��
	//				|1	0	0	0|		
	//	|x, y, z|	|0	1	0	0|	=>	|x + Tx, y + Ty, z + Tz, 1|
	//				|0	0	1	0|		
	//				|tX	tY	tZ	1|		
	Matrix matTrans = XMMatrixTranslation(m_RelativePos.x, m_RelativePos.y, m_RelativePos.z);

	// ũ�� * ȸ�� * �̵� ����
	m_matWorld = matScale * matRot * matTrans;

	// ���� ���� ����
	m_LocalDir[(UINT)DIR_TYPE::RIGHT]	= Vector3(1.f, 0.f, 0.f);
	m_LocalDir[(UINT)DIR_TYPE::UP]		= Vector3(0.f, 1.f, 0.f);
	m_LocalDir[(UINT)DIR_TYPE::FRONT]	= Vector3(0.f, 0.f, 1.f);

	for (int i = 0; i < 3; ++i)
	{
		// ȸ�� ��ĸ� �����ϱ� ���� 4��° ��ǥ ��ġ��ǥ(�̵���ȯ�� ����) ���� �ʴ´�

		// 1. coord		4��° ��ǥ�� 1�� Ȯ���ؼ� ���
		// 2. normal	4��° ��ǥ�� 0�� Ȯ���ؼ� ���
		//XMVector3TransformCoord(m_LocalDir[i], matRot);
		m_WorldDir[i] = m_LocalDir[i] = XMVector3TransformNormal(m_LocalDir[i], matRot);
	}

	// �θ� �ִٸ�
	if (GetOwner()->GetParent())
	{
		const Matrix& matParentWorld = GetOwner()->GetParent()->Transform()->GetWorldMat();

		// �ڽ� ������Ʈ�� �θ� ������Ʈ�� ũ�⿡ ������ �޴´�.
		if (false == m_IndependentScale)
		{
			m_matWorld *= matParentWorld;
		}

		// �ڽ� ������Ʈ�� �������� ũ�⸦ �����ϰ� ���� ���
		else
		{
			Vector3 vParentWorldScale = GetOwner()->GetParent()->Transform()->GetRelativeScale();
			Matrix matParentScale = XMMatrixScaling(vParentWorldScale.x, vParentWorldScale.y, vParentWorldScale.z);
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, matParentScale);

			// �ڽ� LocalMat * �θ�ũ��-1 * �θ�������(�θ�ũ�� * �θ�ȸ�� * �θ��̵�)
			m_matWorld = m_matWorld * matParentScaleInv * matParentWorld;
		}



		// WorldDir(���� ����) ���ϱ�
		m_WorldDir[(UINT)DIR_TYPE::RIGHT] = Vector3(1.f, 0.f, 0.f);
		m_WorldDir[(UINT)DIR_TYPE::UP] = Vector3(0.f, 1.f, 0.f);
		m_WorldDir[(UINT)DIR_TYPE::FRONT] = Vector3(0.f, 0.f, 1.f);
		for (int i = 0; i < 3; ++i)
		{
			// ũ�� ���Ϳ� �̵� ���ʹ� ������� �ʰ� �Ѵ�.
		
			// �̵����ʹ� ������ ������ �ȵ�(���� ��ķ� ����ϱ� ������)
			// ���� Coord�� �ƴ϶� Normal�� ����ؾ� �Ѵ�.
			m_WorldDir[i] = XMVector3TransformNormal(m_WorldDir[i], m_matWorld);

			// ũ�� ���ʹ� ����� ���ۿ� ���� ���� ������ ũ�Ⱑ �پ��ϰ� �ݿ��ȴ�.
			// ���� ũ�Ⱑ 1�� �ǵ��� ���⺤�͸� ����ȭ
			m_WorldDir[i].Normalize();
		}
	}
}

Vector3 CTransform::GetWorldScale()
{
	Vector3 vScale = m_RelativeScale;

	if (false == m_IndependentScale)
	{
		CGameObject* pParent = GetOwner()->GetParent();

		while (pParent)
		{
			vScale *= pParent->Transform()->GetRelativeScale();

			if (pParent->Transform()->m_IndependentScale)
				break;
			else
				pParent = pParent->GetParent();
		}
	}

	return vScale;
}

void CTransform::Binding()
{
	CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);

	g_Trans.matWorld = m_matWorld;
	g_Trans.matWV = g_Trans.matWorld * g_Trans.matView;
	g_Trans.matWVP = g_Trans.matWV * g_Trans.matProj;

	// ��� ���۷� �޸� ������
	// GPU Memory(DRAM?)���� �̵�
	pCB->SetData(&g_Trans);

	// GPU���� ����� �� �ֵ��� 
	// GPU Memory�� GPU �������͸� ���ε�(����)�Ѵ�.
	pCB->Binding();
}