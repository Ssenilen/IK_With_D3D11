#include "stdafx.h"
#include "MainBoneObject.h"
#include "FabrikHelper.h"


CMainBoneObject::CMainBoneObject(const int nBoneCount, const float fBoneLength, CBoneMesh* pBoneMesh)
{
	BuildObject(nBoneCount, fBoneLength, pBoneMesh);
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

	CFabrikHelper::ExecuteFabrik(m_vpBoneObjects, D3DXVECTOR3(50.0f, 0.0f, 0.0f));

	// Fabrik에선 잠시 꺼두자.
	//for (auto boneObject : m_vpBoneObjects)
	//{
	//	d3dxmtxParent = boneObject->Animate(fTimeElapsed, d3dxmtxParent);
	//}
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

void CMainBoneObject::BuildObject(const int nBoneCount, const float fBoneLength, CBoneMesh* pBoneMesh)
{
	for (int n = 0; n < nBoneCount; ++n)
	{
		CBoneObject* pBoneObject = new CBoneObject();
		pBoneObject->SetMesh(pBoneMesh);
		pBoneObject->SetPosition(0.0f, n * fBoneLength, 0.0f);
		pBoneObject->SetBoneLength(fBoneLength);

		AttachBoneObject(pBoneObject);
	}
}
