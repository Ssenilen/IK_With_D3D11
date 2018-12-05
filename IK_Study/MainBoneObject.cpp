#include "stdafx.h"
#include "MainBoneObject.h"
#include "FabrikHelper.h"
#include "CCDHelper.h"


CMainBoneObject::CMainBoneObject(const IK_TYPE iktype, const int nBoneCount, const float fBoneLength, CBoneMesh* pBoneMesh) : m_IKType(iktype)
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

	switch (m_IKType)
	{
	case IK_TYPE::IKT_FABRIK:
		CFabrikHelper::ExecuteFabrik(m_vpBoneObjects, D3DXVECTOR3(50.0f, 0.0f, 0.0f));
		break;
	case IK_TYPE::IKT_CCD:
		CCCDHelper::ExecuteCCD(m_vpBoneObjects, D3DXVECTOR3(75.0f, 20.0f, 50.0f));
		break;
	case IK_TYPE::IKT_AnalyticIK:
		break;
	}

	// IK에서 각각 Animate를 해준다.
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
	D3DXMATRIX d3dxmtxParent;
	D3DXMatrixIdentity(&d3dxmtxParent);

	for (int n = 0; n < nBoneCount; ++n)
	{
		CBoneObject* pBoneObject = new CBoneObject();
		pBoneObject->SetMesh(pBoneMesh);
		pBoneObject->SetPosition(0.0f, n == 0 ? 0 : fBoneLength, 0.0f);
		pBoneObject->SetBoneLength(fBoneLength);

		AttachBoneObject(pBoneObject);
	}
	// FABRIK은 WorldMatrix 기준으로 동작한다. 
	// Animate를 한 번 시켜줘서 WorldMatrix를 만들어주자.
	for (auto boneObject : m_vpBoneObjects)
	{
		d3dxmtxParent = boneObject->Animate(0, d3dxmtxParent);
	}
}
