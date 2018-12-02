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

void CBoneObject::Render(ID3D11DeviceContext* pd3dDeviceContext)
{
	OnPrepareRender();
	CShader::UpdateShaderVariable(pd3dDeviceContext, &m_d3dxmtxWorld);
	if (m_pMesh) m_pMesh->Render(pd3dDeviceContext);
}
