#include "stdafx.h"
#include "MainBoneObject.h"


CMainBoneObject::CMainBoneObject(int nBoneCount, CBoneMesh* pBoneMesh)
{
	BuildObject(nBoneCount, pBoneMesh);
}

CMainBoneObject::~CMainBoneObject()
{
	for (auto boneObject : m_vpBoneObjects) delete boneObject;
	m_vpBoneObjects.empty();
}

D3DXVECTOR3 CMainBoneObject::GetEndEffector()
{
	return D3DXVECTOR3();
}

void CMainBoneObject::AttachBoneObject(CBoneObject* pBoneObject)
{
	m_vpBoneObjects.push_back(pBoneObject);
}

void CMainBoneObject::Animate(float fTimeElapsed)
{
	D3DXMATRIX d3dxmtxParent;
	D3DXMatrixIdentity(&d3dxmtxParent);

	for (auto boneObject : m_vpBoneObjects)
	{
		d3dxmtxParent = boneObject->Animate(fTimeElapsed, d3dxmtxParent);
	}
}

void CMainBoneObject::Render(ID3D11DeviceContext* pd3dDeviceContext)
{
	for (auto boneObject : m_vpBoneObjects)
	{
		boneObject->Render(pd3dDeviceContext);
	}
}

bool CMainBoneObject::IsVisible(CCamera * pCamera)
{
	return true;
}

void CMainBoneObject::BuildObject(int nBoneCount, CBoneMesh* pBoneMesh)
{
	for (int n = 0; n < nBoneCount; ++n)
	{
		CBoneObject* pBoneObject = new CBoneObject();
		pBoneObject->SetMesh(pBoneMesh);
		pBoneObject->SetPosition(0, n == 0 ? 0 : 20, 0);

		AttachBoneObject(pBoneObject);
	}
}
