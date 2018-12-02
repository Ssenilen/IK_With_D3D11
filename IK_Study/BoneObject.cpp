#include "stdafx.h"
#include "BoneObject.h"


CBoneObject::CBoneObject() : m_pChildBone(nullptr), m_pParentBone(nullptr)
{
}


CBoneObject::~CBoneObject()
{
}

void CBoneObject::Animate(float fTimeElapsed, bool bUseLocalMatrix)
{
	if (!bUseLocalMatrix)
	{
		m_d3dxmtxWorld = m_d3dxmtxScale * m_d3dxmtxRotate * m_d3dxmtxTranlate;
	}
	else
	{
		if (bTestFlag)
		{
			m_fRotationAngle += (float)D3DXToRadian(25.0f * fTimeElapsed);
			D3DXMatrixRotationZ(&m_d3dxmtxRotate, m_fRotationAngle);

			m_d3dxmtxLocal = m_d3dxmtxScale * m_d3dxmtxRotate * m_d3dxmtxTranlate;
		}
		else
		{
			m_d3dxmtxLocal = m_d3dxmtxScale * m_d3dxmtxRotate * m_d3dxmtxTranlate;
		}

		D3DXMATRIX d3dxmtxParent;
		D3DXMatrixIdentity(&d3dxmtxParent);

		if (m_pParentBone) d3dxmtxParent = m_pParentBone->GetWorldMatrix();

		m_d3dxmtxWorld = m_d3dxmtxLocal * d3dxmtxParent;

		if (m_pChildBone) m_pChildBone->Animate(fTimeElapsed, bUseLocalMatrix);
	}
}

void CBoneObject::Render(ID3D11DeviceContext* pd3dDeviceContext)
{
	OnPrepareRender();
	CShader::UpdateShaderVariable(pd3dDeviceContext, &m_d3dxmtxWorld);
	if (m_pMesh) m_pMesh->Render(pd3dDeviceContext);

	if (m_pChildBone) m_pChildBone->Render(pd3dDeviceContext);
}
