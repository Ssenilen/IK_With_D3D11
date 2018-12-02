#include "stdafx.h"
#include "BoneObject.h"


CBoneObject::CBoneObject() 
{
}


CBoneObject::~CBoneObject()
{
}

D3DXMATRIX CBoneObject::Animate(float fTimeElapsed, const D3DXMATRIX& pParentMatrix)
{
	m_d3dxmtxLocal = m_d3dxmtxScale * m_d3dxmtxRotate * m_d3dxmtxTranlate;

	m_d3dxmtxWorld = m_d3dxmtxLocal * pParentMatrix;

	return m_d3dxmtxWorld;
}

void CBoneObject::FabrikAnimate()
{
	m_d3dxmtxWorld = m_d3dxmtxScale * m_d3dxmtxRotate * m_d3dxmtxTranlate;
}

void CBoneObject::Render(ID3D11DeviceContext* pd3dDeviceContext)
{
	OnPrepareRender();
	CShader::UpdateShaderVariable(pd3dDeviceContext, &m_d3dxmtxWorld);
	if (m_pMesh) m_pMesh->Render(pd3dDeviceContext);
}

D3DXVECTOR3 CBoneObject::GetPosition()
{
	return D3DXVECTOR3(m_d3dxmtxWorld._41, m_d3dxmtxWorld._42, m_d3dxmtxWorld._43);
}

void CBoneObject::SetPosition(float x, float y, float z)
{
	m_d3dxmtxWorld._41 = x;
	m_d3dxmtxWorld._42 = y;
	m_d3dxmtxWorld._43 = z;
}

void CBoneObject::SetFabrikPosition(D3DXVECTOR3 d3dxvPosition)
{
	m_d3dxmtxTranlate._41 = d3dxvPosition.x;
	m_d3dxmtxTranlate._42 = d3dxvPosition.y;
	m_d3dxmtxTranlate._43 = d3dxvPosition.z;
}

void CBoneObject::Rotate(D3DXMATRIX d3dmtxRotate)
{
	m_d3dxmtxRotate = d3dmtxRotate;
}
