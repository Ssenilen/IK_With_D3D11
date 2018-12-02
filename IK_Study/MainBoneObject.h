#pragma once
#include "GameObject.h"
#include "BoneObject.h"
#include "BoneMesh.h"

enum class IK_TYPE {
	IKT_FABRIK,
	IKT_CCD,
	IKT_AnalyticIK
};

class CMainBoneObject : public CGameObject
{
public:
	CMainBoneObject(const int nBoneCount, const float fBoneLength, CBoneMesh* pBoneMesh);
	~CMainBoneObject();

	D3DXVECTOR3 GetEndEffector();

	virtual void Animate(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext* pd3dDeviceContext);
	virtual bool IsVisible(CCamera *pCamera);

private:
	void BuildObject(const int nBoneCount, const float fBoneLength, CBoneMesh* pBoneMesh);
	void AttachBoneObject(CBoneObject* pBoneObject);

	std::vector<CBoneObject*> m_vpBoneObjects;
};

