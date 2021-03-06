#include "stdafx.h"
#include "CubeObject.h"


CCubeObject::CCubeObject()
{
	m_fRotationSpeed = 50.0f;
}


CCubeObject::~CCubeObject()
{
}

void CCubeObject::Animate(float fTimeElapsed)
{
	m_fRotationAngle += (float)D3DXToRadian(m_fRotationSpeed * fTimeElapsed);
	D3DXMatrixRotationY(&m_d3dxmtxRotate, m_fRotationAngle);

	m_d3dxmtxWorld = m_d3dxmtxScale * m_d3dxmtxRotate * m_d3dxmtxTranlate;
}